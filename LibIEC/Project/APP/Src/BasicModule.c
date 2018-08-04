#include "BasicModule.h"
#include "stm32f4xx_ll_utils.h"

void delay_ms_soft(unsigned int cn)
{
    for ( ; cn-- > 0; )
        for ( int n = 13850; n-- > 0; );

}

void delay_100us_soft(unsigned int cn)
{
    for ( ; cn-- > 0; )
        for ( int n = 1385; n-- > 0; );

}
/**
  * @brief : 初始化IO
  * @param : None.
  * @return: None.
  */
//void delay_ms(uint32_t ms)
//{
//    LL_mDelay(ms);
//}
