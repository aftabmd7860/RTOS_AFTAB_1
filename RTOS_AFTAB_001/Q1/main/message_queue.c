
#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/FreeRTOSConfig.h>
#include<freertos/task.h>
#include<freertos/queue.h>


TaskHandle_t task1Handle, task2Handle, task3Handle, task4Handle, task5Handle;

QueueHandle_t msg_queue;
UBaseType_t QueueLength = 10;
UBaseType_t ItemSize = sizeof(int);
BaseType_t retQueueSend, retQueueReceive;



void Task1(void * data) 
{
    printf("Inside %s():\n", __FUNCTION__);

    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

    configASSERT(((uint32_t *)data) == NULL);

    for(;;) 
    {
        printf("Task1 : running...\n");
     
        vTaskDelay(xDelay);
    }
    vTaskDelete(task1Handle);
}

void Task2(void * data) 
{
    printf("Inside %s():\n", __FUNCTION__);

    const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;

    configASSERT(((uint32_t *)data) == NULL);

    for(;;) 
    {
        printf("Task2 : running...\n");
        vTaskDelay(xDelay);
    }

    vTaskDelete(task2Handle);
}

void Task3(void * data) 
{
    printf("Inside %s():\n", __FUNCTION__);


    const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;

    for(;;) 
    {
        printf("Task3 : running....\n");
        vTaskDelay(xDelay);
    }
    vTaskDelete(task3Handle);
}
 
void Task4(void * data) 
{
    int num = 0;

    printf("Inside %s():\n", __FUNCTION__);

    for(;;) 
    {
        retQueueSend = xQueueSend(msg_queue, (const void *)&num, pdMS_TO_TICKS(1000));
        if(retQueueSend == errQUEUE_FULL) {
            perror("error in QueueSend : ");
            exit(EXIT_FAILURE);
        }
        else if(retQueueSend == pdTRUE) {
            printf("Data sent : success\n");
            printf("Task-4 sent : data : %d\n", num);
            num++;
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    vTaskDelete(task4Handle);
}
void Task5(void * data) 
{
    int num = 0;

    printf("Inside %s():\n", __FUNCTION__);

    configASSERT(((uint32_t *)data) == NULL);

    for(;;) 
    {
        retQueueReceive = xQueueReceive(msg_queue, &num, portMAX_DELAY);
        if(retQueueReceive == pdFALSE) {
            perror("error in Message Queue Receive: \n");
            exit(EXIT_FAILURE);
        }
        else if (retQueueReceive == pdTRUE) {
            printf("Data receive : success\n");
            printf("Task-5 received : data : %d\n", num);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
    vTaskDelete(task5Handle);
}

void app_main()
{
    BaseType_t task1, task2, task3, task4, task5;

    printf("Inside %s() function:\n", __FUNCTION__);

    msg_queue = xQueueCreate(QueueLength,ItemSize);
    if(msg_queue == NULL) {
        perror("error in Message Queue : ");
        exit(EXIT_FAILURE);
    }
    printf("Message Queue : Created\n");
    
    task1 = xTaskCreate(Task1, "Task1", 4096, NULL, 5, &task1Handle);
    if(task1 != pdPASS) {
        perror("error in task1 ");
        exit(EXIT_FAILURE);
    }
    printf("Task1 : created\n");

    task2 = xTaskCreate(Task2, "Task2", 4096, NULL, 6, &task2Handle);
    if(task2 != pdPASS) {
        perror("error in task2 ");
        exit(EXIT_FAILURE);
    }
    printf("Task2 : created\n");

    task3 = xTaskCreate(Task3, "Task3", 4096, NULL, 7, &task3Handle);
    if(task3 != pdPASS) {
        perror("error in task3 ");
        exit(EXIT_FAILURE);
    }
    printf("Task3 : created\n");   //configASSERT(((uint32_t *)parameter) == NULL);

    task4 = xTaskCreate(Task4, "Task4", 4096, NULL, 8, &task4Handle);
    if(task4 != pdPASS) {
        perror("error in task4 ");
        exit(EXIT_FAILURE);
    }
    printf("Task4 : created\n");

    task5 = xTaskCreate(Task5, "Task5", 2048, NULL, 9, &task5Handle);
    if(task5 != pdPASS) {
        perror("error in task5 ");
        exit(EXIT_FAILURE);
    }
    printf("Task5 : created\n");

}