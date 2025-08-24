#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_R = 4;

volatile int b_status = 0;
volatile int n_status = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        b_status = 1;
    } else if (events == 0x8) { // rise edge
        n_status = 1;
    }
}

int main() {
    int led_status = 0;
    uint32_t start_ms;
    uint32_t final_ms;
    stdio_init_all();

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    while (true) {
        if (b_status) {
            led_status = !led_status;
            gpio_put(LED_R, led_status);
            start_ms = to_ms_since_boot(get_absolute_time());
            b_status = 0; // clean IRS flag
        }
        if (n_status) {
            final_ms = to_ms_since_boot(get_absolute_time());
            if (final_ms - start_ms < 800) {
                printf("Aperto curto!\n");
            } else {
                printf("Aperto longo!\n");
            }
            n_status=0;
        }
    }
}
