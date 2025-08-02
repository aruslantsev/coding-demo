#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define LED_PIN GPIO_NUM_2
#define BUTTON_PIN GPIO_NUM_0
#define POLL_DELAY_MS 100
#define ALARM_TIME 50


enum State {
    WAIT,
    POLL,
    ALARM,
};


struct AlarmState{
    enum State state;
    uint16_t last_reset;
    uint16_t do_alarm;
};



void IRAM_ATTR change_state(void *arg) {
    ((struct AlarmState *) arg)->do_alarm = 0;
    ((struct AlarmState *) arg)->last_reset = xTaskGetTickCount() / portTICK_PERIOD_MS;
}


void do_wait() {
    printf("Waiting\n");
    vTaskDelay(pdMS_TO_TICKS(POLL_DELAY_MS));
}


int do_poll(struct AlarmState *alarm) {
    printf("Polling, remaining %lu\n", ALARM_TIME - (xTaskGetTickCount() / portTICK_PERIOD_MS - alarm->last_reset));
    if (xTaskGetTickCount() / portTICK_PERIOD_MS - alarm->last_reset >= ALARM_TIME) {
        return 1;
    }
    return 0;
}


void do_alarm(struct AlarmState *alarm) {
    printf("Alarm\n");
    int state = 0;
    while (alarm->do_alarm) {
        gpio_set_level(LED_PIN, state = !state);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(LED_PIN, state = !state);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void do_action(struct AlarmState *alarm) {
    switch (alarm->state) {
        case WAIT:
            do_wait();
            break;
        case POLL:
            alarm->do_alarm = do_poll(alarm);
            break;
        case ALARM:
            do_alarm(alarm);
            break;
        default:
            printf("Error. Wrong state\n");
            exit(1);
    }
};


void switch_state(struct AlarmState *alarm) {
    switch (alarm->state) {
        case WAIT:
            alarm->state = POLL;
            printf("WAIT->POLL\n");
            break;
        case POLL:
            if (alarm->do_alarm) {
                alarm->state = ALARM;
                printf("POLL->ALARM\n");
            } else {
                alarm->state = WAIT;
                printf("POLL->WAIT\n");
            };
            break;
        case ALARM:
            alarm->state = WAIT;
            printf("ALARM->WAIT\n");
            break;
        default:
            printf("Error. Wrong state\n");
            exit(1);
    }
};


void app_main(void)
{
    struct AlarmState alarm = {.state = WAIT, .last_reset = 0};
    printf("Setting up pins\n");
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ENABLE);
    printf("Setting up interrupts\n");
    gpio_install_isr_service(0);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);
    gpio_isr_handler_add(BUTTON_PIN, change_state, (void *) &alarm);
    while (1) {
        do_action(&alarm);
        switch_state(&alarm);
    }
}
