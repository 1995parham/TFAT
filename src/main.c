// In The Name Of God
// ========================================
// * File Name : main.c
// 
// * Creation Date : 08-12-2014
//
// * Last Modified : Wed 10 Dec 2014 11:35:51 AM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "FAT.h"

int main(int argc, char* argv[]){
	int fd = open("/dev/loop0", O_RDONLY); 
	
	// Cluster 0, sector 0

	fat_BS_t fat;
	read(fd, &fat, 512);
	printf("Boot Jump: %2X %2X %2X\n", fat.bootjmp[0], fat.bootjmp[1], fat.bootjmp[2]);
	printf("OEM Media: %8s\n", fat.oem_name);
	printf("Media Type: %2X\n", fat.media_type);
	printf("Sectors Per Track: %hu\n", fat.sectors_per_track);
	printf("Sectors Per Cluster: %hu\n", fat.sectors_per_cluster);
	printf("Root Entry Count: %hu\n", fat.root_entry_count);
	printf("Reserved Sectors: %hu\n", fat.reserved_sector_count);
	printf("FAT Tables: %hhu\n", fat.table_count); 
	printf("Table Size: %hu\n", fat.table_size_16);

	fat_addr_t root_cluster = first_data_sector(&fat);

	printf("Root Cluster Sector: %hu\n", root_cluster);
	
	// Cluster 0, Sector 1

	fat_addr_t fat_table[256 * fat.table_size_16];
	read(fd, &fat_table, 512 * fat.table_size_16);
	int i;
	for(i = 0; i < 256 * fat.table_size_16; i++){
		printf("%d : %04X\n", i, fat_table[i]);
	}
	read(fd, &fat_table, 512 * fat.table_size_16);
	for(i = 0; i < 256 * fat.table_size_16; i++){
		printf("%d : %04X\n", i, fat_table[i]);
	}

	// Cluster 0, sector 3

	close(fd);

	return 0;
}
