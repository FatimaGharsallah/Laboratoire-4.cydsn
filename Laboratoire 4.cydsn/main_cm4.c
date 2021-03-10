/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "params.h"
#include "queue.h"

void vLEDTask(void)
{
    for(;;)
    {
    Cy_GPIO_Write(LED_0_PORT,LED_0_NUM,1);
    vTaskDelay(pdMS_TO_TICKS(250));
    Cy_GPIO_Write(LED_0_PORT,LED_0_NUM,0);
    vTaskDelay(pdMS_TO_TICKS(250));
    }
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    xTaskCreate(vLEDTask,"LED",80,NULL,1,NULL); 
    vTaskStartScheduler();
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
