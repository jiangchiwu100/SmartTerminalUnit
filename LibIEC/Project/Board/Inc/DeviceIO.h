#ifndef DEVICEIO_H
#define DEVICEIO_H
#include <stdint.h>

void InitDeviceIO(void);
void SystemClock_Config(void);

 

#define INDICATE_LED_ON()  {LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);}
#define INDICATE_LED_OFF() {LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);}
#define INDICATE_LED_TOGGLE() {LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);}

#define KEY1 0xA1
#define KEY2 0xA2
#define KEY3 0xA3
#define KEY4 0xA4
#define KEY5 0xA5
#define KEY6 0xA6
#define KEY7 0xA7
#define KEY8 0xA8


//#define LED1 0xA1
//#define LED2 0xA2
//#define LED3 0xA3
//#define LED4 0xA4
//#define LED5 0xA5
//#define LED6 0xA6
//#define LED7 0xA7
//#define LED8 0xA8
//#define LED9 0xA9
//#define LED10 0xAA
//#define LED11 0xAB


#define LED_ON  0x51
#define LED_OFF 0x52
#define LED_TOGGLE 0x53





//基本模块
extern void delay_ms(uint32_t ms); 
extern uint32_t GetKeyValue(uint8_t key);
extern void SetLedState(uint8_t led, uint8_t state);
extern void     SystemClock_Config(void);
extern void InitDeviceIO(void);

#endif
