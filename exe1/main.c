#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
volatile int b_status =0;
volatile int n_status=0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    b_status =1;
  } else if (events == 0x8) { // rise edge
    n_status=1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (b_status){
      printf("fall \n");
      b_status = 0; // clean IRS flag
    }
    if (n_status){
      printf("rise \n");
      n_status=0;
    }
  }
}
