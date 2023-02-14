#include "stm32f10x.h"
#include "Sys_Init.h"
#include "Delay.h"
#include "USART.h"
#include <string.h>
#include <stdio.h>

#define BT52_defaultBaudRate	115200
const uint8_t SlaveMAC[6] = {0x32, 0x00, 0x00, 0xB1, 0x9A, 0x01};

ErrorStatus BT52_masterConfig(bool performRestore);