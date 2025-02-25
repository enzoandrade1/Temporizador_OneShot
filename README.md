# Projeto: Temporizador de um Disparo com Raspberry Pi Pico W

## Descrição
Este projeto implementa um sistema de temporização utilizando a função `add_alarm_in_ms()` da ferramenta Pico SDK. O sistema aciona três LEDs (azul, vermelho e verde) a partir do clique em um botão (pushbutton), desligando-os sequencialmente a cada 3 segundos.

## Requisitos do Sistema
- **Microcontrolador**: Raspberry Pi Pico W
- **Componentes**:
  - 3 LEDs (azul, verde e vermelho)
  - 3 Resistores de 330 Ω
  - 1 Botão (Pushbutton)

## Funcionamento
1. Ao pressionar o botão (GPIO 5), todos os LEDs serão ligados.
2. A cada 3 segundos, um LED será desligado:
   - Primeiro, o LED azul é desligado.
   - Depois, o LED verde é desligado.
   - Por fim, o LED vermelho é desligado.
3. Durante a sequência de desligamento, o botão não pode iniciar um novo ciclo.
4. O botão só poderá ser pressionado novamente após o último LED ser desligado.

## Configuração dos GPIOs
- **GPIO 5**: Entrada do botão (pushbutton)
- **GPIO 11**: LED Azul
- **GPIO 12**: LED Verde
- **GPIO 13**: LED Vermelho

## Estrutura do Código
O código está estruturado com as seguintes funções principais:
- `button_callback()`: Função de interrupção do botão para detectar cliques com debounce.
- `timer_callback()`: Callback do temporizador que gerencia o desligamento dos LEDs.
- `init_button()`: Inicializa e configura o botão com interrupções.
- `init_leds()`: Inicializa e configura os LEDs.
- `main()`: Loop principal que monitora o botão e inicia a sequência de temporização.

## Como Executar
1. **Carregar o código** no Raspberry Pi Pico W.
2. **Pressionar o botão** para iniciar o ciclo de iluminação dos LEDs.
3. **Observar a sequência de desligamento** dos LEDs a cada 3 segundos.

## Exemplo de Uso
```c
if (button_pressed && !timer_active) {
    button_pressed = false;
    timer_active = true;
    add_alarm_in_ms(3000, timer_callback, NULL, false);
}
```

## Autor
Desenvolvido por Enzo Andrade Sousa Santos.


