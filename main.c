/*
 * Copyright 2016-2018 NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    UART_test_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

uint8_t uart_getchar ();
void put(char *ptr_str);
void uart_putchar (char ch);

int main(void) {
	uint16_t ubd;					/*Variable to save the baud rate*/
	uint8_t temp;

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; /*Enable the UART clock*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; /*Enable the PORTB clock*/
	PORTB->PCR[16] |= PORT_PCR_MUX(3);
	PORTB->PCR[17] |= PORT_PCR_MUX(3);

	UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); /*Disable Tx and Rx*/
	UART0->C1 = 0; /*Dafault settings of the register*/

	ubd = (uint16_t) ((21000 * 1000) / (9600 * 16)); /* Calculate baud settings */

	temp = UART0->BDH & ~(UART_BDH_SBR(0x1F)); /*Save the value of UART0_BDH except SBR*/
	UART0->BDH = temp | (((ubd & 0x1F00) >> 8));
	UART0->BDL = (uint8_t) (ubd & UART_BDL_SBR_MASK);

	UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );    /* Enable receiver and transmitter */

	uint8_t ch;
	put("\033[0;32;46m");
	put("\033[2J");
	put("\033[0;32;41m");
	put("\033[10;10H");
	put("\r\nMicros y DSPs\r\n");
	put("\033[11;10H");
	put("\r\nITESO\n");
	put("\033[12;10H");

	while (1) {
		ch = uart_getchar();
		uart_putchar(ch);
		//BLUE_TOGGLE;
	}
	return 0;
}

uint8_t uart_getchar ()
{
/* Wait until character has been received */
while (!(UART0->S1 & UART_S1_RDRF_MASK));
/* Return the 8-bit data from the receiver */
return UART0->D;
}

void uart_putchar (char ch)
{
/* Wait until space is available in the FIFO */
while(!(UART0->S1 & UART_S1_TDRE_MASK));
/* Send the character */
UART0->D = (uint8_t)ch;
}

void put(char *ptr_str)
{
	while(*ptr_str)
		uart_putchar(*ptr_str++);
}
