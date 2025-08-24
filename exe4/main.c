#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN[2] = {28, 26};
const int LED_PIN[2] = {4, 6};

volatile int r_status = 0;
volatile int g_status = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
     if (gpio == BTN_PIN[0]) {
            r_status = 1;

    } else if (events == 0x8) { // rise edge
        if (gpio == BTN_PIN[1]) {
            g_status = 1;
        }
    }
}

int main() {
    int led_status_g = 0;
    int led_status_r = 0;
    int i;
    stdio_init_all();

    for (i = 0; i < 2; i++) {
        gpio_init(LED_PIN[i]);
        gpio_set_dir(LED_PIN[i], GPIO_OUT);

        gpio_init(BTN_PIN[i]);
        gpio_set_dir(BTN_PIN[i], GPIO_IN);
        gpio_pull_up(BTN_PIN[i]);
    }

    gpio_set_irq_enabled_with_callback(
        BTN_PIN[0], GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    gpio_set_irq_enabled(BTN_PIN[1],
                         GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                         true);
    while (true) {
        if (r_status) {
            led_status_r = !led_status_r;
            gpio_put(LED_PIN[0], led_status_r);
            r_status = 0; // clean IRS flag
        }
        if (g_status) {
            led_status_g = !led_status_g;
            gpio_put(LED_PIN[1], led_status_g);
            g_status = 0; // clean IRS flag
        }
    }
}
