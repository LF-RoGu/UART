/*
 * UART.c
 *
 *  Created on: Oct 22, 2018
 *      Author: LuisFernando
 */

#include "UART.h"

UART_MailBoxType UART0_MailBox;
UART_MailBoxType UART1_MailBox;
UART_MailBoxType UART2_MailBox;
UART_MailBoxType UART3_MailBox;
UART_MailBoxType UART4_MailBox;
UART_MailBoxType UART5_MailBox;

void UART0_Status_IRQHandler(void)
{
	/*First is verified if the serial port finished to transmit*/
	while(!(UART0->S1 & UART_S1_RDRF_MASK));
	/*The info is saved in Data Register*/
	UART0_MailBox.mailBox = UART0->D;
	/*There are new data*/
	UART0_MailBox.flag = TRUE;
}

void UART_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate)
{
	uint16 UARTBaudRate;
	uint8 temp;
	switch(uartChannel)
	{
	case UART_0:
		SIM->SCGC4 = SIM_SCGC4_UART0_MASK;
		UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		UARTBaudRate = (uint16)(systemClk/(16*baudRate));
		temp = UART0->BDH & ~(UART_BDH_SBR(0x1F));
		UART0->BDH = temp | (((UARTBaudRate & UART_BDH_SBR_MASK) >> 8));
		UART0->BDL = (uint8_t) (UARTBaudRate & UART_BDL_SBR_MASK);
		UART0->C4 = UART_C4_BRFA_MASK;

		UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
		break;
	case UART_1:
		SIM->SCGC4 = SIM_SCGC4_UART1_MASK;
		UART1->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		UARTBaudRate = (uint16)(systemClk/(16*baudRate));
		break;
	case UART_2:
		SIM->SCGC4 = SIM_SCGC4_UART2_MASK;
		UART2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		UARTBaudRate = (uint16)(systemClk/(16*baudRate));
		break;
	case UART_3:
		SIM->SCGC4 = SIM_SCGC4_UART3_MASK;
		UART3->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		UARTBaudRate = (uint16)(systemClk/(16*baudRate));
		break;
	case UART_4:
		SIM->SCGC1 = SIM_SCGC1_UART4_MASK;
		UART4->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		UARTBaudRate = (uint16)(systemClk/(16*baudRate));
		break;
	case UART_5:
		SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
		UART5->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		UARTBaudRate = (uint16)(systemClk/(16*baudRate));
		break;
	}
}

void UART0_interruptEnable(UART_ChannelType uartChannel)
{
	NVIC_setBASEPRI_threshold(PRIORITY_11);
	switch(uartChannel)
	{
	case UART_0:
		NVIC_enableInterruptAndPriotity(UART0_IRQ,PRIORITY_2);
		break;
	case UART_1:
			NVIC_enableInterruptAndPriotity(UART1_IRQ,PRIORITY_2);
			break;
	case UART_2:
			NVIC_enableInterruptAndPriotity(UART2_IRQ,PRIORITY_2);
			break;
	case UART_3:
			NVIC_enableInterruptAndPriotity(UART3_IRQ,PRIORITY_2);
			break;
	case UART_4:
			NVIC_enableInterruptAndPriotity(UART4_IRQ,PRIORITY_2);
			break;
	case UART_5:
			NVIC_enableInterruptAndPriotity(UART5_IRQ,PRIORITY_2);
			break;

	}
}

void UART_putChar (UART_ChannelType uartChannel, uint8 character)
{
	switch (uartChannel)
	{
	case UART_0:
		/*Check if there isn't any data transmition*/
		while(!(UART0->S1 && UART_S1_TDRE_MASK));
		/*Send char to the Data Reg*/
		UART0->D = character;
		break;
	case UART_1:
		/*Check if there isn't any data transmition*/
		while(!(UART1->S1 && UART_S1_TDRE_MASK));
		UART1->D = character;
		break;
	case UART_2:
		/*Check if there isn't any data transmition*/
		while(!(UART2->S1 && UART_S1_TDRE_MASK));
		UART2->D = character;
		break;
	case UART_3:
		/*Check if there isn't any data transmition*/
		while(!(UART3->S1 && UART_S1_TDRE_MASK));
		UART3->D = character;
		break;
	case UART_4:
		/*Check if there isn't any data transmition*/
		while(!(UART4->S1 && UART_S1_TDRE_MASK));
		UART4->D = character;
		break;
	case UART_5:
		/*Check if there isn't any data transmition*/
		while (!(UART5->S1 && UART_S1_TDRE_MASK));
		UART5->D = character;
		break;

	}
}
void UART_putString(UART_ChannelType uartChannel, sint8* string)
{
	uint8 count;
	switch(uartChannel)
	{
		case UART_0:
			/*Check if there isn't any data transmition*/
			while(!(UART0->S1 && UART_S1_TDRE_MASK));
			/*Transmit data*/
			/*Preguntar si se puede funcion null*/
			while(string[count] != '\0')
			{
				UART0->D = string[count];
				count++;
			}
			break;
		case UART_1:
			/*Check if there isn't any data transmition*/
			while(!(UART1->S1 && UART_S1_TDRE_MASK));
			/*Transmit data*/
			while(string[count] != '\0')
			{
				UART1->D = string[count];
				count++;
			}
			break;
		case UART_2:
			/*Check if there isn't any data transmition*/
			while(!(UART2->S1 && UART_S1_TDRE_MASK));
			/*Transmit data*/
			while(string[count] != '\0')
			{
				UART2->D = string[count];
				count++;
			}
			break;
		case UART_3:
			/*Check if there isn't any data transmition*/
			while(!(UART3->S1 && UART_S1_TDRE_MASK));
			/*Transmit data*/
			while(string[count] != '\0')
			{
				UART3->D = string[count];
				count++;
			}
			break;
		case UART_4:
			/*Check if there isn't any data transmition*/
			while(!(UART4->S1 && UART_S1_TDRE_MASK));
			/*Transmit data*/
			while(string[count] != '\0')
			{
				UART4->D = string[count];
				count++;
			}
			break;
		case UART_5:
			/*Check if there isn't any data transmition*/
			while(!(UART5->S1 && UART_S1_TDRE_MASK));
			/*Transmit data*/
			while(string[count] != '\0')
			{
				UART5->D = string[count];
				count++;
			}
			break;
		default:
			break;

	}
}
