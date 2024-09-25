#include <stdio.h>
//#include <stdint.h>
//#include <string.h>
#include "rprintf.h"



#define MU_IO *((volatile unsigned int*)0x3F215040)


void serial_putc(int data) {

 //   while ((MU_IO & (1 << 5)) != 0) {
        
   // }

    // write the character to the data register
  MU_IO = data;

}


//int main() {
//    esp_printf(serial_putc, "Current Execution Level is %d\r\n", getEL());

  //  return 0;
//}
