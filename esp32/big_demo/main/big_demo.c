#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

#define LED_PIN         GPIO_NUM_2
#define BTN_PIN         GPIO_NUM_18
#define BTN_LED_PIN     GPIO_NUM_19
#define ADC_UNIT        ADC_UNIT_1

#define is_debug        0

TaskHandle_t Cycle1sHandle = NULL, Delay1sHandle = NULL, TaskDispatcherHandle = NULL, Blink1sHandle = NULL, BtnLEDHandle = NULL, ADCReadHandle = NULL;

/*
driver/adc.h legacy adc driver is deprecated, please migrate to use esp_adc/adc_oneshot.h and esp_adc/adc_continuous.h for oneshot mode and continuous mode drivers respectively
esp_adc_cal.h legacy adc calibration driver is deprecated, please migrate to use esp_adc/adc_cali.h and esp_adc/adc_cali_scheme.h
'ADC_ATTEN_DB_11' is deprecated [-Wdeprecated-declarations]
ADC_ATTEN_DB_11 __attribute__((deprecated)) = ADC_ATTEN_DB_12,  ///<This is deprecated, it behaves the same as `ADC_ATTEN_DB_12`
*/



void ADCRead(void *arg) {
    static esp_adc_cal_characteristics_t adc1_chars;

    esp_adc_cal_characterize(ADC_UNIT, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);

    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    uint32_t voltage;

    while (1)
    {
        int adc_value = adc1_get_raw(ADC1_CHANNEL_4);
        voltage = esp_adc_cal_raw_to_voltage(adc_value, &adc1_chars);
        printf("Voltage: %ld mV\n", voltage);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void BtnLED(void *arg) {
    int is_pressed = 0;
    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(BTN_LED_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        if (gpio_get_level(BTN_PIN) == 0) {
            if (is_pressed) {
                if (is_debug)
                    printf("Button released\n");
                gpio_set_level(BTN_LED_PIN, 0);
                is_pressed = 0;
            }
        } else {
            if (!is_pressed) {
                if (is_debug)
                    printf("Button pressed\n");
                gpio_set_level(BTN_LED_PIN, 1);
                is_pressed = 1;
            }
        }
        vTaskDelay(1);
    }
}

void Blink1s(void *arg) {
    // gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1) {
        if (is_debug)
            printf("LED. Current time: %ld. Last wake time %ld\n", xTaskGetTickCount() / portTICK_PERIOD_MS, last_wake_time / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 1);
        if (is_debug)
            printf("LED enabled on pin %d\n", LED_PIN);
        vTaskDelay(150 / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);
        if (is_debug)
            printf("LED disabled on pin %d\n", LED_PIN);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
        // After suspend and resume it blinks faster until it is on time because the resume time is in the past
    }
}

void TaskDispatcher(void *arg)
{
    for(int counter = 0;; counter++) {
        if (is_debug)
            printf("TaskDispatcher printing at %ld...\n", xTaskGetTickCount() / portTICK_PERIOD_MS);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (counter % 10 == 5) {
            if (is_debug)
                printf("Suspending tasks\n");
            vTaskSuspend(Cycle1sHandle);
            vTaskSuspend(Delay1sHandle);
            vTaskSuspend(Blink1sHandle);
            if (is_debug)
                printf("Done\n");
        } else if (counter % 10 == 0) {
            if (is_debug)
                printf("Resuming tasks\n");
            vTaskResume(Cycle1sHandle);
            vTaskResume(Delay1sHandle);
            vTaskResume(Blink1sHandle);
            if (is_debug)
                printf("Done\n");
        }

        // vTaskDelete(Blink1sHandle);
    }
}

void Cycle1s(void *arg) {
    /*
     * Demo. Every cycle should be 1s. But after suspend and resume the delay of vTaskDelayUntil will be zero
     * for some cycles because last_wake_time is in the past
     */
    TickType_t last_wake_time = xTaskGetTickCount(), curr_time;
    for (int i = 0;; i++) {
        curr_time = xTaskGetTickCount();
        if (is_debug)
            printf("Cycle1000. Cycle %d.  Woke up at %ld, planned at %ld, planning next wake up at %ld\n", i, curr_time, last_wake_time, last_wake_time + pdMS_TO_TICKS(1000));
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(1000));
    }
}

void Delay1s(void *arg) {
    /*
     * Demo. Sleeps 1s after each cycle. After suspend and resume the delay is 1s
     */
    TickType_t  last_wake_time = 0, curr_time;
    for (int i = 0;; i++) {
        curr_time = xTaskGetTickCount();
        if (is_debug)
            printf("Delay1000. Cycle %d. Woke up at %ld, planned at %ld, planning next wake up at %ld\n", i, curr_time, last_wake_time, curr_time + pdMS_TO_TICKS(1000));
        last_wake_time = curr_time + pdMS_TO_TICKS(1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    xTaskCreate(TaskDispatcher, "TaskDispatcher", 4096, NULL, 10, &TaskDispatcherHandle);
    xTaskCreate(Cycle1s, "Cycle1s", 4096, NULL, 10, &Cycle1sHandle);
    xTaskCreate(Delay1s, "Delay1s", 4096, NULL, 10, &Delay1sHandle);
    xTaskCreate(Blink1s, "Blink1s", 4096, NULL, 10, &Blink1sHandle);
    xTaskCreate(BtnLED, "BtnLED", 4096, NULL, 10, &BtnLEDHandle);
    xTaskCreate(ADCRead, "ADCRead", 4096, NULL, 10, &ADCReadHandle);
}
