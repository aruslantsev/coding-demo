#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#define DHT_PIN GPIO_NUM_18
#define ets_delay_us esp_rom_delay_us


enum dht11_status {
    DHT11_CRC_ERROR = -2,
    DHT11_TIMEOUT_ERROR = -1,
    DHT11_OK = 0
};


struct dht11_data {
    int16_t status;
    double temperature;
    double humidity;
};


int32_t measure_delay(gpio_num_t pin_num, uint16_t level, uint16_t timeout) {
    int32_t delay = 0;
    while (gpio_get_level(pin_num) == level) {
        delay++;
        if (delay > timeout)
            return DHT11_TIMEOUT_ERROR;
        ets_delay_us(1);
    }
    return delay;
}


void send_hello(gpio_num_t pin_num) {
    gpio_set_direction(pin_num, GPIO_MODE_OUTPUT);
    gpio_set_level(pin_num, 0);
    ets_delay_us(20000);
    gpio_set_level(pin_num, 1);
    ets_delay_us(40);
    gpio_set_direction(pin_num, GPIO_MODE_INPUT);
}


int16_t wait_response(gpio_num_t pin_num) {
    if (measure_delay(pin_num, 0, 80) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;
    if (measure_delay(pin_num, 1, 80) == DHT11_TIMEOUT_ERROR)
        return DHT11_TIMEOUT_ERROR;
    return DHT11_OK;
}


double bytes_to_double(uint8_t integer, uint8_t decimal) {
    double result = 0, converted = 0;
    result = (double) integer;
    converted = 0;
    while (decimal > 0) {
        converted += decimal % 10;
        converted /= 10;
        decimal /= 10;
    }
    return result + converted;
}


struct dht11_data dht_read(gpio_num_t dht11_pin) {
    struct dht11_data dht_reading = {DHT11_OK, 0, 0};
    uint8_t data[5] = {0, 0, 0, 0, 0};
    send_hello(dht11_pin);
    if (wait_response(dht11_pin) == DHT11_TIMEOUT_ERROR)
        dht_reading.status = DHT11_TIMEOUT_ERROR;
    for (int count = 0; count < 40; count++) {
        if (measure_delay(dht11_pin, 0, 50) == DHT11_TIMEOUT_ERROR)
            dht_reading.status = DHT11_TIMEOUT_ERROR;
        if (measure_delay(dht11_pin, 1, 70) > 28)
            data[count / 8] |= (1 << (7 - (count  % 8)));
    }
    dht_reading.temperature = bytes_to_double(data[2], data[3]);
    dht_reading.humidity = bytes_to_double(data[0], data[1]);
    if (data[0] + data[1] + data[2] + data[3] != data[4])
        dht_reading.status = DHT11_CRC_ERROR;
    return dht_reading;
}


void app_main(void)
{
    struct dht11_data dht_data;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        dht_data = dht_read(DHT_PIN);
        switch (dht_data.status) {
        case DHT11_TIMEOUT_ERROR:
            printf("Timeout error\n");
            break;
        case DHT11_CRC_ERROR:
            printf("CRC error\n");
            break;
        default:
            break;
        }
        printf("%.2f %.2f\n", dht_data.temperature, dht_data.humidity);
    }
}
