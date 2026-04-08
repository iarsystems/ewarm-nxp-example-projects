/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2009 Embedded Artists AB
 *
 * Description:
 *    The Maze Game
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lpc313x_timer_driver.h"
#include "lcd_grph.h"

#include "maze.h"
#include "joystick.h"


/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/
//Color setup
#define COLOR_BKGND             YELLOW
#define COLOR_BORDER            BLUE
#define COLOR_INVISIBLE_WALL	  BLACK
#define COLOR_VISIBLE_WALL      BLACK
#define COLOR_VISITED           OLIVE
#define COLOR_CURSOR            BLACK
#define COLOR_FINISH            RED

#define FIELD_WIDTH		220	// Game field width in px
#define FIELD_HEIGHT	200 // Game field height in px

#define MAXWIDTH        30	// Maze max width in cells
#define MAXHEIGHT       25    // Maze max height in cells

#define DIR_LEFT	    1	// Maze directions
#define DIR_RIGHT     2
#define DIR_UP		    3
#define DIR_DOWN	    4
#define DIR_INIT	    5  // Special maze direction used to mark initial location of maze generation

#define ICON_BPP     24

/*****************************************************************************
 * Local variables
 ****************************************************************************/
static INT_8  mMaze[MAXHEIGHT][MAXWIDTH];
static INT_32	width, height, ox, oy;
static INT_32 currX, currY, lastX, lastY;
static INT_32 cellWidth, cellHeight;
static UNS_32 wallHitCount, moveCount;
static UNS_8  showWalls, finishGame;

static INT_32 dev_timer = 0;


/******************************************************************************
 * Local functions
 *****************************************************************************/


/*****************************************************************************
 *
 * Description:
 *    return cell value or -1 of request out of bounds
 *
 ****************************************************************************/
static INT_8 getCell(INT_32 x, INT_32 y) {
    if((x >= 0) && (y >= 0) && (x < width) && (y < height))
        return mMaze[y][x];
    return -1;
}

/*****************************************************************************
 *
 * Description:
 *    Initialize a maze.
 *
 ****************************************************************************/
static void setupMaze(void) {
	INT_32 x = width / 2, y = height / 2;
	INT_32 availDirs[4], dirCount;
  INT_8 dir = 0;
    
	memset(mMaze, 0, sizeof(mMaze));
	mMaze[y][x] = DIR_INIT; //Mark start position
	do {
		dirCount = 0;
		if(getCell(x - 1, y) == 0)
			availDirs[dirCount++] = DIR_LEFT;
		if(getCell(x + 1, y) == 0)
			availDirs[dirCount++] = DIR_RIGHT;
		if(getCell(x, y - 1) == 0)
			availDirs[dirCount++] = DIR_UP;
		if(getCell(x, y + 1) == 0)
			availDirs[dirCount++] = DIR_DOWN;

		if(dirCount) { // if there's at least one way to unmarked cell - go there
			dir = availDirs[rand() % dirCount];
			switch(dir) {
				case DIR_LEFT:	x--; break;
				case DIR_RIGHT: x++; break;
				case DIR_UP:	y--; break;
				case DIR_DOWN:	y++; break;
			}
			mMaze[y][x] = dir; 	// Store direction from which we entered this cell
		} else { 				// no ways left to progress - retard one cell
			switch(mMaze[y][x]) {
				case DIR_LEFT:	x++; break; // got here from right (i.e. LEFT move taken to get there)
				case DIR_RIGHT: x--; break;
				case DIR_UP:	y++; break;
				case DIR_DOWN:	y--; break;
			}
		}
	} while(mMaze[y][x] != DIR_INIT); // if we did not return to start - repeat generating.
}


/*****************************************************************************
 *
 * Description:
 *    Check for ability to move to selected direction
 *
 ****************************************************************************/
static INT_8 checkWall(INT_32 x, INT_32 y, INT_8 dir) {
    INT_8 cell = getCell(x, y);
    if(cell < 0)
        return 0;
    switch(dir) {
        case DIR_LEFT:  return (cell == DIR_RIGHT)  || (getCell(x - 1, y) == DIR_LEFT);
        case DIR_RIGHT: return (cell == DIR_LEFT)   || (getCell(x + 1, y) == DIR_RIGHT);
        case DIR_UP:    return (cell == DIR_DOWN)   || (getCell(x, y - 1) == DIR_UP);
        case DIR_DOWN:  return (cell == DIR_UP)     || (getCell(x, y + 1) == DIR_DOWN);
    }
    return 0;
}

/*****************************************************************************
 *
 * Description:
 *    Draw a single wall.
 *
 ****************************************************************************/
static void drawWall(INT_32 x, INT_32 y, INT_8 dir, UNS_16 color) {
    INT_32 cellX, cellY;
    cellX = ((dir == DIR_RIGHT)? (x + 1): x) * cellWidth  + ox;
    cellY = ((dir == DIR_DOWN)?  (y + 1): y) * cellHeight + oy;

    switch(dir) {
        case DIR_LEFT:
        case DIR_RIGHT:
            lcd_fillRect(cellX, cellY, cellX, cellY + cellHeight, color);
            break;
        case DIR_UP:
        case DIR_DOWN:
            lcd_fillRect(cellX, cellY, cellX + cellWidth, cellY, color);
            break;
    }
}

/*****************************************************************************
 *
 * Description:
 *    Draw a maze.
 *
 ****************************************************************************/
static void drawMaze(void) {

  INT_32 x, y;
  
  /* background */
  lcd_fillRect(0, 0, 240, 320, OLIVE);

	if(showWalls) {
		for(x = 0; x < width; x++)
    		for(y = 0; y < height; y++) {
        		if(y && (mMaze[y][x] != DIR_DOWN && mMaze[y - 1][x] != DIR_UP))
            		drawWall(x, y, DIR_UP, COLOR_INVISIBLE_WALL);

		        if(x && (mMaze[y][x] != DIR_RIGHT && mMaze[y][x - 1] != DIR_LEFT))
        		    drawWall(x, y, DIR_LEFT, COLOR_INVISIBLE_WALL);	    
     }
  }

  lcd_drawRect(ox, oy, ox + (width*cellWidth), oy + (height*cellHeight), 
    COLOR_VISIBLE_WALL);
}


/*****************************************************************************
 *
 * Description:
 *    Draw cursor in a current position and mark previous position as visited
 *
 ****************************************************************************/
static void drawCursor(void) {
    INT_32 cellX, cellY;   
    
    if(lastX != currX || lastY != currY) {
        cellX = ox + lastX * cellWidth;
        cellY = oy + lastY * cellHeight;

        lcd_fillRect(cellX + 1, cellY + 1, cellX + cellWidth - 2, 
          cellY + cellHeight - 2, COLOR_VISITED);
                     
    }
    if(currX >= 0 && currY >= 0) {
        cellX = ox + currX * cellWidth;
        cellY = oy + currY * cellHeight;
        
        lcd_fillRect(cellX + 2, cellY + 2, cellX + cellWidth - 2, 
          cellY + cellHeight - 2, COLOR_CURSOR);          
    }
}

/*****************************************************************************
 *
 * Description:
 *    Draw a Finish location (bottom rightmost cell)
 *
 ****************************************************************************/
static void drawFinish(void) {
  INT_8 i = cellWidth / 4;
  INT_32 cellX, cellY;
        
  cellX = ox + (width  - 1) * cellWidth;
  cellY = oy + (height - 1) * cellHeight;
    
  lcd_fillRect(cellX + i + 1, cellY + 2, (cellX + i + 1 )+ cellWidth -2 * i - 1, 
    (cellY + 1) + cellHeight - 2, COLOR_FINISH);
  
  lcd_fillRect(cellX + 2, cellY + i + 1, (cellX + 1) + cellWidth - 2, 
    (cellY + i + 1) + cellHeight - 2 * i - 1, COLOR_FINISH);

}

/*****************************************************************************
 *
 * Description:
 *    Draw current score
 *
 ****************************************************************************/
static void showScore(void) {
  static char str[13];
	INT_32 len;
    
  len = sprintf(str, "M: %d", moveCount);
  lcd_fontColor(YELLOW, BLACK);
  lcd_putString(120 - len * 4, 270, (UNS_8*)str);

	len = sprintf(str, "W: %d", wallHitCount);

  lcd_fontColor(RED, BLACK);
  lcd_putString(160 - len * 4, 270, (UNS_8*)str);
}

/*****************************************************************************
 *
 * Description:
 *    Start the Maze game.
 *
 ****************************************************************************/
static void startMaze(UNS_32 gridSize) 
{
  INT_32 count = 0; 

  width =  gridSize & 0xFF;
  height = (gridSize >> 8) & 0xFF;

  cellWidth = FIELD_WIDTH / width;
  cellHeight = FIELD_HEIGHT / height;

  oy = 50;
  ox = 10;
  ox += ((FIELD_WIDTH - width*cellWidth) / 2);
   
  count = timer_ioctl(dev_timer,
                   TMR_GET_STATUS,
                   TMR_GET_COUNT);
  srand(count);
  setupMaze();
  drawMaze();

  currX = 0;  currY = 0;
  lastX = 0;  lastY = 0;
	wallHitCount = 0;
	moveCount = 0;
	finishGame = 0;

	drawFinish();
  drawCursor();
	showScore();
}


/*****************************************************************************
 *
 * Description:
 *    Show congratulations
 *
 ****************************************************************************/
static void showCongratulations(void) {
	static UNS_8 flash = FALSE;
  UNS_8 key;
    
  if(flash)
    lcd_fontColor(RED, YELLOW);
  else
    lcd_fontColor(WHITE, BLUE);

  lcd_putString(65, 120, "Congratulations!");
  lcd_putString(65, 140, "Press CENTER key");
  
  flash = !flash;
  timer_wait_ms(NULL, 40);
  
  key = joyState();
  if(key == KEY_CENTER)
  { 
    /* new game */
    finishGame = FALSE;
    lcd_fillScreen(BLACK);
    startMaze( 12 + (10 << 8));
  }

}


/*****************************************************************************
 *
 * Description:
 *    Run the Maze game.
 *
 ****************************************************************************/
static void runGame(void)
{
  UNS_8 keypress = 0, dir = 0;
    
  if(finishGame)
  {
    showCongratulations();
  }
  
  else
  {
    keypress = joyState();
        
    switch(keypress) 
    {
      case KEY_LEFT:
        dir = DIR_LEFT;
          currX--;
          break;

      case KEY_RIGHT:
        dir = DIR_RIGHT;
        currX++;
        break;

      case KEY_UP:
        dir = DIR_UP;
        currY--;
        break;

      case KEY_DOWN:
        dir = DIR_DOWN;
        currY++;
        break;
        
      default:
        dir = 0;
    }

    if(dir) 
    {
        if(checkWall(lastX, lastY, dir)) 
        {
          drawCursor();
          lastX = currX, lastY = currY;
          moveCount++;
          if((currX == width - 1) && (currY == height - 1))  //is lower right corner reached
            finishGame = TRUE;
        } 
        else 
        {
          currX = lastX, currY = lastY;
          drawWall(currX, currY, dir, COLOR_VISIBLE_WALL);
          wallHitCount++;
        }
        
        showScore();
    }
  }
}




/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/

/*****************************************************************************
 *
 * Description:
 *    Initialize and start Maze
 *
 ****************************************************************************/
void 
mazeInit(void)
{
	showWalls = TRUE;
  finishGame = FALSE;

  dev_timer = timer_open((void*)TIMER0_BASE, 0);
  if (dev_timer > 0) {
    timer_ioctl(dev_timer,
                TMR_ENABLE,
                1);
  }

  /* width = 12, height = 10 */
  startMaze(12 + (10 << 8));

  while(1) {
    runGame();
    timer_wait_ms(NULL, 300);
  }

}


