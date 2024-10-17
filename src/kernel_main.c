#include "serial.h"
#include "mmu.h"


extern struct table_descriptor_stage1 L1table[512];


unsigned long get_timer_count() {
    unsigned long *timer_count_register = 0x3f003004;
    return *timer_count_register;
}

void wait_lms() {
    unsigned long start_time = get_timer_count();
    unsigned long end_timer = start_time + 1000;
    while (get_timer_count() < end_timer) {
        //waits for 1ms
    }
}

int getEL() {
    unsigned int el;
    asm("mrs %0,CurrentEL"
    : "=r"(el)
    :
    :);
    return el>>2;
}

char glbl[128];

void kernel_main() {
//adding a comment

    int timer = get_timer_count();

    extern int __bss_start, __bss_end;
    char *bssstart, *bssend;

    mapPages((void*)0x0, (void*)0x0);
    int result = loadPageTable(L1table);

    esp_printf(serial_putc, "Current Execution Level is %d\r\n", getEL());

    bssstart = &__bss_start;
    bssend = &__bss_end;

    for (char *ptr = bssstart; ptr <= bssend; ptr++) {
        *ptr=0;
    }

    while(1){
    }
  
}

