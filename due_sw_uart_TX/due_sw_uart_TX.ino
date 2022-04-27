
#include "sw_uart.h"

due_sw_uart uart;

void setup() {
  Serial.begin(9600);
  sw_uart_setup(&uart, 2, 1, 8, SW_UART_EVEN_PARITY);
}

void loop() {
 envia_byte();
 delay(500);
}


void envia_byte() {
  sw_uart_write_byte(&uart, 'm');
  Serial.println("Enviando o caractere 'm'");
}
