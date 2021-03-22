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

/*Variable globale*/
    SemaphoreHandle_t bouton_semph;
    int count =2;
    task_params_t   task_A = {
                    .delay = 1000,
                    .message = "Tache A en cours\n\r"
                    };
    task_params_t   task_B = {
                    .delay = 999,
                    .message = "Tache B en cours \n\r"
                    };
    QueueHandle_t print_queue;

void vLEDTask()
{
    for(;;)
    {
    Cy_GPIO_Write(LED_0_PORT,LED_0_NUM,1);
    vTaskDelay(pdMS_TO_TICKS(250));
    Cy_GPIO_Write(LED_0_PORT,LED_0_NUM,0);
    vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void isr_bouton(void)
{
        xSemaphoreGiveFromISR(bouton_semph,NULL);
        Cy_GPIO_ClearInterrupt(Bouton_0_PORT,Bouton_0_NUM);
        NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
                
}

void vbouton_task()
{
    for(;;)
    {
        xSemaphoreTake(bouton_semph,portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(20));
        if(count%2 ==0)
        {
            UART_1_PutString("\n\r Bouton appuye");
            count++;
            bouton_semph= xSemaphoreCreateBinary();
        }
        else if(count%2 !=0)
        {
            UART_1_PutString("\n\r Bouton relache");
            count++;
            bouton_semph= xSemaphoreCreateBinary();
        }
    }
        
    
}
void print_loop(void *params)
    {
        task_params_t parametre = *(task_params_t*)params;
        char *pointepetitpointeur= &(parametre.message[0]);
    for(;;)
        {
        CyDelay(parametre.delay);
        //UART_1_PutString(parametre.message); PARTIE 2
        xQueueSend(print_queue, &pointepetitpointeur, portMAX_DELAY);
        }
    }

void print()
    {
        char * message; 
        for (;;)
        {
            xQueueReceive(print_queue, &message, portMAX_DELAY);
            UART_1_PutString(message);
        }
    }
    
int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    UART_1_Start();
    
    bouton_semph=xSemaphoreCreateBinary(); //initialisé avant la prochaine ligne !
    /*Initialize interrupt*/
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
     print_queue = xQueueCreate(2, sizeof(char *));
    
    
    xTaskCreate(vbouton_task, "tache_affichage_bouton",80,NULL,3,NULL);
    xTaskCreate(vLEDTask,"LED",80,NULL,3,NULL); 
    xTaskCreate(print_loop, "tache A", configMINIMAL_STACK_SIZE,(void *)&task_A, 1, NULL);
    xTaskCreate(print_loop, "tache B", configMINIMAL_STACK_SIZE, (void *)&task_B, 1, NULL);
    xTaskCreate(print,"printprintwutprint",80,NULL,3,NULL);
    vTaskStartScheduler();
    
    
    for(;;)
    {
        /* Place your application code here. */
    }

/* [] END OF FILE */
}
