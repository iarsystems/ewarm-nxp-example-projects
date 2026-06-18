#ifndef _UTILS_API_H_
#define _UTILS_API_H_
#include "lpc_types.h"

char * s_itoa(char * buffer, int i);
void * s_memset(void * dst,  int ch, int length);
void * s_memcpy(void * s1, const void *s2, int n);
uint8_t s_strncmp(uint8_t *s1,uint8_t *s2,uint32_t num);
void u_itoa( int n, char s[],int d );
void malloc_init(void);
void * u_malloc(uint32_t size);
void u_free(void *packet);
//void delay(uint32_t count);
#endif
