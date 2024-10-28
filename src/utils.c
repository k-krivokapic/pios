#include "utils.h"

#define UART_BASE_ADDR 0x3F201000

void wait_msec(unsigned int msec) {
    // Implement a simple busy wait for milliseconds
    // Assuming a loop can create delay, though precise timing may vary by CPU
    volatile unsigned int count;
    while (msec > 0) {
        for (count = 0; count < 1000; count++); // Adjust count for more accurate delays if needed
        msec--;
    }
}

void wait_cycles(unsigned int cycles) {
    // A simple busy loop for CPU cycles
    volatile unsigned int count;
    for (count = 0; count < cycles; count++);
}

void uart_send(char c) {
    volatile unsigned int *uart_data_reg = (unsigned int *)(UART_BASE_ADDR + 0x00); // Adjust offset if needed
    *uart_data_reg = c; // Write the character to the UART data register
}

void uart_puts(const char *str) {
    // UART output loop: write each character until null terminator
    while (*str) {
        uart_send(*str++); // Assuming you have a uart_send function that sends one character
    }
}

