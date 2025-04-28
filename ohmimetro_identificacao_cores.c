#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "pico/bootrom.h"
#include "numeros.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define ADC_PIN 28 // GPIO para o voltímetro
#define Botao_A 5  // GPIO para botão A
#define botaoB 6   // botão B para entrar no modo BOOTSEL
#define MATRIZ_LED_PIN 7        // Pino GPIO 7 conectado à matriz de LEDs WS2812

int R_conhecido = 10000;   // Resistor conhecido
float ADC_VREF = 3.31;     // Tensão de referência
int ADC_RESOLUTION = 4095; // Resolução ADC

char cor1[10], cor2[10], cor3[10], cor4[10];

const float valores_comerciais[] = {
    0.22, 0.24, 0.27, 0.3, 0.33, 0.36, 0.39, 0.43, 0.47, 0.51,
    0.56, 0.62, 0.68, 0.75, 0.82, 0.91,
    1, 1.1, 1.2, 1.3, 1.5, 1.8, 2, 2.2, 2.4, 2.7, 3, 3.3, 3.6, 3.9,
    4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1,
    10, 11, 12, 13, 15, 18, 20, 22, 24, 27, 30, 33, 36, 39,
    43, 47, 51, 56, 62, 68, 75, 82, 91,
    100, 110, 120, 130, 150, 180, 200, 220, 240, 270, 300, 330, 360, 390,
    430, 470, 510, 560, 620, 680, 750, 820, 910,
    1000, 1100, 1200, 1300, 1500, 1800, 2000, 2200, 2400, 2700, 3000, 3300, 3600, 3900,
    4300, 4700, 5100, 5600, 6200, 6800, 7500, 8200, 9100,
    10000, 11000, 12000, 13000, 15000, 18000, 20000, 22000, 24000, 27000, 30000, 33000, 36000, 39000,
    43000, 47000, 51000, 56000, 62000, 68000, 75000, 82000, 91000,
    100000, 110000, 120000, 130000, 150000, 180000, 200000, 220000, 240000, 270000, 300000, 330000, 360000, 390000,
    430000, 470000, 510000, 560000, 620000, 680000, 750000, 820000, 910000,
    1000000, 1100000, 1200000, 1300000, 1500000, 1800000, 2000000, 2200000, 2400000, 2700000, 3000000, 3300000, 3600000, 3900000,
    4300000, 4700000, 5100000, 5600000, 6200000, 6800000, 7500000, 8200000, 9100000};
const int num_valores = sizeof(valores_comerciais) / sizeof(valores_comerciais[0]);

const char *cores[] = {
    "Preto", "Marrom", "Vermelho", "Laranja", "Amarelo",
    "Verde", "Azul", "Violeta", "Cinza", "Branco"};

    int primeiro_numero = 0;
    int segundo_numero = 0;
    int terceiro_numero = 0;

// Funções auxiliares
float encontrar_valor_proximo(float valor)
{
  float melhor = valores_comerciais[0];
  float menor_erro = fabs(valor - melhor);

  for (int i = 1; i < num_valores; i++)
  {
    float erro = fabs(valor - valores_comerciais[i]);
    if (erro < menor_erro)
    {
      melhor = valores_comerciais[i];
      menor_erro = erro;
    }
  }
  return melhor;
}

void calcular_cores(float valor, char *cor1, char *cor2, char *cor3)
{
  primeiro_numero = valor;
  segundo_numero = valor;
  terceiro_numero = valor;
  int count = 0;
  while (primeiro_numero >= 10)
  {
    primeiro_numero = primeiro_numero / 10;
    count++;
  }

  // Captura o primeiro dígito
  while (segundo_numero >= 100)
  {
    segundo_numero = segundo_numero / 10;
  }

  segundo_numero = segundo_numero - primeiro_numero * 10;
  terceiro_numero = count - 1;
  // Usa os valores das cores
  sprintf(cor1, "%s ", cores[primeiro_numero]);
  sprintf(cor2, "%s", cores[segundo_numero]);
  sprintf(cor3, "%s", cores[terceiro_numero]);
  sprintf(cor4, " %d%d%d",primeiro_numero,segundo_numero,terceiro_numero);
}


// Função para modo BOOTSEL
void gpio_irq_handler(uint gpio, uint32_t events)
{
  reset_usb_boot(0, 0);
}



int main()
{
  npInit(MATRIZ_LED_PIN);

  stdio_init_all();

  gpio_init(botaoB);
  gpio_set_dir(botaoB, GPIO_IN);
  gpio_pull_up(botaoB);
  gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

  gpio_init(Botao_A);
  gpio_set_dir(Botao_A, GPIO_IN);
  gpio_pull_up(Botao_A);

  // Inicializa I2C e Display
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  ssd1306_t ssd;
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_send_data(&ssd);
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  adc_init();
  adc_gpio_init(ADC_PIN);

  char str_adc[10], str_rx[10], str_valor[10];

  bool cor = true;

  
  //DesligaMatriz();
  while (true)
  {
     adc_select_input(2);
    float soma = 0.0f;
    for (int i = 0; i < 500; i++)
    {
      soma += adc_read();
      sleep_ms(1);
    }
    float media = soma / 500.0f;

    float R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);
    float R_comercial = encontrar_valor_proximo(R_x);

    calcular_cores(R_comercial, cor1, cor2, cor3);

    sprintf(str_adc, "%1.0f", media);
    sprintf(str_rx, "%1.0f Ohm", R_x);
    sprintf(str_valor, "%1.0f", R_comercial);
   


    // Atualiza o display
    ssd1306_fill(&ssd, false);

    // Linha das cores
    int borda = 2;
    
    ssd1306_rect(&ssd, 0, 0,128, 36, true, false);

    ssd1306_draw_string(&ssd, "1:", 0 + borda, 0 + borda);
    ssd1306_draw_string(&ssd, cor1, 15 + borda, 0 + borda);

    ssd1306_draw_string(&ssd, "2:", 0 + borda, 10 + borda);
    ssd1306_draw_string(&ssd, cor2, 15 + borda, 10 + borda);

    ssd1306_draw_string(&ssd, "3:", 0 + borda, 20 + borda);
    ssd1306_draw_string(&ssd, cor3, 15 + borda, 20 + borda);

    ssd1306_draw_string(&ssd,cor4,84 + borda, 0 + borda);
    ssd1306_rect(&ssd, 0, 0,84, 36, true, false);
    ssd1306_draw_string(&ssd, "ADC",90 + borda, 10 + borda);

    ssd1306_draw_string(&ssd,str_adc,90 + borda, 20 + borda);


    // Desenho do resistor
    //  ssd1306_line(&ssd, 0, 30, 40, 30, true);    // Fio esquerdo
    // Corpo do resistor
    // ssd1306_line(&ssd, 80, 30, 127, 30, true);   // Fio direito

    // Texto dos valores
    ssd1306_rect(&ssd, 36, 0, 128, 28, true, false);
    ssd1306_draw_string(&ssd, "Rx:", 0 + borda, 42);
    ssd1306_draw_string(&ssd, str_rx, 30 + borda, 42);
    ssd1306_draw_string(&ssd, "Rcomer:", 0 + borda, 52);
    ssd1306_draw_string(&ssd, str_valor, 57 + borda, 52);

    ssd1306_send_data(&ssd);
    Enviar_cores(primeiro_numero,segundo_numero,terceiro_numero);

    sleep_ms(1000);
  }
}
