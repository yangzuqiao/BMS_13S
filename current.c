/**************************************************************************************************/
#include"current.h"
/**************************************************************************************************/
void current_check(void)
{
    current_sample();
    
    if((current >= 10) && (current < 32768))  // Charging
    {
        state_charge = 0x1;
        state_discharge = 0x0;
        state_quiet = 0x0;
        t_doc_1 = 0x0;
        t_doc_2 = 0x0;
        t_quiet = 0x0;
        
        if(current > COC)
        {
            t_coc++;
            if(t_coc >= T_COC)
            {
                t_coc = 0x0;
                flag_coc = 0x1;
            }
        }
        else
        {
            t_coc = 0x0;
        }
    }
    else if((current > 32768) && (current <= 65526))  // Discharging
    {
        state_discharge = 0x1;
        state_charge = 0x0;
        state_quiet = 0x0;
        t_coc = 0x0;
        t_quiet = 0x0;
        
        if(current < DOC_1)
        {
            t_doc_1++;
            if(t_doc_1 >= T_DOC_1)
            {
                t_doc_1 = 0x0;
                flag_doc_1 = 0x1;
                
                LOADCHECK_ENABLE;
            }
        }
        else
        {
            t_doc_1 = 0x0;
        }
        
        if(current < DOC_2)
        {
            t_doc_2++;
            if(t_doc_2 >= T_DOC_2)
            {
                t_doc_2 = 0x0;
                flag_doc_2 = 0x1;
                
                LOADCHECK_ENABLE;
            }
        }
        else
        {
            t_doc_2 = 0x0;
        }
    }
    else  // Quiet
    {
        state_quiet = 0x1;
        state_charge = 0x0;
        state_discharge = 0x0;
        t_coc = 0x0;
        t_doc_1 = 0x0;
        t_doc_2 = 0x0;
        
        if(flag_coc)
        {
            if(flag_closed_cmos)  // CMOS closed
            {
                t_quiet++;
                if(t_quiet >= 300)
                {
                    t_quiet = 0x0;
                    flag_coc = 0x0;
                }
            }
        }
        
        if(flag_doc_1 || flag_doc_2)
        {
            if(flag_closed_dmos)  // DMOS closed
            {
                if(P6IN & BIT1)  // LOAD removed
                {
                    flag_doc_1 = 0x0;
                    flag_doc_2 = 0x0;
                    
                    LOADCHECK_DISABLE;
                }
            }
                
        }
    }
}
/**************************************************************************************************/
void current_sample(void)
{
    uint i;
    
    i = 0x0;
    do
    {
        receivemsg_from_ina230(0x6);
        i++;
        if(i > 0x1f4)  // 500
        {
            i = 0x0;
            flag_ot_230 = 0x1;
            break;
        }
    }
    while(!(iic_word & BIT3));
    
    receivemsg_from_ina230(0x4);
    current = iic_word;
}
/**************************************************************************************************/



