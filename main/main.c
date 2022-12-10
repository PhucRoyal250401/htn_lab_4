#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"

#include <stdio.h>
#include <time.h>
#include "sdkconfig.h"

static const uint32_t queueSize = 4;
static QueueHandle_t queue;

// message structure
typedef struct AMessage
{
    int id;
    char *value;
} xMessage;

void receptionalTask(void *arg)
{
    static int taskID = 1;
    srand(time(NULL));
    while (1)
    {
        xMessage inputMessage;
        srand(time(NULL));
        taskID = rand() % 4 + 1; // random number from 1 to 4
        inputMessage.id = taskID;
        switch (taskID)
        {
        case 1:
            inputMessage.value = "Functional Task 1";
            break;
        case 2:
            inputMessage.value = "Functional Task 2";
            break;
        case 3:
            inputMessage.value = "Functional Task 3";
            break;
        case 4:
            inputMessage.value = "No task to execute";
            break;
        default:
            printf("ERROR task ID\n");
            break;
        }
        xQueueSend(queue, &inputMessage, 10);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void errorTask(void *arg)
{
    xMessage message;
    while (1)
    {
        if (queue != 0)
        {
            // Receive an item from a queue without removing the item from the queue.
            if (xQueuePeek(queue, (void *)&message, 10) == pdTRUE)
            {
                if (message.id == 4)
                {
                    if (xQueueReceive(queue, (void *)&message, 10) == pdPASS)
                    {
                        printf("Error: %s with data id = %d.\n", message.value, message.id);
                    }
                }
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void functionalTask1(void *pvParameter)
{
    xMessage message;
    while (1)
    {
        if (queue != 0)
        {
            // Receive an item from a queue without removing the item from the queue.
            if (xQueuePeek(queue, (void *)&message, 10) == pdTRUE)
            {
                if (message.id == 1)
                {
                    if (xQueueReceive(queue, (void *)&message, 10) == pdPASS)
                    {
                        printf("Task 1 receive message '%s' with id = %d.\n", message.value, message.id);
                    }
                }
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}
void functionalTask2(void *arg)
{
    xMessage message;
    while (1)
    {
        if (queue != 0)
        {
            if (xQueuePeek(queue, (void *)&message, 10) == pdTRUE)
            {
                if (message.id == 2)
                {
                    if (xQueueReceive(queue, (void *)&message, 10) == pdPASS)
                    {
                        printf("Task 2 receive message '%s' with id = %d.\n", message.value, message.id);
                    }
                }
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}
void functionalTask3(void *arg)
{
    xMessage message;
    while (1)
    {
        if (queue != 0)
        {
            if (xQueuePeek(queue, (void *)&message, 10) == pdTRUE)
            {
                if (message.id == 3)
                {
                    if (xQueueReceive(queue, (void *)&message, 10) == pdPASS)
                    {
                        printf("Task 3 receive message '%s' with id = %d.\n", message.value, message.id);
                    }
                }
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    queue = xQueueCreate(queueSize, sizeof(xMessage));
    xTaskCreatePinnedToCore(&receptionalTask, "receptional task", 2048, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(&functionalTask1, "functional_task_1", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&functionalTask2, "functional_task_2", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&functionalTask3, "functional_task_3", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&errorTask, "error_task", 2048, NULL, 1, NULL, 1);
}