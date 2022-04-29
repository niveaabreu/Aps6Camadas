
#include "sw_uart.h"

due_sw_uart uart;

char string[]="Camada Física da Computação\n";
int i =0;

void setup() {
  Serial.begin(9600);
  sw_uart_setup(&uart, 2, 1, 8, SW_UART_EVEN_PARITY);
}

void loop() {
 envia_byte(string[i]);
 
 if (i<32){
  i++;
 }
 else{
   i=0;
 }
  delay(500);
}


void envia_byte(char letra) {
  sw_uart_write_byte(&uart, letra);
  Serial.print("Enviando o caractere: ");
  Serial.print(letra);
  Serial.println();
}
