/*************************
 * File: app.c
 *      This header file declares all tasks, variables, and defines required for the Light FLashing Portion of RTES Sp2020 Final Project
 *
 * Author: Mike Fruge
 */

#include <stdint.h>


#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"
#include "morse.h"

#define CLOCK_FREQ      50000000

/* # of Timer interrupts per second */
#define SECOND_SLICE            6000

/* Use these to set length of short and long pulses, expressed in timer cycles */
#define DOT_UNIT_LEN            1000
#define DASH_UNIT_LEN           3000


/* GPIO Pin Defines */
#define RED_GPIO_BASE           GPIO_PORTF_BASE
#define RED_TIMER_BASE          TIMER0_BASE
#define BLUE_GPIO_BASE          GPIO_PORTF_BASE
#define BLUE_TIMER_BASE         TIMER1_BASE
#define GREEN_GPIO_BASE         GPIO_PORTF_BASE
#define GREEN_TIMER_BASE        TIMER1_BASE

#define RED_GPIO_PIN            GPIO_PIN_1
#define BLUE_GPIO_PIN           GPIO_PIN_2
#define GREEN_GPIO_PIN          GPIO_PIN_3


/* Struct to hold messages */

typedef struct {
    uint8_t taskNum;
    uint8_t timeElapsed;
    uint8_t ExecutionNum;
} MessageStruct_t;



/* Create a message queue to be read out after test has concluded */
QueueHandle_t LogMsgQueue;

/* Message queue used to transport the Morse Packages */
QueueHandle_t mqTranslatedChars;


/* Task Handles defined for notifications */
TaskHandle_t thFlashMorse;
TaskHandle_t thGetAndTrans;



void myTimerIntHandler(void);

void GetAndTranslateTask(void* pvParameters);
void FlashMorseTask(void* pvParameters);

void InitLedGPIOpio(void);
void LEDWrite(MorseUnit_e unit);
void StartMorseTimer(void);

