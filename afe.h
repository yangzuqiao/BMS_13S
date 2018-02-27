/**************************************************************************************************/
#ifndef __AFE
#define __AFE
/**************************************************************************************************/
#include"head.h"
/**************************************************************************************************/
void afe_check(void);
void AFE_init(void);
void command_helloall(void);
void command_rollcall(void);
void command_setlastaddress(void);
void command_writeall(uchar addr,uchar data1,uchar data2);
void command_readall(uchar addr);
void sendmsg_to_afe(uchar device,uchar addr,uchar data1,uchar data2);
uint receivemsg_from_afe(uchar device,uchar addr);
uint crc_verify(uchar count);

#endif