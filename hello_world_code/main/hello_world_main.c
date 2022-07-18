/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "output_iot.h"
#include "input_iot.h"

// TimerHandle_t xTimers[2];
EventGroupHandle_t xEventGroup;

#define BIT_EVENT_BUTTON_PRESS (1 << 0)
#define BIT_EVENT_BUTTON_DOUBLE_PRESS (1 << 1)

// void vTimerCallback(TimerHandle_t xTimer)
// {
//    /* Optionally do something if the pxTimer parameter is NULL. */
//    configASSERT(xTimer);
//    int ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
//    if (ulCount == 0)
//    {
//       output_io_toggle(13);
//    }
//    else if (ulCount == 1)
//    {
//       printf("Hello\n");
//    }
// }

// void vTask1(void *pvParameters)
// {
//    for (;;)
//    {
//       printf("Task 1\n");
//       vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
// }

// void vTask2(void *pvParameters)
// {
//    for (;;)
//    {
//       printf("Task 2\n");
//       vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
// }

// void vTask3(void *pvParameters)
// {
//    for (;;)
//    {
//       printf("Task 3\n");
//       vTaskDelay(1000 / portTICK_PERIOD_MS);
//    }
// }

void vTaskButton(void *pvParameters)
{
   for (;;)
   {
      EventBits_t uxBits = xEventGroupWaitBits(
          xEventGroup,
          BIT_EVENT_BUTTON_PRESS | BIT_EVENT_BUTTON_DOUBLE_PRESS,
          pdTRUE,
          pdFALSE,
          portMAX_DELAY);

      if (uxBits & BIT_EVENT_BUTTON_PRESS)
      {
         printf("BUTTON PRESS\n");
         output_io_toggle(15);
      }
      if (uxBits & BIT_EVENT_BUTTON_DOUBLE_PRESS)
      {
         printf("BUTTON DOUBLE PRESS\n");
         output_io_toggle(13);
      }
   }
}

void button_callback(int pin)
{
   if (pin == GPIO_NUM_21)
   {
      BaseType_t pxHigherPriorityTaskWoken;
      xEventGroupSetBitsFromISR(xEventGroup, BIT_EVENT_BUTTON_PRESS, &pxHigherPriorityTaskWoken);
   }
}

void app_main(void)
{
   // xTimers[0] = xTimerCreate("TimerBlink", pdMS_TO_TICKS(500), pdTRUE, (void *)0, vTimerCallback);
   // xTimers[1] = xTimerCreate("TimerPrint", pdMS_TO_TICKS(1000), pdTRUE, (void *)1, vTimerCallback);

   output_io_create(13);
   input_io_create(GPIO_NUM_21, ANY_EDGE);
   input_set_callback(button_callback);

   // xTimerStart(xTimers[0], 0);
   // xTimerStart(xTimers[1], 0);

   xEventGroup = xEventGroupCreate();

   // xTaskCreate(
   //     vTask1,   /* Function that implements the task. */
   //     "vTask1", /* Text name for the task. */
   //     1024,     /* Stack size in words, not bytes. */
   //     NULL,     /* Parameter passed into the task. */
   //     4,        /* Priority at which the task is created. */
   //     NULL);    /* Used to pass out the created task's handle. */

   // xTaskCreate(
   //     vTask2,   /* Function that implements the task. */
   //     "vTask2", /* Text name for the task. */
   //     1024,     /* Stack size in words, not bytes. */
   //     NULL,     /* Parameter passed into the task. */
   //     5,        /* Priority at which the task is created. */
   //     NULL);    /* Used to pass out the created task's handle. */
   // xTaskCreate(
   //     vTask3,   /* Function that implements the task. */
   //     "vTask3", /* Text name for the task. */
   //     1024,     /* Stack size in words, not bytes. */
   //     NULL,     /* Parameter passed into the task. */
   //     6,        /* Priority at which the task is created. */
   //     NULL);    /* Used to pass out the created task's handle. */
   xTaskCreate(
       vTaskButton,   /* Function that implements the task. */
       "vTaskButton", /* Text name for the task. */
       1024,          /* Stack size in words, not bytes. */
       NULL,          /* Parameter passed into the task. */
       4,             /* Priority at which the task is created. */
       NULL);         /* Used to pass out the created task's handle. */
}
