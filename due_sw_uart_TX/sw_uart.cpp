
  
#include "sw_uart.h"
#pragma GCC optimize ("-O3")

void sw_uart_setup(due_sw_uart *uart, int tx, int stopbits, int databits, int paritybit) {
	
	uart->pin_tx     = tx;
	uart->stopbits   = stopbits;
	uart->paritybit  = paritybit;
  uart->databits   = databits;
  pinMode(tx, OUTPUT);
  
}


//Atribui ao pointer de bit o valor de paridade do char enviado
void calc_even_parity(int *bit,char data) {
  int ones = 0;
  for(int i = 0; i < 8; i++) {
    ones += (data >> i) & 0x01;
  }
  *bit = ones % 2;
}


void sw_uart_write_byte(due_sw_uart *uart, char data){
  //Atribui a paridade ao bit do caractere enviado
  int bit;
  calc_even_parity(&bit, data);

  //mantém estado não receptivo
  digitalWrite(uart->pin_tx, HIGH);
  //envia bit de start para começo da leitura
  digitalWrite(uart->pin_tx, LOW);
  _sw_uart_wait_T(uart);

  //Envia cada bit do caractere em ASCII na ordem "little" para leitura em "big"
  for (int i=0; i<8; i++){
    //Enviamos cada bit dos 8 possíveis, usando shift para "deslizar entre eles", assegurando
    //que seja um e apenas um, enviado de cada vez
    //Serial.print((data >> i) & 0x01);
    digitalWrite(uart->pin_tx,(data >> i) & 0x01);
    _sw_uart_wait_T(uart);  
  }
  Serial.println();
  //envia o bit de paridade para o pino RX
  digitalWrite(uart->pin_tx,bit);
  _sw_uart_wait_T(uart);

  //envia o bit de parada
  digitalWrite(uart->pin_tx, HIGH);
  _sw_uart_wait_T(uart);
}


// MCK 21MHz
void _sw_uart_wait_half_T(due_sw_uart *uart) {
  for(int i = 0; i < 1093; i++)
    asm("NOP");
}

void _sw_uart_wait_T(due_sw_uart *uart) {
  _sw_uart_wait_half_T(uart);
  _sw_uart_wait_half_T(uart);
}
