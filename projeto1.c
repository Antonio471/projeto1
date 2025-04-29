#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "pico/multicore.h"

#define JOY_X_PIN 26
#define JOY_Y_PIN 27
#define LED_VERDE 11
#define LED_VERMELHO 13
#define LED_AZUL 12
#define BUZZER 21

volatile int prioridade = 0;

void core1_main() {
    while (true) {
        int atual = prioridade;

        gpio_put(LED_VERDE, 0);
        gpio_put(LED_AZUL, 0);
        gpio_put(LED_VERMELHO, 0);
        gpio_put(BUZZER, 0);

        if (atual == 1){
            gpio_put(LED_AZUL, 1);
        } else if(atual == 2) {
            gpio_put(LED_VERDE, 1);
        } else if(atual == 3) {
            gpio_put(LED_VERMELHO, 1);
            gpio_put(BUZZER, 1);
        }

        sleep_ms(100);
    }
}
int main() {
    stdio_init_all();

    gpio_init(LED_VERDE);
    gpio_init(LED_AZUL);
    gpio_init(LED_VERMELHO);
    gpio_init(BUZZER);

    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(BUZZER, GPIO_OUT);

    adc_init();
    adc_gpio_init(JOY_X_PIN);
    adc_gpio_init(JOY_Y_PIN);

    multicore_launch_core1(core1_main);

    while (true)
    {
       adc_select_input(0); 
       uint16_t x = adc_read();
       adc_select_input(1); 
       uint16_t y = adc_read();

       if (x > 3500 || y > 3500) prioridade = 3;
        else if (x > 2500 || y > 2500) prioridade = 2;
        else if (x > 1500 || y > 1500) prioridade = 1;
        else prioridade = 0;

        sleep_ms(100);
    }
}