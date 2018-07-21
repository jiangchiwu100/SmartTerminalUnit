#ifndef SERIALPORT_H
#define SERIALPORT_H
#include "buffer.h"
#include "ll_driver.h"


#define UART_6_BUFFER_LEN 128
#define UART_5_BUFFER_LEN 512
#define UART_4_BUFFER_LEN 255
#define UART_3_BUFFER_LEN 128
#define UART_1_BUFFER_LEN 128


extern FifoHandle Uart6FifoHandle;
extern FifoHandle Uart5FifoHandle;
extern FifoHandle Uart4FifoHandle;
extern FifoHandle Uart3FifoHandle;
extern FifoHandle Uart1FifoHandle;

extern uint8_t UartSend(USART_TypeDef *USARTx, uint8_t* pData, uint8_t len);
extern void SerialPortInit(void);
extern void SerialPort4Init(void);
extern void SerialPort6Init(void);
extern void SerialPort5Init(void);
#endif

