#include "main.h"

uint8_t bufferRX[ReadBufferSize];
uint8_t bufferTX[WriteBufferSize];

void main()
{
	asm("CPSID i");
	Sys_Init();
	USARTx_Init(USART1, BT52_defaultBaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	asm("CPSIE i");
	for (int i = 0; i < 25; i++) _delay_ms(200);
	if (BT52_masterConfig(true) == ERROR) while (1);
	while (1)
	{
		USART_Receive(USART1, bufferRX, ReadBufferSize);
	}
}


/**
*	@section User functions
*/
ErrorStatus BT52_masterConfig(bool performRestore)
{
	/* Enter to AT mode */
	USART_Send(USART1, "+++", sizeof("+++")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* Check if the module is available */
	uint8_t bufferResponse[20];
	memset(bufferResponse, NULL, sizeof(bufferResponse));
	USART_Send(USART1, "AT", sizeof("AT")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferResponse, sizeof(bufferResponse));
	if (strcmp((const char*)bufferResponse, "+OK\r\n") != 0) return ERROR;

	/* Factory reset */
	if (performRestore)
	{
		USART_Send(USART1, "AT+RESTORE", sizeof("AT+RESTORE")-1);
		_delay_ms(500);
	}

	/* Status printing enable */
	USART_Send(USART1, "AT+LOGMSG=1", sizeof("AT+LOGMSG=1")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* Select master mode */
	USART_Send(USART1, "AT+ROLE=1", sizeof("AT+ROLE=1")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* MAC address filtering */
	uint8_t MACcmd[11+6] = "AT+BONDMAC=";
	for (int i = 0; i < sizeof(SlaveMAC); i++) MACcmd[11+i] = SlaveMAC[i];
	USART_Send(USART1, MACcmd, sizeof(MACcmd));
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* Set the module to transparent transmission mode */
	USART_Send(USART1, "AT+TRANMD=1", sizeof("AT+TRANMD=1")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* Start master scanning function */
	USART_Send(USART1, "AT+SCAN=1", sizeof("AT+SCAN=1")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* Restart to take effect */
	USART_Send(USART1, "AT+RESET", sizeof("AT+RESET")-1);
	_delay_ms(100);
	USART_Receive(USART1, bufferRX, ReadBufferSize);

	/* Wait until it is connected */
	do
	{
		USART_Receive(USART1, bufferRX, ReadBufferSize);
	}
	while (strncmp((const char*)bufferRX, "\r\nSTA:connect", sizeof("\r\nSTA:connect")-1) != 0);

	_delay_ms(500);
	USART_Send(USART1, "Connection success from master\r\n", sizeof("Connection success from master\r\n")-1);

	return SUCCESS;
}