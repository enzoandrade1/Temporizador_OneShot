#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define BUTTON_PIN 5          // Botão A no GPIO 5
#define LED_PIN_1 11          // LED 1 no GPIO 11 (azul)
#define LED_PIN_2 12          // LED 2 no GPIO 12 (verde)
#define LED_PIN_3 13          // LED 3 no GPIO 13 (vermelho)

volatile bool button_pressed = false;  // Flag para indicar que o botão foi pressionado
static absolute_time_t last_button_time;  // Debounce
bool timer_active = false;               // Flag para evitar múltiplas ativações do temporizador

// Função de callback para o botão
void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN) {
        // Debounce: ignora eventos com menos de 500ms do último
        if (absolute_time_diff_us(last_button_time, get_absolute_time()) > 500000) {
            button_pressed = true;
            last_button_time = get_absolute_time();
        }
    }
}

// Função de callback para o temporizador
bool timer_callback(repeating_timer_t *rt) {
    static int led_state = 0;  // Controla o estado atual dos LEDs

    switch (led_state) {
        case 0:
            // Liga todos os LEDs
            gpio_put(LED_PIN_1, 1);
            gpio_put(LED_PIN_2, 1);
            gpio_put(LED_PIN_3, 1);
            led_state = 1;
            break;
        case 1:
            // Desliga o LED 1 (azul)
            gpio_put(LED_PIN_1, 0);
            led_state = 2;
            break;
        case 2:
            // Desliga o LED 2 (verde)
            gpio_put(LED_PIN_2, 0);
            led_state = 3;
            break;
        case 3:
            // Desliga o LED 3 (vermelho)
            gpio_put(LED_PIN_3, 0);
            led_state = 0;
            timer_active = false;  // Permite novo pressionamento do botão
            return false;          // Para o temporizador
    }
    return true;  // Continua o temporizador
}

// Inicializa os botões
void init_button() {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Habilita interrupções para o botão
    gpio_set_irq_enabled(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_callback(&button_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);  // Habilita interrupções do GPIO
}

// Inicializa os LEDs
void init_leds() {
    gpio_init(LED_PIN_1);
    gpio_init(LED_PIN_2);
    gpio_init(LED_PIN_3);
    gpio_set_dir(LED_PIN_1, GPIO_OUT);
    gpio_set_dir(LED_PIN_2, GPIO_OUT);
    gpio_set_dir(LED_PIN_3, GPIO_OUT);
    gpio_put(LED_PIN_1, 0);
    gpio_put(LED_PIN_2, 0);
    gpio_put(LED_PIN_3, 0);
}

int main() {
    stdio_init_all();
    init_button();
    init_leds();

    repeating_timer_t timer;

    while (true) {
        if (button_pressed && !timer_active) {
            button_pressed = false;  // Reseta a flag do botão
            timer_active = true;     // Impede novo pressionamento durante a temporização

            // Configura o temporizador para disparar a cada 3 segundos
            add_repeating_timer_ms(3000, timer_callback, NULL, &timer);
        }
        sleep_ms(100);  // Evita uso excessivo da CPU
    }
}