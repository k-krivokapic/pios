
unsigned long get_timer_count() {
    unsigned long *timer_count_register = 0x3f003004;
    return *timer_count_register;
}

void wait_lms() {
    unsigned long start_time = get_timer_count();
    unsigned long end_timer = start_time + 1000;
    while (get_timer_count() < end_time) {
        //waits for 1ms
    }
}

void kernel_main() {
//adding a comment
    int timer = get_timer_count();
    while(1){
    }
  
}

