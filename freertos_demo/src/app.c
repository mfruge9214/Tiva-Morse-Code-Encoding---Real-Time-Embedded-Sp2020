/*************************
 * File: app.c
 *      This file implements all tasks required in Real Time Embedded Systems, HW3
 *
 * Author: Mike Fruge
 */

#include <stdio.h>
#include <stdbool.h>

#include "app.h"
#include "utils/uartstdio.h"

// From example
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#include "task.h"
#include "queue.h"




uint32_t unitDuration;

void init_led_gpio(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // enable clock for Port F

    // set PF3 as output
    GPIOPinTypeGPIOOutput(RED_GPIO_BASE, RED_GPIO_PIN);
    GPIOPinTypeGPIOOutput(BLUE_GPIO_BASE, BLUE_GPIO_PIN);
    GPIOPinTypeGPIOOutput(GREEN_GPIO_BASE, GREEN_GPIO_PIN);
    GPIOPinWrite(RED_GPIO_BASE, RED_GPIO_PIN, 0x0);
    GPIOPinWrite(BLUE_GPIO_BASE, BLUE_GPIO_PIN, 0x0);
    GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, GREEN_GPIO_PIN);
//    vTaskDelay(pdMS_TO_TICKS(100));
//    GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, 0x0);

}


//*****************************************************************************
//
// The interrupt handler for the Timer interrupt.
//
//*****************************************************************************
void myTimerIntHandler(void)
{
    static int count = 0;
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //
    // Clear the timer interrupt flag.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    count ++;
    if(count == unitDuration)
    {

        //
        // Turn off TImer interrupt.
        //
        TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        //
        // Clear any pending interrupt flag.
        //
        TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

        IntDisable(INT_TIMER0A);

        TimerDisable(TIMER0_BASE, TIMER_A);
        count = 0;
        vTaskNotifyGiveFromISR(thFlashMorse, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    }
}


void GetAndTranslateTask(void* pvParameters)
{
    char string[50];
    MorseTranslation_t TranslatedWord[MAX_INPUT_LENGTH];
    int decodeRet;
    int i;
    while(1)
    {
        UARTprintf(" Please input a message to encode (%d character limit, Alphanumeric Characters Only)\n", MAX_INPUT_LENGTH);

        // Get string from user
        UARTgets(string, MAX_INPUT_LENGTH);

        decodeRet = DecodeString(string, TranslatedWord);

        if(decodeRet < 0)
        {
            // Raise an error, should be positive unless invalid characters
//            xQueueSend(LogMsgQueue, )
        }

        // DecodeRet holds amount of characters that were translated to Morse
        // Add to message queue for flashing task
        for(i = 0; i < decodeRet; i++)
        {
            xQueueSend(mqTranslatedChars, (void *) &TranslatedWord[i], (TickType_t) 0);
        }

        // Block the Task until the Flasher Task finished
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}


void FlashMorseTask(void* pvParameters)
{
    MorseTranslation_t currentMorseChar;
    int i;

    // Setup timer to time the light flashing, not enabled here
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
    IntMasterEnable();


    // Initialize GPIO
    init_led_gpio();

    while(1)
    {
        while (xQueueReceive(mqTranslatedChars, &currentMorseChar, (TickType_t) portMAX_DELAY))     // Obtain translated characters
        {
            i = 0;
            while(1)
            {
                LEDWrite(currentMorseChar.MorseWord[i]);

                if(currentMorseChar.MorseWord[i] == END || currentMorseChar.MorseWord[i] == START)
                {
                    break;
                }

                i++;
            }

            if(uxQueueMessagesWaiting( mqTranslatedChars ) == 0)    // Entire string has been transmitted
            {
                // Unblock the UI/Translate task
                xTaskNotify( thGetAndTrans, 0, eNoAction );
                GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, GREEN_GPIO_PIN);
            }
        }

    }
}

void LEDWrite(MorseUnit_e unit)
{
    switch(unit)
    {
        case DOT:
            unitDuration = DOT_UNIT_LEN;
            GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, GREEN_GPIO_PIN);
            break;
        case DASH:
            unitDuration = DASH_UNIT_LEN;
            GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, GREEN_GPIO_PIN);
            break;
        case END:
            unitDuration = END_UNIT_LEN;
            GPIOPinWrite(BLUE_GPIO_BASE, BLUE_GPIO_PIN, BLUE_GPIO_PIN);
            break;
        case SPACE:
            unitDuration = SPACE_UNIT_LEN;
            GPIOPinWrite(RED_GPIO_BASE, RED_GPIO_PIN, RED_GPIO_PIN);
            break;
        case START:
            unitDuration = START_UNIT_LEN;
            GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, 0x0);
            break;
        default:
            // Catch error
            break;

    }
    StartMorseTimer();
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // Turn lights off for 1 dot unit after each flash of the light
    unitDuration = DOT_UNIT_LEN;
    GPIOPinWrite(RED_GPIO_BASE, RED_GPIO_PIN, 0x0);
    GPIOPinWrite(BLUE_GPIO_BASE, BLUE_GPIO_PIN, 0x0);
    GPIOPinWrite(GREEN_GPIO_BASE, GREEN_GPIO_PIN, 0x0);

    StartMorseTimer();
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}


void StartMorseTimer(void)
{
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/SECOND_SLICE);       // 1s intervals
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

