#include <stdio.h>
#include "rprintf.h"

#define MU_IO *((volatile unsigned int*)0x3F215040)


void serial_putc(int data) {

  MU_IO = data;

}


