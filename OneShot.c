#include <stdio.h>
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Definicao dos pinos dos LEDs Azul, Vermelho e Verde
#define LED_B 11  
#define LED_R 12  
#define LED_G 13  
#define BUTTON_PIN 5  

// Tempo que cada LED fica aceso (em milissegundos)
#define INTERVAL_DURATION 3000  

// Variavel para controlar se o ciclo esta rodando
volatile bool is_running = false;

// Declaracao antecipada da funcao que inicia o ciclo dos LEDs
void handle_button(); 

// Funcao para configurar os LEDs e o botao
void Conf_Led() {
    // Inicializa e configura o LED azul como saida
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    
    // Inicializa e configura o LED vermelho como saida
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    
    // Inicializa e configura o LED verde como saida
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    
    // Inicializa e configura o botao como entrada com pull-up interno ativado
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
}

// Funcao que apaga o LED azul apos um tempo determinado
int64_t desligar_led_azul(alarm_id_t id, void *user_data) {
    gpio_put(LED_B, 0); // Desliga o LED azul
    return 0;
}

// Funcao que apaga o LED vermelho apos um tempo determinado
int64_t desligar_led_vermelho(alarm_id_t id, void *user_data) {
    gpio_put(LED_R, 0); // Desliga o LED vermelho
    return 0;
}

// Funcao que apaga o LED verde e reinicia o ciclo
int64_t desligar_led_verde(alarm_id_t id, void *user_data) {
    gpio_put(LED_G, 0); // Desliga o LED verde
    is_running = false; // Indica que o ciclo terminou

    // Aguarda um tempo antes de reiniciar o ciclo automaticamente
    add_alarm_in_ms(INTERVAL_DURATION, (alarm_callback_t)handle_button, NULL, false);
    
    return 0;
}

// Funcao que inicia o ciclo de acendimento dos LEDs
void handle_button() {
    if (!is_running) { // Se o ciclo nao estiver rodando, inicia
        is_running = true;

        // Acende o LED azul e programa para apagar depois de um tempo
        gpio_put(LED_B, 1);
        add_alarm_in_ms(INTERVAL_DURATION, desligar_led_azul, NULL, false);

        // Acende o LED vermelho e programa para apagar depois do dobro do tempo
        gpio_put(LED_R, 1);
        add_alarm_in_ms(INTERVAL_DURATION * 2, desligar_led_vermelho, NULL, false);

        // Acende o LED verde e programa para apagar e reiniciar o ciclo
        gpio_put(LED_G, 1);
        add_alarm_in_ms(INTERVAL_DURATION * 3, desligar_led_verde, NULL, false);
    }
}

int main() {
    stdio_init_all();  // Inicializa a comunicacao serial
    Conf_Led(); // Configura os LEDs e o botao

    while (true) {
        // Se o botao for pressionado (estado baixo), inicia o ciclo
        if (gpio_get(BUTTON_PIN) == 0) {
            handle_button();
        }
        sleep_ms(100); // Pequeno atraso para evitar leituras erradas do botao
    }
}
