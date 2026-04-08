/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : command.c
 *    Description : Command Line Interpreter
 *
 *    History :
 *    1. Date        : 03.4.2008 
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yfuns.h>
#include <assert.h>
#include "commands.h"
/** local definitions **/
#define CTRL(c) (c - '@')
#define _TAB  4

/** default settings **/

/** external functions **/

/** external data **/
extern const unsigned char Prompt[];
/** internal functions **/
void PrintHelp(const struct s_cmd * cmd, int tab);

/** public data **/

/** private data **/

/** public functions **/
/*************************************************************************
 * Function Name: CmdEdit
 * Parameters: char * cmd - pointer to command line buffer
 *             char * history - pointer to history buffer
 *             int c_size - size fo the cmd buffer
 *             int h_size - size of the history buffer
 * Return: pointer ot cmd buffer
 *
 * Description:Command line editor with history
 *
 *************************************************************************/
char * CmdEdit(char * cmd, char * history ,int c_size, int h_size)
{
int c;
char * h_push, * h_last, * h_read;

int l=0, esc = 0;
int cursor = 0;

  //init history indexes
  //Be careful here!!! If the history memory is not init properly
  //this will crash
  //
  if(history)
  {
    //
    assert(h_size>c_size);
    //
    for(h_push=history;/*empty*/;/*empty*/)
    {
      int length = strlen(h_push);
    
      if(0 == length) 
      {
        break;
      }
      h_push += (length + 1);
      //
      assert(h_push < (history + h_size));
    }
  
    h_last = h_push;
  
    for(char * next = h_push;/*empty*/;/*empty*/)
    {
      int length = strlen(next);

      if(length > 0)
      {
        h_last = next;
      }  

      next += length+1;
      
      if(next >= (history+h_size)) next = history;
    
      if(next == h_push) break;
       
    }
  
    h_read = NULL;
  }
  //clear command buffer
  cmd[l]='\0';
  
  //Show Prompt
  printf("\n\r%s",Prompt);
  
  while (1)
	{
	  if ( 0 > (c=getchar())/*read(NULL, &c, 1)*/)
	  {
		  continue;
	  }

	  /* handle ANSI arrows*/
	  if (esc)
	  {
		  if (esc == '[')
		  {
		    /* remap to readline control codes */
		    switch (c)
		    {
			    case 'A': /* Up */
			      c = CTRL('^');
			    break;

			    case 'B': /* Down */
			      c = CTRL('_');
			    break;

			    case 'C': /* Right */
			      c = CTRL('\\');
			    break;

			    case 'D': /* Left */
			      c = CTRL(']');
			    break;
          
        case 'K':  /* EOL */
            c = CTRL('E');
          break;
          
        case 'H':   /* HOME */
            c = CTRL('A');
          break;
          
			    default:
			      c = 0;
		     }
         esc = 0;
		  }
		  else
		  {
		    esc = (c == '[') ? c : 0;
		    continue;
		  }
	  }

	  if (c == 0) continue;
    
	  if (c == '\n') continue;

	  if (c == '\r')
	  {
		  break;
	  }

	  if (c == '\x1B')
	  {//ESC
      esc = 1;
		  continue;
	  }

    // back word, backspace, delete 
	  if (c == CTRL('W') || c == CTRL('H') || c == '\x7F')
	  {
		  int back = cursor;

		  if (c == CTRL('W')) /* word */
		  {
          if(0 == back) continue;
          
		      while (back && cmd[back - 1] == ' ')
		      {
			      back--;
		      }

		      while (back && cmd[back - 1] != ' ')
		      {
			      back--;
		      }
		  }
      else if('\x7F' == c)
      {
        if( cursor >= l) continue;
       
			  putchar(cmd[cursor]);
  			cursor++;
      }
		  else /* char */
		  {
		    if (0 == back ) 
		    {
          putchar('\a');
			    continue;
		    }
		    back--;
		  }

		  if(back != cursor)
		  {
        int i, k;
        
        for( i = back, k = cursor; k < l ; i++, k++ ) cmd[i] = cmd[k];
        l=i;
        for(;i < k ; i++) cmd[i] = ' ';
        for(;cursor>back;cursor--) putchar('\b');
        printf("%s",&cmd[cursor]);
        cmd[l] = '\0';
        for(i=cursor;i<k;i++) putchar('\b');
        continue;
		  }
	  }

	  /* clear line */
	  if (c == CTRL('U'))
	  {
      printf("\r%s",Prompt);
      for(int i = 0 ; i < l ; i++) putchar(' ');
      printf("\r%s",Prompt);
		  l = cursor = 0;
      cmd[l] = '\0';
		  continue;
	  }

	   /* kill to EOL */
	   if (c == CTRL('K'))
	   {
		  if (cursor == l)
		      continue;

      int i;
      
		  for (i = cursor; i < l; i++) putchar(' ');
			
      for (i = cursor; i < l; i++) putchar('\b');
      
	    l = cursor;
      cmd[l] = '\0';
		  continue;
	  }

    /* EOT */
	  if (c == CTRL('D'))
	  {
      if (l)
      {
		    continue;
      }

		  strcpy(cmd, "Quit");
		  l = cursor = strlen(cmd);
		  printf("%s",cmd);
		  break;
	  }

    /* left/right cursor motion */
	  if (c == CTRL(']') || c == CTRL('\\'))
	  {
		  if (c == CTRL(']')) /* Left */
		  {
		    if (cursor)
		    {
          putchar('\b');
			    cursor--;
		    }
		  }
		  else /* Right */
		  {
        if (cursor < l)
		    {
          putchar(cmd[cursor]);
  			  cursor++;
	      }
		  }
      continue;
	  }

	  /* start of line */
 	  if (c == CTRL('A'))
	  {
      printf("\r%s",Prompt);
      cursor = 0;
		  continue;
    }

	  /* end of line */
	  if (c == CTRL('E'))
	  {
		  if (cursor < l)
		  {
        printf("%s",&cmd[cursor]);
        cursor = l;
		  }

		  continue;
    }
    
    //History
    if(history && ((CTRL('^') == c) || (CTRL('_') == c)))
    {
      //the history is empty
      if(h_last == h_push) continue;
         
      if(h_read)
      {//not first read
        char * check = h_read;
        char * next = h_read;
    
        while(1)
        {//
          do
          {
            next += strlen(next)+1;
            if(next >= (history + h_size)) next = history;
          }while(0 == *next);
        
          if(next == check) break;
        
          h_read = next;
          //Down key. Break immediately
          if(CTRL('_') == c) break;
        }
      }
      else h_read = h_last; //first read. Read the last record
      //Clear line
      printf("\r%s",Prompt);
      for(int i = 0 ; i < l ; i++) putchar(' ');

      for(int i = 0 ; i < l ; i++) putchar('\b');
      //copy commad from history
      strcpy(cmd,h_read);
      cursor = l = strlen(cmd);
      //Show command
      printf("%s",cmd);
      
      continue; 
    }
    
    //Tab and normal character
    if((' ' <= c) || ('\t' == c))
    {
      int i,k,t ;
      if('\t' == c)
      {
        t = _TAB;
        c = ' ';
      }
      else
      {
        t = 1;
      }
      for( k = l,i = l+t; k > cursor;/*empty*/)
      {
        i--;
        k--;
        if(i < c_size) cmd[i] = cmd[k];                 //shift buffer
      }
      if((l+=t) >= c_size) l = c_size-1;                //Set l
      
      for(i=0; i < t; i++) 
      {
        if(k < l)  cmd[k++]=c;                         //insert spaces
      }
      cmd[l] = '\0';
      printf("%s",&cmd[cursor]);
      cursor = k;
      for(i=l; i > cursor;i--) putchar('\b');  //
      continue;
    }
	}

	if (l <= 0) return NULL;

  if(history && (0 != strcmp(cmd,h_last)))
  {
    //check command length
    int length = strlen(cmd)+1;
    if(h_push+length > (history + h_size))
    {//no space at the end of history.
      h_push = history;         //point at history start
    }
    //save the command in history
    strcpy(h_push, cmd);
    //
    h_push += (length);
    //
    if((h_push) >= (history + h_size)) h_push = history;
    //clear the last record
    memset(h_push,'\0',strlen(h_push));
  }
  return cmd;
}

/*************************************************************************
 * Function Name: CmdParse
 * Parameters:char **argv - pointer to a char pointer table.
 *            char * argbuff - pointer to the command buffer
 * Return: The Number of the substrings
 *
 * Description: Simply devide the command line in to null terminated sub 
 *              strings. Each sub string contains only one word.
 *************************************************************************/
int CmdParse(char ** argv, char * argbuff)
{
  int argc = 0;
  char ch, oldch = ' ';
  //while end of string
  while('\0' != (ch = *argbuff))
  {
    if(' ' == oldch)
    {//
      if(' ' != ch)
      {//arg string begin 
        oldch = ch;
        //save arg address
        argv[argc++] = argbuff;
      }
    }
    else
    {//
      if(' ' == ch)
      {//arg string end
        oldch = ch;
        //Terminate the arg string
        *argbuff = '\0';
      }
    }
    argbuff++;
  }
  return argc;
}
/*************************************************************************
 * Function Name: CmdExec
 * Parameters: argc - pointer to the argument counter
 *             argv -  pointer to the argument pointer table
 *             cmd - command table pointer
 * Return: 0  command is executed
 *         -1 error
 * Description:
 *
 *************************************************************************/
int CmdExec(int * argc, char *** argv,const struct s_cmd * cmd)
{
  do
  {//check all commands 
    if((*argc) && (0 == strcmp(cmd->cmd, **argv)))
    {//Command mach
      //Poin to next arg
      (*argv)++;
      //Dect arg counter
      (*argc)--;
      //call command func
      return (*(cmd->func))(argc, argv,cmd->child);
    }
  }while(NULL != (cmd = cmd->next));
  //No mach return with -1
  return -1;
}

/*************************************************************************
 * Function Name: Help
 * Parameters: argc - pointer to the argument counter
 *             argv -  pointer to the argument pointer table
 *             cmd - command table pointer
 * Return: 0  help printed
 *         -1 error
 * Description: Prints command Help
 *
 *************************************************************************/
int Help(int * argc, char *** argv,const struct s_cmd * cmd)
{
static int help_entry = 0;
  
  help_entry++;
  
  if(*argc)
  {
    do
    {
      if((0 == strcmp(cmd->cmd, **argv)))
      {
        (*argv)++;
        (*argc)--;
        
        PrintHelp(cmd,help_entry);
                
        int r = 0;
        
        if(cmd->child) r = Help(argc, argv,cmd->child);
        
        help_entry--;
        return r;
      }
    }while(NULL != (cmd = cmd->next));
  }
  else
  {
    do
    {
      PrintHelp(cmd,help_entry);
            
      if(cmd->child) Help(argc, argv,cmd->child);
      
    }while( NULL != (cmd = cmd->next));
    
    help_entry--;
    return 0;
  }
  
  help_entry--;
  return -1;
}

/** private functions **/
void PrintHelp(const struct s_cmd * cmd, int tab)
{
  printf("\n\r");
        
  for(/*empty*/ ; 0 < tab ; tab--) putchar('\t');
        
  printf("%s",cmd->cmd);
  printf("\t%s",cmd->help);
}


