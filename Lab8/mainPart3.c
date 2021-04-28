#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "myprintf.h"

/* Priorities at which the tasks are created. */
#define myTASK_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

xQueueHandle Global_Queue_Handle = 0;

void sender_Task(void *p) {
	while(1){
		const uint64_t arrPins[4] = {PIN_PA14, PIN_PA09, PIN_PA08, PIN_PA15};
		const uint64_t arrPorts[4] = {PORT_PA14, PORT_PA09, PORT_PA08, PORT_PA15};
		for (int i = 0; i < 4; ++i) {
			// Read push buttons.
			PORT->Group[0].PINCFG[arrPins[i]].reg = 0x2;
			PORT->Group[0].DIRCLR.reg = arrPorts[i];
			// Send a number in case of a push.
			if ((PORT->Group[0].IN.reg & PORT_IN_IN(arrPorts[i])) == _U_(0x00000000)) {
				continue;
			}

			if (!xQueueSend(Global_Queue_Handle, &i, 1000)) {
				myprintf("Failed to send to queue %d \n", i);
			}

			int ack = 0;
			if (!xQueueReceive(Global_Queue_Handle, &ack, 1000)){
				myprintf("Failed to receive ack\n");
			}

			vTaskDelay(20);
		}
	}
}

void receiver_Task(void *p) {
	int rx_int = 0;
	while (1) {
		if (!xQueueReceive(Global_Queue_Handle, &rx_int, 1000)){
			continue;
            vTaskDelay(20);
		}

		if (rx_int == 0){
			myprintf("Up \n");
        } else if (rx_int == 1) {
            myprintf("Down \n");
        } else if (rx_int == 2) {
            myprintf("Left \n");
        } else {
            myprintf("Right \n");
		}

		int ack = 0;
		if (!xQueueSend(Global_Queue_Handle, &ack, 1000)) {
			myprintf("Failed to send ack from receiver\n");
		}

		vTaskDelay(20);
	}
}

int main() {
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	
	SYSCTRL->OSC8M.bit.PRESC = 0;
	initUART();
	
	// Create the queue
	Global_Queue_Handle = xQueueCreate(5, sizeof(int));
	xTaskCreate(sender_Task, (const char * const) "tx", 512, NULL, myTASK_TASK_PRIORITY, NULL);
	xTaskCreate(receiver_Task, (const char * const) "rx", 512, NULL, myTASK_TASK_PRIORITY, NULL);
	
	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	for( ;; );
	return(0);
}