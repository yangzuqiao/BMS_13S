/**************************************************************************************************/
#include"afe.h"
/**************************************************************************************************/
void afe_check(void)
{
    uchar i;
    
    // MUX Diag
    // command_writeall(0x8,0x2b,0x03);  // ADCCFG Register: MUXDIAG enable
    // voltage_sample();
    for(i=0x0;i<0x10;i++)
    {
        if((vcell[i] < 0x666) || (vcell[i] > 0x3e80))  // 0.5v,1638;4.88v,16380
        {
            flag_open_cn = 0x1;
            break;
        }
        else
        {
            flag_open_cn = 0x0;
        }
    }
    
    command_readall(0x2);  // STATUS register
    
    if((iic_sum[0] & BIT9) || (iic_sum[1] & BIT9))  // ALRTPEC
    {
        flag_alrtpec = 0x1;
    }
    
    #if 0
    if((iic_sum[0] & BIT8) || (iic_sum[1] & BIT8))  // ALRTACK
    {
        flag_alrtack = 0x1;
    }
    #endif
    
    if((iic_sum[0] & BIT7) || (iic_sum[1] & BIT7))  // ALRTFMEA
    {
        flag_alrtfmea = 0x1;
        
        //command_readall(0xe);  // FMEA register
        //_NOP();
    }
}
/**************************************************************************************************/
void AFE_init(void)
{
    delay_ms(5);
    command_helloall();
    command_rollcall();
    command_setlastaddress();
    
    command_writeall(0x2,0x0,0x0);       // STATUS Register:clear 
    
    #if 0
    command_writeall(0x18,0x0,0x0);      // OV Clear Threshold
    command_writeall(0x19,0xff,0xfc);    // OV Set Threshold
    command_writeall(0x1a,0x0,0x0);      // UV Set Threshold
    command_writeall(0x1b,0xff,0xfc);    // UV Clear Threshold
    command_writeall(0x1c,0xff,0xfc);    // Cell Mismatch Threshold
    command_writeall(0x1e,0x0,0x0);      // OT Set Threshold
    command_writeall(0x1f,0xff,0xf0);    // UT Set Threshold
    #endif
    
    command_writeall(0xc,0x0,0x20);      // ACQCFG:Balance Time->4s
    command_writeall(0x8,0x23,0x3);      // ADCCFG:Enable AIN1 AIN2,MUXDIAG disable,OverSamples:4
                                         // Unipolar mode,ALRMPEC ALRMACK
    command_writeall(0x9,0xff,0x70);     // CELL SCANEN:Enable CELL1~CELL8
                                         // Cell input test source current:50uA
    command_writeall(0x6,0x0,0x0);       // Disable OV alert
    command_writeall(0x7,0x0,0x0);       // Disable UV alert
    
    command_writeall(0x13,0xf8,0xf);     // CELLTEST Register: MUXDIAGSRC:AGND,CTSTSRC:AGND
                                         // Cell8~Cell0 cell input test switch enables
    
    // Verify the datas written to the registers
    flag_failconfig_afe = 0x0;
    command_readall(0x2);
    if((iic_sum[0] != 0x0) || (iic_sum[1] != 0x0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    #if 0
    command_readall(0x18);
    if((iic_sum[0] != 0x2a) || (iic_sum[1] != 0xc0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x19);
    if((iic_sum[0] != 0x30) || (iic_sum[1] != 0xa4))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x1a);
    if((iic_sum[0] != 0x20) || (iic_sum[1] != 0x0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x1b);
    if((iic_sum[0] != 0x27) || (iic_sum[1] != 0xb0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x1c);
    if((iic_sum[0] != 0x6) || (iic_sum[1] != 0x68))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x1e);
    if((iic_sum[0] != 0xff) || (iic_sum[1] != 0xf0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x1f);
    if((iic_sum[0] != 0x0) || (iic_sum[1] != 0x0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    #endif
    
    command_readall(0xc);
    if((iic_sum[0] != 0x2000) || (iic_sum[1] != 0x2000))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x8);
    if((iic_sum[0] != 0x323) || (iic_sum[1] != 0x323))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x9);
    if((iic_sum[0] != 0x70ff) || (iic_sum[1] != 0x70ff))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x6);
    if((iic_sum[0] != 0x0) || (iic_sum[1] != 0x0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x7);
    if((iic_sum[0] != 0x0) || (iic_sum[1] != 0x0))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
    
    command_readall(0x13);
    if((iic_sum[0] != 0xff8) || (iic_sum[1] != 0xff8))
    {
        flag_failconfig_afe = 0x1;  // Failure config the afe
    }
}
/**************************************************************************************************/
// Start Address:0x02
void command_helloall(void)
{
    iic_start();
    iic_send_byte(0xd0);  // 0xd0
    iic_receive_ack();
    iic_stop();
}
/**************************************************************************************************/                      
void command_rollcall(void)
{
    uchar i;
    //uchar addr[6];
    
    iic_start();
    iic_send_byte(0x40);
    iic_receive_ack();
    iic_send_byte(0x01);  // Address register:0x01
    iic_receive_ack();
    
    iic_start();
    iic_send_byte(0x41);
    iic_receive_ack();
    
    for(i=0;i<6;i++)
    {
        iic_receive_byte();
        addr[i] = (uchar)iic_byte;
        
        if(i==5)
        {
            iic_send_nack();
        }
        else
        {
            iic_send_ack();
        }
        
    }
    
    iic_stop();
    _NOP();    
}
/**************************************************************************************************/
void command_setlastaddress(void)
{
    iic_start();
    iic_send_byte(0x40);  // Broadcast write
    iic_receive_ack();
    iic_send_byte(0x01);  // Register address
    iic_receive_ack();
    iic_send_byte(0x00);  // Data1
    iic_receive_ack();
    iic_send_byte(0x03);  // Data2:0x03
    iic_receive_ack();
    iic_send_byte(0xf9);  // CRC:0xf9(0x03)  0xfe(0x02)
    iic_receive_ack();
    iic_stop();
}
/**************************************************************************************************/
void command_writeall(uchar addr,uchar data1,uchar data2)
{
    uchar data3;
    
    crc[0] = 0x40;
    crc[1] = addr;
    crc[2] = data1;
    crc[3] = data2;
    data3 = (uchar)(crc_verify(0x4));
    
    iic_start();
    iic_send_byte(0x40);  // Broadcast write
    iic_receive_ack();
    iic_send_byte(addr);  // Register address
    iic_receive_ack();
    iic_send_byte(data1);  // Data_L
    iic_receive_ack();
    iic_send_byte(data2);  // Data_H
    iic_receive_ack();
    iic_send_byte(data3);  // CRC
    iic_receive_ack();
    iic_stop();
}
/**************************************************************************************************/
void command_readall(uchar addr)
{
    iic_sum[0] = 0x0;
    iic_sum[1] = 0x0;
    
    iic_start();
    iic_send_byte(0x40);  // Broadcast write
    iic_receive_ack();
    iic_send_byte(addr);  // Register address
    iic_receive_ack();
    
    iic_start();
    iic_send_byte(0x41);  // Broadcast read
    iic_receive_ack();
    
    iic_receive_byte();
    iic_sum[0] = iic_byte;
    iic_send_ack();
    
    iic_receive_byte();
    iic_byte <<= 8;
    iic_sum[0] += iic_byte;
    iic_send_ack();
    
    iic_receive_byte();
    iic_sum[1] = iic_byte;
    iic_send_ack();
    
    iic_receive_byte();
    iic_byte <<= 8;
    iic_sum[1] += iic_byte;
    iic_send_nack();
    
    iic_stop();
}
/**************************************************************************************************/
// WRITEDEVICE Command
void sendmsg_to_afe(uchar device,uchar addr,uchar data1,uchar data2)
{
    uchar data3;
    
    if(device == 0x1)
    {
        crc[0] = 0x90;
    }
    else
    {
        crc[0] = 0xb0;
    }
    crc[1] = addr;
    crc[2] = data1;
    crc[3] = data2;
    data3 = (uchar)(crc_verify(0x4));
    
    iic_start();
    
    if(device == 0x1)
    {
        iic_send_byte(0x90);  // Device address:0x02(start address)
    }
    else  // device = 0x2
    {
        iic_send_byte(0xb0);  // Device address:0x03
    }
    
    iic_receive_ack();
    
    iic_send_byte(addr);  // Register address
    iic_receive_ack();
    iic_send_byte(data1);  // Data_L
    iic_receive_ack();
    iic_send_byte(data2);  // Data_H
    iic_receive_ack();
    iic_send_byte(data3);  // CRC
    iic_receive_ack();
    iic_stop();
}
/**************************************************************************************************/
uint receivemsg_from_afe(uchar device,uchar addr)
{
    iic_word = 0;
    
    iic_start();
    
    if(device == 0x1)
    {
        iic_send_byte(0x90);  // Device address:0x02(start address)
    }
    else
    {
        iic_send_byte(0xb0);  // Device address:0x03
    }
    
    iic_receive_ack();
    
    iic_send_byte(addr);  // Register address
    iic_receive_ack();
    
    iic_start();
    
    if(device == 0x1)
    {
        iic_send_byte(0x91);  // Device address:0x02(start address)
    }
    else
    {
        iic_send_byte(0xb1);  // Device address:0x03
    }
    
    iic_receive_ack();
    
    iic_receive_byte();  // Low byte
    iic_word = iic_byte;
    iic_send_ack();
    
    iic_receive_byte();  // High byte
    iic_byte <<= 8;
    iic_word += iic_byte;
    iic_send_nack();
    iic_stop();
    
    return iic_word;
}
/**************************************************************************************************/
uint crc_verify(uchar count)
{
    uchar i;
    uchar j;
    uint  remainder;
    uint  crcbyte;
    
    crcbyte = 0;
    for(i=0;i<count;i++)
    {
        remainder = crcbyte^crc[i];
        for(j=0;j<8;j++) 
        {
            if((remainder & 0x80) == 0x80) 
            {
                remainder <<= 1;
                remainder ^= 0x07;
                crcbyte = remainder;
            } 
            else 
            {
                remainder <<=1;
                crcbyte = remainder;
            }
            crcbyte &=0xff;
        }
    }
    return crcbyte;
}
/**************************************************************************************************/
