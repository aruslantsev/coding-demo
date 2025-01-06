#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#define LED_PIN GPIO_NUM_2

TaskHandle_t Cycle1000Handle = NULL, Delay1000Handle = NULL, TaskDispatcherHandle = NULL, Blink1sHandle = NULL;

void Blink1s(void *arg) {
    // gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1) {
        printf("LED. Current time: %ld. Last wake time %ld\n", xTaskGetTickCount() / portTICK_PERIOD_MS, last_wake_time / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 1);
        printf("LED enabled on pin %d\n", LED_PIN);
        vTaskDelay(150 / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);
        printf("LED disabled on pin %d\n", LED_PIN);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000)); // After suspend and resume works without delay for skipped cycles
    }
}

void TaskDispatcher(void *arg)
{
    for(int counter = 0;; counter++) {
        printf("TaskDispatcher printing at %ld...\n", xTaskGetTickCount() / portTICK_PERIOD_MS);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (counter % 10 == 5) {
            printf("Suspending tasks\n");
            vTaskSuspend(Cycle1000Handle);
            vTaskSuspend(Delay1000Handle);
            vTaskSuspend(Blink1sHandle);
            printf("Done\n");
        } else if (counter % 10 == 0) {
            printf("Resuming tasks\n");
            vTaskResume(Cycle1000Handle);
            vTaskResume(Delay1000Handle);
            vTaskResume(Blink1sHandle);
            printf("Done\n");
        }

        // vTaskDelete(Blink1sHandle);
    }
}

void Cycle1000(void *arg) {
    TickType_t last_wake_time = xTaskGetTickCount(), curr_time;
    for (int i = 0;; i++) {
        curr_time = xTaskGetTickCount();
        printf("Cycle1000. Cycle %d.  Woke up at %ld, planned at %ld, planning next wake up at %ld\n", i, curr_time, last_wake_time, last_wake_time + pdMS_TO_TICKS(1000));
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
    }
}

void Delay1000(void *arg) {
    TickType_t  last_wake_time = 0, curr_time;
    for (int i = 0;; i++) {
        curr_time = xTaskGetTickCount();
        printf("Delay1000. Cycle %d. Woke up at %ld, planned at %ld, planning next wake up at %ld\n", i, curr_time, last_wake_time, curr_time + pdMS_TO_TICKS(1000));
        last_wake_time = curr_time + pdMS_TO_TICKS(1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    xTaskCreate(TaskDispatcher, "TaskDispatcher", 4096, NULL, 10, &TaskDispatcherHandle);
    xTaskCreate(Cycle1000, "Cycle1000", 4096, NULL, 10, &Cycle1000Handle);
    xTaskCreate(Delay1000, "Delay1000", 4096, NULL, 10, &Delay1000Handle);
    xTaskCreate(Blink1s, "Blink1s", 4096, NULL, 10, &Blink1sHandle);
}
