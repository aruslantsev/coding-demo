#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#define DHT_PIN GPIO_NUM_18
#define ets_delay_us esp_rom_delay_us


void send_hello(gpio_num_t pin_num) {
    gpio_set_direction(pin_num, GPIO_MODE_OUTPUT);
    gpio_set_level(pin_num, 0);
    ets_delay_us(20000);
    gpio_set_level(pin_num, 1);
    ets_delay_us(40);
    gpio_set_direction(pin_num, GPIO_MODE_INPUT);
}


int32_t measure_delay(gpio_num_t pin_num, int32_t level) {
    int32_t delay = 0;
    while (gpio_get_level(pin_num) == level) {
        delay++;
        if (delay > 1000)
            return -1;
        ets_delay_us(1);
    }
    return delay;
}


int32_t wait_response(gpio_num_t pin_num) {
    uint32_t timer_low, timer_high;
    timer_low = measure_delay(pin_num, 0);
    timer_high = measure_delay(pin_num, 0);
    return ((timer_low == -1) || (timer_high == -1));
}


int32_t adc_read() {
    int32_t high_delay;
    uint8_t data[5] = {0, 0, 0, 0, 0};
    double temperature, humidity;

    send_hello(DHT_PIN);
    if (wait_response(DHT_PIN) != 0) {
        printf("Failed to initialize\n");
        return -1;
    }

    for (int count = 0; count < 40; count++) {
        if (measure_delay(DHT_PIN, 0) == -1) {
            printf("Failed to read, low level\n");
            return -1;
        }
        high_delay = measure_delay(DHT_PIN, 1);
        if (high_delay == -1) {
            printf("Failed to read, low level\n");
            return -1;
        }
        if (high_delay > 28)
            data[count / 8] |= (1 << (7 - (count % 8)));
    }
    if (((data[0] + data[1] + data[2] + data[3]) & 0xff) != data[4])
        printf("CRC error\n");

    humidity = (data[0] + data[1]) / 10.0;
    temperature = (data[2] + data[3]) / 10.0;
    printf("%f %f\n", humidity, temperature);
    return 0;
}

void app_main(void)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        adc_read();
    }
}
