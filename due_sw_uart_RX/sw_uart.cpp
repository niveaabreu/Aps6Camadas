#include "sw_uart.h"
#pragma GCC optimize ("-O3")

void sw_uart_setup(due_sw_uart *uart, int rx, int stopbits, int databits, int paritybit) {
	
	uart->pin_rx     = rx;
	uart->stopbits   = stopbits;
	uart->paritybit  = paritybit;
  uart->databits   = databits;
  pinMode(rx, INPUT);
  
}


int calc_even_parity(char data) {
  int ones = 0;

  for(int i = 0; i < 8; i++) {
    ones += (data >> i) & 0x01;
  }

  return ones % 2;
}

int sw_uart_receive_byte(due_sw_uart *uart, char* data) {
  // Fica em while true até receber o start bit
  while(digitalRead(uart->pin_rx) == HIGH)
  {
    //Serial.println("esperando byte");
  }

  //Quando recebe o start bit, da meio periodo de intervalo
  // confirm start bit
  _sw_uart_wait_half_T(uart);
  // HIGH = invalid
	
// Confere se não houve erro na chegada do bit
  if(digitalRead(uart->pin_rx) == HIGH) {
    return SW_UART_ERROR_FRAMING;
  }

// Mais um intervalo de período T para recepção de dados
  _sw_uart_wait_T(uart);
  
  // Inicia recebimento de dados via mascaramento, e a cada bit recebido, da um intervalo T (_sw_uart_wait_T)
  char aux = 0x00;
  for(int i = 0; i < uart->databits; i++) {
	  // Lê o bit
    aux |= digitalRead(uart->pin_rx) << i;
	  //Intervalo T
    _sw_uart_wait_T(uart);
  }
  
  // Lê o bit de paridade
  int rx_parity = 0;
  if(uart->paritybit != SW_UART_NO_PARITY) {
    rx_parity = digitalRead(uart->pin_rx);
	  // Da um intervalo T após a leitura do bit de paridade
    _sw_uart_wait_T(uart);
  }

  // Recebe o stop bit
  for(int i = 0; i < uart->stopbits; i++) {
	  // Confere se nao houve erro do stopbit
    if(digitalRead(uart->pin_rx) == LOW) {
      return SW_UART_ERROR_FRAMING;
    }
	  / Da um intervalo T após o stop bit
    _sw_uart_wait_T(uart);
  }
  
  int parity = 0;
	// Confere se a paridade está coerente com o byte recebido
  if(uart->paritybit == SW_UART_EVEN_PARITY) {
     parity = calc_even_parity(aux);
  } else if(uart->paritybit == SW_UART_ODD_PARITY) {
     parity = !calc_even_parity(aux);
  }
// Erro de paridade
  if(parity != rx_parity) {
    return SW_UART_ERROR_PARITY;
  }
  // Mostra dados corretamente recebidos
  *data = aux;
  return SW_UART_SUCCESS;
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
