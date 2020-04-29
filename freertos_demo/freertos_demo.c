//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"

#include "utils/uartstdio.h"
#include "led_task.h"
#include "switch_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "app.h"
#include "uart.h"


xSemaphoreHandle g_pUARTSemaphore;

//extern TaskHandle_t thFib10;
//extern TaskHandle_t thFib40;
//extern TaskHandle_t thSequencer;
//extern TaskHandle_t thGetFrame;
extern TaskHandle_t thGetAndTrans;
extern TaskHandle_t thFlashMorse;

extern QueueHandle_t mqTranslatedChars;

//extern SemaphoreHandle_t TimerSmphr;
//extern SemaphoreHandle_t SequencerSmphr;
//extern SemaphoreHandle_t ServiceSmphrs[NUM_SERVICES];


//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main(void)
{
    //
    // Set the clocking to run at 50 MHz from the PLL.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);

    //
    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    //
    ConfigureUART();
    UARTprintf("\n UART FUNCTIONAL !\n");

    int i;

    mqTranslatedChars = xQueueCreate(MAX_INPUT_LENGTH, sizeof(MorseTranslation_t));

    BaseType_t GetStringReturned, WriteLEDReturned;

    WriteLEDReturned = xTaskCreate(FlashMorseTask, "FLash LED", 2000, (void *) NULL, (configMAX_PRIORITIES - 1), &thFlashMorse);
    GetStringReturned = xTaskCreate(GetAndTranslateTask, "Get and decode task", 2000, (void *) NULL, (configMAX_PRIORITIES - 2), &thGetAndTrans);


    vTaskStartScheduler();

}







#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


