#include "sam.h"
#include "uart.h"
#include "spi.h"
#include "myprintf.h"
 
#define RXBUFSIZE 0x400
#define LENGTH_R1 0x03
#define LENGTH_R7 0x07
 
void initCycles(void);
uint32_t spiXchg(const uint8_t *send_buff, uint32_t bc, uint8_t *receive_buff);
void initSD();
void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs );
 
#define SIZE_SD_CMD 0x06
#define kCMD00 0x40
#define kCMD08 0x48
#define kCMD55 0x77
#define kCMD41 0x69
 
const uint8_t CMD00[SIZE_SD_CMD] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
const uint8_t CMD08[SIZE_SD_CMD] = {0x48, 0x00, 0x00, 0x01, 0xAA, 0x87};
const uint8_t CMD17[SIZE_SD_CMD] = {0x51, 0x00, 0x00, 0x00, 0x00, 0x01};
const uint8_t CMD18[SIZE_SD_CMD] = {0x52, 0x00, 0x00, 0x00, 0x00, 0x01};
const uint8_t CMD55[SIZE_SD_CMD] = {0x77, 0x00, 0x00, 0x00, 0x00, 0x65};
const uint8_t CMD41[SIZE_SD_CMD] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x77};
 
uint8_t RxBuffer[RXBUFSIZE];
 
void testPrint()
{
    char buff[31];
    buff[30] = '\0';
    mysnprintf(buff, sizeof buff, "testing %d %d %07d", 1, 2, 3);
    myprintf("\nBuff=%s", buff);
    mysnprintf(buff, sizeof buff, "faster %s %ccheaper%c", "and", 34, 34);
    myprintf("\nBuff=%s", buff);
    mysnprintf(buff, sizeof buff, "%x %% %X", 0xdeadf00d, 0xdeadf00d);
    myprintf("\nBuff=%s", buff);
    mysnprintf(buff, sizeof buff, "%09d%09d%09d%09d%09d", 1, 2, 3, 4, 5);
    myprintf("\nBuff=%s", buff);
    mysnprintf(buff, sizeof buff, "%d %u %d %u", 50, 50, -50, -50);
    myprintf("\nBuff=%s\n\n", buff);
}
 
int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
 
	uartInit();
	spiInit();
	initCycles();
	initSD();
	myprintf("Datos\n");
	rcvr_datablock(CMD17, 0x00, RxBuffer, 512);
}
 
void initCycles(void)
{
	uint32_t i;
	REG_PORT_OUTSET0 = PORT_PA18;
	for (i = 0; i < 76; i++)
	spiSend(0xFF);
}
 
void initSD()
{
	/* reset instruction */
	do 
	{
		spiXchg(CMD00, SIZE_SD_CMD, RxBuffer);	
	} while (RxBuffer[1] != 1);
 
	/* request the contents of the operating conditions register for the connected card */
	spiXchg(CMD08, SIZE_SD_CMD, RxBuffer);
	if (RxBuffer[1] != 1 && RxBuffer[4] != 1 && RxBuffer[5] != 170)
	{
		myprintf("Error: Invalid response from SD card");
		return;
	} 
	
	/* determine if the SD card is ready for exchange of data */
	do 
	{
		spiXchg(CMD55, SIZE_SD_CMD, RxBuffer);
		spiXchg(CMD41, SIZE_SD_CMD, RxBuffer);
	} while (RxBuffer[1] != 0);
	
	myprintf("\nSD Card Ready!\n");
}
 
//ADD spiXchg FUNCTION TO THE “spi.c” FILE
 
uint32_t spiXchg(const uint8_t *send_buff, uint32_t bc, uint8_t *receive_buff)
{
	uint8_t temp = 0xFF;
	uint32_t i;
	uint8_t temp_cmd = send_buff[0];
 
	REG_PORT_OUTCLR0 = PORT_PA18;
	myprintf("\nData Sent: ");
	for (i = 0; i < bc; i++)
	{
		uint8_t temp_snt = *(send_buff++);
		temp = spiSend(temp_snt);
		myprintf(" %x", temp_snt);
	}
	myprintf("\nData Received: ");
	switch (temp_cmd)
	{
		case kCMD00:
		
		for (i = 0; i < LENGTH_R1; i++)
		{
			temp = spiSend(0xFF);
			*(receive_buff++) = temp;
			myprintf(" %x", temp);
		}
		break;
		case kCMD08:
		for (i = 0; i < LENGTH_R7; i++)
		{
			temp = spiSend(0xFF);
			*(receive_buff++) = temp;
			myprintf(" %x", temp);
		}
		break;
		case kCMD41:
		for (i = 0; i < LENGTH_R1; i++)
		{
			temp = spiSend(0xFF);
			*(receive_buff++) = temp;
			myprintf(" %x", temp);
		}
		break;
		case kCMD55:
		for (i = 0; i < LENGTH_R1; i++)
		{
			temp = spiSend(0xFF);
			// myprintf(" %x", temp);
		}
		break;
		default:
		myprintf("\n Error in CMD");
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	return (temp);
}

void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs ) {
	uint8_t temp = 0xFF;
	uint32_t i;
	
	REG_PORT_OUTCLR0 = PORT_PA18;
	myprintf("\n\n");

	temp = send_buff[0];
	temp = spiSend(temp);
	myprintf("BUFF[0] %x \n", temp);
	
	temp = send_buff[1] | (lba >> 24);
	temp = spiSend(temp);
	myprintf("BUFF[1] %x \n", temp);
	
	temp = send_buff[2] | ((lba >> 16 ) & 0xFF);
	temp = spiSend(temp);
	myprintf("BUFF[2] %x \n", temp);
	
	temp = send_buff[3] | ((lba >> 8 ) & 0xFF);
	temp = spiSend(temp);
	myprintf("BUFF[3] %x \n", temp);
	
	temp = send_buff[4] | (lba & 0xFF);
	temp = spiSend(temp);
	myprintf("BUFF[4] %x \n", temp);

	temp = send_buff[5];
	temp = spiSend(temp);
	myprintf("BUFF[5] %x \n", temp);

	// Reading to find the beginning of the sector

	temp = spiSend(0xFF);
	while(temp != 0xFE){
		temp = spiSend(0xFF);
		myprintf(" %x", temp);
	}
	
	// Receiving the memory sector/block
	
	myprintf("\n\n");
	for(i=0; i< bs; i++) {
		while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
		SERCOM1->SPI.DATA.reg = 0xFF;
		while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
		while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
		temp = SERCOM1->SPI.DATA.reg;
		*(receive_buff++) = temp;
		myprintf(" %x", temp);
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	myprintf("\n\n");
}