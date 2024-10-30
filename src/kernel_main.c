#include "serial.h"
#include "mmu.h"
#include "fat.h"
#include "rprintf.h"


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

    // Initialize the filesystem
    int init_status = fatInit();
    if (init_status != 0) {
	esp_printf("Failed to initialize FAT filesystem: %d\n", init_status);
        return;  // Exit if initialization fails
    }
    esp_printf("FAT filesystem initialized successfully: %d\n", init_status);

    // Variables to store file details
    unsigned int start_cluster;
    unsigned int file_size;

    // Open a file, for example "/BIN/BASH"
    int open_status = fatOpen("/BIN/BASH", &start_cluster, &file_size);
    if (open_status != 0) {
	esp_printf("Failed to open /BIN/BASH: : %d\n", open_status);
        return;  // Exit if file opening fails
    }
    esp_printf("File /BIN/BASH opened successfullyStart cluster: %u, File size: %u bytes.\n", start_cluster, file_size);

    // Allocate a buffer to read file data
    unsigned char buffer[file_size];

    // Read the file contents
    int bytes_read = fatRead(start_cluster, buffer, file_size);
    if (bytes_read != file_size) {
	esp_printf("Error: Expected to read %u bytes but only read %d bytes.\n", file_size, bytes_read);
        return;    
    }
    esp_printf("File data read successfully. Total bytes read: %d.\n", bytes_read);

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

