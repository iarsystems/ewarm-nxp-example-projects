/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : commands.h
 *    Description : Command header file
 *
 *    History :
 *    1. Date        : 18.4.2008 
 *       Author      : Stoyan Choynev
 *       Description : Initila Revision
 *
 *    $Revision: 4271 $
 **************************************************************************/
#ifndef   __COMMANDS_H
  #define __COMMANDS_H

/** include files **/

/** definitions **/
struct s_cmd
{
  const char * cmd;
  const struct s_cmd * next;
  const struct s_cmd * child;
  int (* func)(int * argc, char *** argv,const struct s_cmd * cmd);
  const char * help;
};

/** default settings **/

/** public data **/

/** public functions **/
extern char * CmdEdit(char * cmd, char * history ,int c_size, int h_size);
extern int CmdParse(char ** argv, char * argbuff);
extern int CmdExec(int * argc, char *** argv,const struct s_cmd * cmd);
extern int Help(int * argc, char *** argv,const struct s_cmd * cmd);

#endif /*__COMMANDS_H*/
