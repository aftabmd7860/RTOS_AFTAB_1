#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/FreeRTOSConfig.h>
#include<freertos/task.h>
#include<freertos/timers.h>

TaskHandle_t mytask1, mytask2, mytask3, mytask4, mytask5;
TimerHandle_t retTimerCreate;



void TimerCallback(TimerHandle_t xTimer) 
{
    printf("Inside %s() function : \n", __FUNCTION__);

    printf("timer trigger::\n");
    const uint32_t MaxExpiryCountBeforeStopping = 10;
    uint32_t Count;

    configASSERT(xTimer);

    Count = (uint32_t)pvTimerGetTimerID(xTimer);
    Count++;


    if(Count>=MaxExpiryCountBeforeStopping) {
        printf("Timer Expire : 10 times : Stopping it now...\n");
        xTimerStop(xTimer,0);
    }
    else {
        vTimerSetTimerID(xTimer,(void*)Count);
    }

}

void Task1(void * arg) 
{
    printf("Inside %s():\n", __FUNCTION__);


    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;


    configASSERT(((uint32_t *)arg) == NULL);

    for(;;) 
    {
        printf("Task1 : running::\n");
        vTaskDelay(xDelay);
    }
    vTaskDelete(mytask1);
}

void Task2(void * arg) 
{
    printf("Inside %s():\n", __FUNCTION__);

    const TickType_t xDelay = 2000 / portTICK_PERIOD_MS;
    configASSERT(((uint32_t *)arg) == NULL);

    for(;;) 
    {
        printf("Task2 : running::\n");
        vTaskDelay(xDelay);
    }

    vTaskDelete(mytask2);
}
void Task3(void * parameter) 
{
    printf("Inside %s():\n", __FUNCTION__);

    const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;

    configASSERT(((uint32_t *)parameter) == NULL);
    retTimerCreate = xTimerCreate("One shot software Timer", pdMS_TO_TICKS(10000), pdFALSE, (void*)0, TimerCallback);
    if(retTimerCreate == NULL) {
        perror("error in timer creation ");
        exit(EXIT_FAILURE);
    }
    printf("Timer started....:::\n");
    xTimerStart(retTimerCreate,0);

    for(;;) 
    {
        printf("Task3 : running:\n");
        vTaskDelay(xDelay);
    }
    vTaskDelete(mytask3);
}
void app_main()
{
    BaseType_t task1, task2, task3;

    printf("Inside %s() function:\n", __FUNCTION__);
    
    task1 = xTaskCreate(Task1, "Task1", 4096, NULL, 5, &mytask1);
    if(task1 != pdPASS) {
        perror("error in task1 ");
        exit(EXIT_FAILURE);
    }
    printf("Task1 : created\n");

    task2 = xTaskCreate(Task2, "Task2", 4096, NULL, 6, &mytask2);
    if(task2 != pdPASS) {
        perror("error in task2 ");
        exit(EXIT_FAILURE);
    }
    printf("Task2 : created\n");

    task3 = xTaskCreate(Task3, "Task3", 4096, NULL, 7, &mytask3);
    if(task3 != pdPASS) {
        perror("error in task3 ");
        exit(EXIT_FAILURE);
    }
    printf("Task3 : created\n");

}