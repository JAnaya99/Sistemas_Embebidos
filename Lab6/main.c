#include "stdio.h"
#include "myprintf.h"
#include "sam.h"
#include "stdbool.h"
#include "RTCControl.h"

#define SLAVE_ADDR 0x68u
#define BUF_SIZE 8

// Control variables for data transfer and reception
volatile uint32_t i;
uint8_t tx_buf[BUF_SIZE] = {SECONDS_ADDR, 0x00u, 0x59, 0x41, 0x04, 0x14, 0x04, 0x21};
uint8_t rx_buf[BUF_SIZE];


void UARTInit(void);

void printTimeStamp(){
	struct RTCType aux;
	aux.seconds_type.reg = rx_buf[SECONDS_ADDR];
	aux.minutes_type.reg = rx_buf[MINUTES_ADDR];
	aux.hours_type.reg	= rx_buf[HOURS_ADDR];
	aux.day_type.reg = rx_buf[DAY_ADDR];
	aux.date_type.reg = rx_buf[DATE_ADDR];
	aux.month_type.reg = rx_buf[MONTH_ADDR];
	aux.year_type.reg = rx_buf[YEAR_ADDR];
	
	char hour_format[3] = "AM";
	if (aux.hours_type.field.is_pm == 1){
		hour_format[0] = 'P';
		hour_format[1] = 'M';
	}
	
	myprintf("%d%d/", aux.date_type.field.dec_date, aux.date_type.field.un_date);
	myprintf("%d%d/", aux.month_type.field.dec_month, aux.month_type.field.un_month);
	myprintf("%d%d ", aux.year_type.field.dec_year, aux.year_type.field.un_year);
	
	myprintf("%d%d:", aux.hours_type.field.dec_hours, aux.hours_type.field.un_hours);
	myprintf("%d%d:", aux.minutes_type.field.dec_minutes, aux.minutes_type.field.un_minutes);
	myprintf("%d%d ", aux.seconds_type.field.dec_seconds, aux.seconds_type.field.un_seconds);
	myprintf("%s \n", hour_format);
}

int main(void)
{	
	I2CInit();
	
	sendI2CDataArray(SLAVE_ADDR, tx_buf, BUF_SIZE);
	receiveI2CDataArray(SLAVE_ADDR, rx_buf, BUF_SIZE);
	
	UARTInit();

	printTimeStamp();
	

	while(true){
		I2CInit();
		receiveI2CDataArray(SLAVE_ADDR, rx_buf, BUF_SIZE);

		printTimeStamp();

	}
	
	
	return 0;
}

void UARTInit(void) {
	/* Initialize the SAM system */
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	
	/* port mux configuration*/
	PORT->Group[0].DIR.reg |= (1 << 10);                  /* Pin 10 configured as output */
	PORT->Group[0].PINCFG[PIN_PA11].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	PORT->Group[0].PINCFG[PIN_PA10].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	
	/*PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[5].reg |= 0x02;                   /* Selecting peripheral function C */
	PORT->Group[0].PMUX[5].reg |= 0x20;                   /* Selecting peripheral function C */
	
	/* APBCMASK */
	//PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;			  /* SERCOM 0 enable*/
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

	/*GCLK configuration for sercom0 module: using generic clock generator 0, ID for sercom0, enable GCLK*/

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) |
	GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

	
	/* configure SERCOM0 module for UART as Standard Frame, 8 Bit size, No parity, BAUDRATE:9600*/

	SERCOM0->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);
	
	uint64_t br = (uint64_t)65536 * (8000000 - 16 * 9600) / 8000000;
	
	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	SERCOM0->USART.BAUD.reg = (uint16_t)br;

	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}
