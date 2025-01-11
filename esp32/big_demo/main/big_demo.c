#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

#define LED_PIN         GPIO_NUM_2
#define BTN_PIN         GPIO_NUM_18
#define BTN_LED_PIN     GPIO_NUM_19
#define ADC1            ADC_UNIT_1
#define ADC1_CH0        ADC_CHANNEL_4
#define ADC_ATTEN       ADC_ATTEN_DB_12

#define is_debug        0

TaskHandle_t Cycle1sHandle = NULL, Delay1sHandle = NULL, TaskDispatcherHandle = NULL, Blink1sHandle = NULL, BtnLEDHandle = NULL, ADCReadHandle = NULL;


void ADCRead(void *arg) {
    int adc_raw, adc_voltage;

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC1,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adc1_handle, ADC1_CH0, &config);

    adc_cali_handle_t adc1_cali_chan0_handle = NULL;
    /*
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC1,
        .chan = ADC1_CH0,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_cali_create_scheme_curve_fitting(&cali_config, &adc1_cali_chan0_handle);
    */
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC1,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_cali_create_scheme_line_fitting(&cali_config, &adc1_cali_chan0_handle);

    while (1) {
        adc_oneshot_read(adc1_handle, ADC1_CH0, &adc_raw);
        printf("ADC%d Channel[%d] Raw Data: %d\n", ADC_UNIT_1 + 1, ADC1_CH0, adc_raw);
        adc_cali_raw_to_voltage(adc1_cali_chan0_handle, adc_raw, &adc_voltage);
        printf("ADC%d Channel[%d] Cali Voltage: %d mV\n", ADC_UNIT_1 + 1, ADC1_CH0, adc_voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));
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
