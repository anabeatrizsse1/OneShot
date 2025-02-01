#include <stdio.h>
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Definicao dos pinos dos LEDs
#define BLUE_LED_PIN 11
#define RED_LED_PIN 12
#define GREEN_LED_PIN 13
#define BUTTON_PIN 5

// Tempo que cada LED fica aceso (em milissegundos)
#define INTERVAL_DURATION 3000  

// Variavel para controlar se o ciclo esta rodando
volatile bool is_running = false;

void handle_button(); // Declaracao antecipada da funcao

// Funcao para inicializar os componentes
void Conf_Led() {
    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);
    
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Ativa resistor de pull-up interno
}

// Funcao que apaga o LED azul apos o tempo definido
int64_t desligar_led_azul(alarm_id_t id, void *user_data) {
    gpio_put(BLUE_LED_PIN, 0);
    return 0;
}

// Funcao que apaga o LED vermelho apos o tempo definido
int64_t desligar_led_vermelho (alarm_id_t id, void *user_data) {
    gpio_put(RED_LED_PIN, 0);
    return 0;
}

// Funcao que apaga o LED verde e reinicia o ciclo apos o tempo definido
int64_t desligar_led_verde(alarm_id_t id, void *user_data) {
    gpio_put(GREEN_LED_PIN, 0);
    is_running = false;

    // Aguarda um pequeno intervalo antes de reiniciar o ciclo
    add_alarm_in_ms(INTERVAL_DURATION, (alarm_callback_t)handle_button, NULL, false);
    
    return 0;
}

// Funcao que inicia o ciclo de piscagem dos LEDs
void handle_button() {
    if (!is_running) {
        is_running = true;

        // Acende o LED azul e agenda para apagar
        gpio_put(BLUE_LED_PIN, 1);
        add_alarm_in_ms(INTERVAL_DURATION, desligar_led_azul, NULL, false);

        // Acende o LED vermelho e agenda para apagar
        gpio_put(RED_LED_PIN, 1);
        add_alarm_in_ms(INTERVAL_DURATION * 2, desligar_led_vermelho , NULL, false);

        // Acende o LED verde e agenda para apagar e reiniciar o ciclo
        gpio_put(GREEN_LED_PIN, 1);
        add_alarm_in_ms(INTERVAL_DURATION * 3, desligar_led_verde, NULL, false);
    }
}

int main() {
    stdio_init_all();  // Inicializa a comunicacao serial
    Conf_Led(); // Configura os LEDs e o botao

    while (true) {
        // Se o botao for pressionado, inicia o ciclo
        if (gpio_get(BUTTON_PIN) == 0) {
            handle_button();
        }
        sleep_ms(100); // Pequeno delay para evitar leituras erradas do botao
    }
}
