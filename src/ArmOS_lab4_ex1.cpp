/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "FreeRTOS.h"
#include "task.h"
#include "DigitalIoPin.h"
#include "semphr.h"
#include <mutex>
#include <stdio.h>
#include "queue.h"
#include "itm.h"

#define myTICK_RATE_HZ (configTICK_RATE_HZ)

class Fmutex  {
public:
	Fmutex() {
		mutex = xSemaphoreCreateMutex();
	}
	virtual ~Fmutex() {
		vSemaphoreDelete(mutex);
	}
	void lock() {
		xSemaphoreTake(mutex, portMAX_DELAY);
	}
	void unlock() {
		xSemaphoreGive(mutex);
	}
private:
	xSemaphoreHandle mutex;
};

xSemaphoreHandle lim_sem = xSemaphoreCreateBinary();

static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
}

static void vTask1(void *pvParameters) {
	DigitalIoPin red(0, 25, DigitalIoPin::output, false);
	DigitalIoPin green(0, 3, DigitalIoPin::output, false);
	DigitalIoPin lim1(0, 27, DigitalIoPin::pullup, false);
	DigitalIoPin lim2(0, 28, DigitalIoPin::pullup, false);
	green.write(true);
	while(1) {

		if(lim1.read() == false) {
			if(xSemaphoreGive(lim_sem) == pdTRUE)
				red.write(false);
		}
		else red.write(true);

		if(lim2.read() == false) {
			if(xSemaphoreGive(lim_sem) == pdTRUE)
				green.write(false);
		}
		else green.write(true);

		vTaskDelay(configTICK_RATE_HZ/10);
	}
}


static void vTask2(void *pvParameters) {
	DigitalIoPin DIR(1, 0, DigitalIoPin::output, false);
	DigitalIoPin sw1(0, 17, DigitalIoPin::pullup, false);
	DigitalIoPin sw3(1, 9, DigitalIoPin::pullup, false);
	DigitalIoPin STEP(0, 24, DigitalIoPin::output, false);
	DigitalIoPin blue(1, 1, DigitalIoPin::output, false);
	while(1) {
		if(xSemaphoreTake(lim_sem, 100) != pdTRUE) {

			if((sw1.read() == false) && (sw3.read() != false)) {
				DIR.write(false);
				STEP.write(true);
				vTaskDelay(configTICK_RATE_HZ/2000);
				STEP.write(false);
				vTaskDelay(configTICK_RATE_HZ/2000);
			}
			else STEP.write(false);

			if((sw3.read() == false) && (sw1.read() != false)) {
				DIR.write(true);
				STEP.write(true);
				vTaskDelay(configTICK_RATE_HZ/2000);
				STEP.write(false);
				vTaskDelay(configTICK_RATE_HZ/2000);
			}
			else STEP.write(false);
			blue.write(true);
		}
		else blue.write(false);
	}

}


/* the following is required if runtime statistics are to be collected */
extern "C" {

void vConfigureTimerForRunTimeStats( void ) {
	Chip_SCT_Init(LPC_SCTSMALL1);
	LPC_SCTSMALL1->CONFIG = SCT_CONFIG_32BIT_COUNTER;
	LPC_SCTSMALL1->CTRL_U = SCT_CTRL_PRE_L(255) | SCT_CTRL_CLRCTR_L; // set prescaler to 256 (255 + 1), and start timer
}
}


int main(void)
{
	prvSetupHardware();
	xTaskCreate(vTask1, "limit sw",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(vTask2, "stepper",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);
	vTaskStartScheduler();
	return 1;
}


