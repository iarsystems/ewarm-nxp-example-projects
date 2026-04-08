

#include "lpc_types.h"
#include "string.h"
//#include "malloc.h"
#include "utils_api.h"


//#include "fwu_utils.h"

typedef void (*IAP)(unsigned int [],unsigned int[]);

//----------------------------------------------------------------------------
void * s_memset(void * dst,  int ch, int length)
{
  register char *ptr = (char *)dst - 1;

  while(length--)
     *++ptr = ch;

  return dst;
}

//----------------------------------------------------------------------------
void * s_memcpy(void * s1, const void *s2, int n)
{
   register int mask = (int)s1 | (int)s2 | n;

   if(n == 0)
      return s1;

   if(mask & 0x1)
   {
      register const char *src = s2;
      register char *dst = s1;

      do
      {
         *dst++ = *src++;
      }while(--n);

      return s1;
   }

   if(mask & 0x2)
   {
      register const short *src = s2;
      register       short *dst = s1;

      do
      {
        *dst++ = *src++;
      }while( n -= 2);

      return s1;
   }
   else
   {
      register const int *src = s2;
      register       int *dst = s1;

      do
      {
         *dst++ = *src++;
      }while (n -= 4);

      return s1;
   }
}


uint8_t s_strncmp(uint8_t *s1,uint8_t *s2, uint32_t num)
{
	uint8_t retval=0;
	uint32_t i=0;

 for(i=0;i<num;i++)
 {
  if(*(s1+i) != *(s2+i))
  {
   retval = 1;
   break;
   }
 }
  
  return retval;
}



int divide(int a, int b)
{
  int result = 0;
  int temp = b;
  
  while (temp >= b)
  {
    temp = a - b * result++;
  }
  
  return result-1;
}

void reverse(char s[])
{
  int c, i, j;
  
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
    }  
  }
  
void u_itoa( int n, char s[],int d )
{
  int i, sign, j=0;
  
  if(( sign = n ) < 0 )
    n = -n;
  i = 0;
  do
  {
	s[i++] = n % 10 + '0';
  } while(( n /= 10 ) > 0 );

  if(strlen(s) < d )
  {
	  for(j=d;d>strlen(s);j--)
	  {
		  s[i++]='0';
	  }
  }
  if( sign < 0  )
     s[i++] = '-';
  s[i] = '\0';
  reverse( s );
}

void delay(uint32_t delay)
{
	int i;
	for(i=0;i<delay;i++);
}
