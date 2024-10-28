#include "sd.h"
#include <stdio.h>
//#include "rprintf.h"
#include "fat.h"

#define SECTOR_SIZE 512
#define FAT_TYPE "FAT12"
#define DIRECTORY_ATTRIBUTE 0x10  // FAT attribute bitmask for directory


struct boot_sector *bs;
char bootSector[SECTOR_SIZE];     // Allocate a global array to store boot sector
char fat_table[8 * SECTOR_SIZE];  // FAT table
unsigned int root_sector;

unsigned int sectors_per_cluster;
unsigned int bytes_per_sector;
unsigned int root_sector;


// Structure for directory entries, assuming 32 bytes per entry (typical for FAT16/FAT32)
struct dir_entry {
    char name[11];         // Filename in 8.3 format
    uint8_t attributes;    // File attributes
    uint8_t reserved[10];  // Reserved bytes
    uint16_t first_cluster; // Starting cluster of the file
    uint32_t size;         // File size in bytes
} __attribute__((packed));


int fatInit() {
    // Read boot sector (sector 0) into memory
    sd_readblock(0, bootSector, 1);    // Read sector 0 from disk into bootSector array
    bs = (struct boot_sector *)bootSector;    // Point struct to bootSector for easier access to fields

    // Validate boot signature
    if (bs->boot_signature != 0xAA55) {
        return -1;  // Invalid boot signature
    }

    // validate file system type (FAT12)
    int pios_strncmp(const char *str1, const char *str2, size_t n) {
    while (n-- > 0) {
        // reached the end of either string or they differ
        if (*str1 != *str2) {
            return (*(unsigned char *)str1 - *(unsigned char *)str2);
        }
        // both characters are the same, move to the next characters
        if (*str1 == '\0') {
            return 0; // reached end of str1
        }
        str1++;
        str2++;
    }
    return 0; // first n characters are equal
}

    if (pios_strncmp(bs->fs_type, FAT_TYPE, 5) != 0) {
        return -1;  // invalid file system type
    }

    // read the FAT table into memory
    int fat_start_sector = bs->num_reserved_sectors;
    sd_readblock(fat_start_sector, fat_table, 8);    // Assuming FAT size fits in 8 sectors

    // compute root sector
    root_sector = bs->num_fat_tables * bs->num_sectors_per_fat + bs->num_reserved_sectors + bs->num_hidden_sectors;

    return 0;    // Successful initialization
}

// supporting method for fatOpen
int compare_names(const char *name1, const char *name2, int length) {
    for (int i = 0; i < length; i++) {
        if (name1[i] != name2[i]) {
            return 0; // Names don't match
        }
    }
    return 1; // Names match
}

int fatOpen(const char *filepath) {
    struct dir_entry rde;
    int root_dir_sector = root_sector; // Start of root directory region
    int entries_per_sector = SECTOR_SIZE / sizeof(struct dir_entry);
    char buffer[SECTOR_SIZE];

    // Assume we have parsed "target_name" from "filepath" as "/BIN/BASH"
    char target_name[11] = { 'B', 'I', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; // Example name

    for (int sector = root_dir_sector; /* condition to end loop */; sector++) {
        // Read each sector in the root directory
        sd_readblock(sector, buffer, 1);  // Read a sector into buffer (512 bytes)

        for (int i = 0; i < entries_per_sector; i++) {
            // Copy the current directory entry from the buffer
            for (int j = 0; j < sizeof(struct dir_entry); j++) {
                ((char *)&rde)[j] = buffer[i * sizeof(struct dir_entry) + j];
            }

            // Compare entry name with target_name
            if (compare_names(rde.name, target_name, 11)) { // Filename match
                // Check if it's a directory or file
                if (rde.attributes & DIRECTORY_ATTRIBUTE) {
                    // Handle as a directory
                } else {
                    // File found, store details
                    // Set the first_cluster, size, etc., for later use in fatRead
                    return 0; // Success
                }
            }
        }
    }
    return -1; // File not found
}

// supporting method for fatRead()
void readBootSector() {
    unsigned char bootSector[SECTOR_SIZE];
    sd_readblock(0, bootSector, 1); // Assuming sector 0 has the boot sector

    // Bytes per sector (offset 11–12)
    bytes_per_sector = bootSector[11] | (bootSector[12] << 8);

    // Sectors per cluster (offset 13)
    sectors_per_cluster = bootSector[13];

    // Root directory sector (calculate based on reserved sectors, FAT size, etc.)
    unsigned int reserved_sectors = bootSector[14] | (bootSector[15] << 8);
    unsigned int num_fats = bootSector[16];
    unsigned int fat_size_sectors = bootSector[22] | (bootSector[23] << 8);
    root_sector = reserved_sectors + (num_fats * fat_size_sectors);
}

int fatRead(unsigned int start_cluster, unsigned char *buffer, unsigned int size) {
    unsigned int current_cluster = start_cluster;
    unsigned int bytes_read = 0;
    unsigned int sector;

    while (bytes_read < size) {
        // Calculate the sector number for the current cluster
        sector = root_sector + (current_cluster - 2) * sectors_per_cluster;

        // Read each sector in the cluster
        for (int i = 0; i < sectors_per_cluster; i++) {
            // Adjust buffer offset based on bytes read
            sd_readblock(sector + i, buffer + bytes_read, 1);
            bytes_read += bytes_per_sector;

            // Check if we’ve reached or exceeded requested size
            if (bytes_read >= size) {
                return bytes_read;  // Return bytes actually read
            }
        }

        // Move to the next cluster in the chain
        current_cluster = fat_table[current_cluster];

        // Check for end of file in FAT table (assuming 0xFFF is the EOF marker for FAT12)
        if (current_cluster >= 0xFFF) {
            break;  // End of file reached
        }
    }

    return bytes_read;  // Return the total bytes read
}

