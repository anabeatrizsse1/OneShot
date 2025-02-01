# OneShot
Este projeto implementa um sistema de controle sequencial de LEDs utilizando um Raspberry Pi Pico. Os LEDs acendem em ordem e apagam automaticamente após um determinado tempo. O ciclo reinicia automaticamente ao final e pode ser acionado por um botão.

# Componentes Utilizados

-1x Raspberry Pi Pico

-1x LED Azul (conectado ao GPIO 11)

-1x LED Vermelho (conectado ao GPIO 12)

-1x LED Verde (conectado ao GPIO 13)

-1x Botão (conectado ao GPIO 5)

-Resistores de pull-up (internos ativados no código)

-Cabos de conexão

# Funcionamento

-O sistema aguarda o acionamento do botão.

-Quando pressionado, o ciclo de iluminação inicia:

-O LED azul acende e apaga após 3 segundos.

-O LED vermelho acende e apaga após 6 segundos.

-O LED verde acende e apaga após 9 segundos.

-Ao final do ciclo, o sistema aguarda um intervalo e reinicia automaticamente.

# Como Executar o Projeto

Conecte os componentes conforme o esquema indicado.

Compile e carregue o código no Raspberry Pi Pico.

Pressione o botão para iniciar o ciclo de iluminação.
