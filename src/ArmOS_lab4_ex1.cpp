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
#define LPC_IRQNUM      UART0_IRQn

#define SCT_PWM            LPC_SCT0 /* Use SCT0 for PWM */
#define SCT_PWM_PIN_LED    0        /* COUT0 [index 2] Controls LED */
#define SCT_PWM_LED        2        /* Index of LED PWM */
#define SCT_PWM_RATE	   1000        /* PWM frequency 1 KHz */

#define LED_STEP_CNT      20        /* Change LED duty cycle every 20ms */

struct debugEvent {
	char *format;
	uint32_t data[3];
};

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

void debug(char* format, uint32_t d1, uint32_t  d2, uint32_t d3, QueueHandle_t queue) {
	debugEvent e = {format, d1, d2, d3};
	xQueueSend(queue, &e, portMAX_DELAY);
}

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
	while(1) {
		if(lim1.read() == false) {
			red.write(false);
			xSemaphoreGive(lim_sem);
		}
		else red.write(true);
		if(lim2.read() == false) {
			green.write(false);
			xSemaphoreGive(lim_sem);
		}
		else green.write(true);
		vTaskDelay(configTICK_RATE_HZ/10);
	}
}


static void vTask2(void *pvParameters) {
	DigitalIoPin DIR(1, 0, DigitalIoPin::output, false);
	DigitalIoPin blue(1, 1, DigitalIoPin::output, false);
	DigitalIoPin sw1(0, 17, DigitalIoPin::pullup, false);
	DigitalIoPin sw3(1, 9, DigitalIoPin::pullup, false);
	/* Initialize the SCT as PWM and set frequency */
	Chip_SCTPWM_Init(SCT_PWM);
	Chip_SCTPWM_SetRate(SCT_PWM, SCT_PWM_RATE);

	/* Enable SWM clock before altering SWM */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
	Chip_SWM_MovablePinAssign(SWM_SCT0_OUT0_O, 24);
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);

	Chip_SCTPWM_SetOutPin(SCT_PWM, SCT_PWM_LED, SCT_PWM_PIN_LED);
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, 0);
	Chip_SCTPWM_Start(SCT_PWM);

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);
	bool isatlim = false;
	while(1) {
		if(xSemaphoreTake(lim_sem, 0) == pdTRUE)
			isatlim = true;
		else isatlim = false;
		if((sw1.read() == false) && (sw3.read() != false) && (isatlim == false)) {
			DIR.write(false);
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, Chip_SCTPWM_PercentageToTicks(SCT_PWM, 50));
		}
		else Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, 0);

		if((sw3.read() == false) && (sw1.read() != false) && (isatlim == false)) {
			DIR.write(true);
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, Chip_SCTPWM_PercentageToTicks(SCT_PWM, 50));
		}
		else Chip_SCTPWM_SetDutyCycle(SCT_PWM, SCT_PWM_LED, 0);

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
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2UL),
			(TaskHandle_t *) NULL);

	xTaskCreate(vTask2, "stepper",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(TaskHandle_t *) NULL);
	vTaskStartScheduler();
	return 1;
}


