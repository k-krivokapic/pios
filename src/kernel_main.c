
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

char glbl[128];

void kernel_main() {
//adding a comment
    int timer = get_timer_count();

    extern int __bss_start, __bss_end;
    char *bssstart, *bssend;

    bssstart = &__bss_start;
    bssend = &__bss_end;

    for (char *ptr = bssstart; ptr <= bssend; ptr++) {
        *ptr=0;
    }

    while(1){
    }
  
}

