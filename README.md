<div align="center">
  <img src="EmbarcaTech_logo_Azul-1030x428.png" alt="EmbarcaTech Logo" width="600"/>
</div>

# 🧵 Projeto: Ohmímetro com Reconhecimento e Exibição de Cores em Display OLED e Matriz de LEDs Integrados na BitDogLab
## 📜 Descrição

Daniel Silva de Souza

### [Video Demonstrativo do Projeto](https://youtu.be/wP9TNFgGwGA)

Este projeto consiste em um **ohmímetro digital** baseado em **RP2040** (Raspberry Pi Pico ou BitDogLab), que realiza:

- Medição de resistência utilizando um divisor de tensão.
- Exibição no **display OLED** dos valores:
  - Valor medido (`Rx`)
  - Valor comercial mais próximo (`Rcomercial`)
  - Cores correspondentes às faixas do resistor.
- Representação gráfica dos **anéis de cores** de resistores:
  - Em um **display OLED** (I2C)
  - Em uma **matriz de LEDs WS2812B (NeoPixel)** com as faixas coloridas.

Além disso, o projeto suporta a entrada em **modo BOOTSEL** via botão.

## 🎯 Objetivo

Facilitar a **identificação automática** de resistores pelo seu valor medido, exibindo **visualmente** as cores que representam o resistor.

## 🛠️ Tecnologias e Componentes

- **Microcontrolador**: RP2040 (Raspberry Pi Pico ou BitDogLab)
- **Display**: OLED 128x64 (I2C)
- **Matriz de LEDs**: WS2812B (5x5 LEDs)
- **Leitura Analógica**: ADC do RP2040
- **Programação**: C (SDK da Raspberry Pi Pico)
- **Protocolos**: I2C, PIO para LEDs

## ⚙️ Funcionamento

- O sistema lê o valor analógico do divisor de tensão através do **ADC**.
- Calcula a resistência desconhecida usando a fórmula:

  \[
  Rx = \frac{R_{\text{conhecido}} \times \text{média\_ADC}}{4095 - \text{média\_ADC}}
  \]

## 📋 Funcionalidades Implementadas

- Leitura de resistência usando o conversor analógico-digital (ADC) do RP2040.
- Cálculo do valor da resistência medida (Rx).
- Identificação do valor comercial mais próximo (base E24).
- Determinação automática das faixas de cores do resistor.
- Exibição gráfica no display OLED:
  - Nome da cor de cada faixa (1ª, 2ª e 3ª).
  - Código numérico das faixas.
  - Valor da resistência lido (Rx) e valor comercial encontrado.
  - Resistores representados estilizados com linhas e retângulos no display.
- Implementação do botão B para entrar no modo BOOTSEL da placa.


## 🖥️ Organização dos Códigos

| Arquivo | Função |
|:--------|:-------|
| `ohmimetro_identificacao_cores.c` | Código principal: medição, cálculo, exibição no display. |
| `numeros.h` |Gerenciamento dos sprites da matriz de LEDs, funções auxiliares de controle de cor. |
| `matrizled.c` | Biblioteca de controle para a matriz de LEDs WS2812B via PIO. |

## 📷 Interface de Exibição

- **Display OLED**: mostra o valor lido, o valor comercial mais próximo, e as cores calculadas.
- **Matriz de LEDs WS2812B**: exibe três faixas coloridas correspondentes ao resistor lido.

## 📏 Dependências

- `lib/ssd1306.h` (Driver para o display OLED)
- `lib/font.h` (Fontes do display OLED)
- `ws2818b.pio.h` (Controle de matriz de LEDs usando PIO)
- `numeros.h` (Controle de matriz de LEDs usando PIO)
- `matrizled.c` (Controle de matriz de LEDs usando PIO)
- Pico SDK (`pico/stdlib.h`, `hardware/adc.h`, `hardware/i2c.h`)

## 🧩 Pinagem (Padrão)

| Função | Pino |
|:-------|:-----|
| SDA (I2C) | GPIO 14 |
| SCL (I2C) | GPIO 15 |
| ADC (entrada de tensão) | GPIO 28 |
| Botão B (modo BOOTSEL) | GPIO 6 |
| Matriz de LEDs WS2812 | GPIO 7 |

## 🚀 Como Compilar e Rodar

1. Configure o ambiente de desenvolvimento para RP2040 (Pico SDK).
2. Clone o repositório ou copie os arquivos.
3. Compile o projeto (`cmake`, `make`, ou usando IDEs como VSCode ou Arduino IDE para Pico).
4. Grave o binário no RP2040.
5. Conecte o display OLED e a matriz de LEDs.
6. O sistema inicia automaticamente a leitura e exibição.


## 📦 Exemplo de Tela

## 📌 Observações Técnicas

- **Reset USB**: botão B pode ser usado para reiniciar em modo BOOTSEL.
- **Precisão**: Média de 500 amostras ADC para maior estabilidade de leitura.
- **Referência de tensão**: definida em `ADC_VREF = 3.31V` (ajustável conforme necessário).


## 🔧 Hardware Utilizado

- **Placa:** BitDogLab (RP2040)
- **Display:** OLED 0.96" SSD1306 (I2C)
- **Resistor conhecido:** 10kΩ (R_conhecido)
- **Botões:** Botão B (modo BOOTSEL)

## 🛠️ Diagrama de Ligações

| Componente | GPIO |
|:-----------|:----:|
| SDA (Display OLED) | GP14 |
| SCL (Display OLED) | GP15 |
| ADC Entrada (Resistor Testado) | GP28 |
| Botão A | GP5 |
| Botão B (BOOTSEL) | GP6 |

> ⚡ Alimentação feita diretamente pela USB.

## 🎯 Exibição no Display
1: Marrom 2: Cinza 3: Vermelho

- As três cores representam a codificação do resistor.
- O retângulo no meio simula o corpo do resistor.
- Rx: resistência medida.
- Com: valor comercial encontrado.

## 🚀 Como Rodar o Projeto

1. Clone o repositório:

   ```bash
   git clone https://github.com/SEU-USUARIO/bitdoglab-ohmimetro-codigo-cores.git

