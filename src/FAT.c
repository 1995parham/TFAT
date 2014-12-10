// In The Name Of God
// ========================================
// * File Name : FAT.c
// 
// * Creation Date : 08-12-2014
//
// * Last Modified : Wed 10 Dec 2014 06:23:26 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include "FAT.h"

#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>


fat_BS_t fat_boot;
fat_addr_t* fat_table;
fat_addr_t* fat_table_bak;

void init_fat(int fd){
	read(fd, &fat_boot, 512);

	fat_table = (fat_addr_t*)malloc(sizeof(fat_addr_t) * 256 * fat_boot.table_size_16);
	read(fd, fat_table, 512 * fat_boot.table_size_16);
	
	fat_table_bak = (fat_addr_t*)malloc(sizeof(fat_addr_t) * 256 * fat_boot.table_size_16);
	read(fd, fat_table_bak, 512 * fat_boot.table_size_16);

}

fat_addr_t next_cluster(fat_addr_t index){
	if(fat_table[index] && 0xF000 == 0xF000)
		return 0x0000;
	if(fat_table[index] == 0x0000)
		return 0x0000;
	return fat_table[index];
}

fat_addr_t root_dir_sectors(){ 
	return ((fat_boot.root_entry_count * 32) + (fat_boot.bytes_per_sector - 1)) / fat_boot.bytes_per_sector;
}
fat_addr_t first_fat_sector(){
	return  fat_boot.reserved_sector_count;
}
fat_addr_t first_data_sector(){
	return fat_boot.reserved_sector_count + (fat_boot.table_count * fat_boot.table_size_16);
}
fat_addr_t data_sectors(){
	return fat_boot.total_sectors_16 - (fat_boot.reserved_sector_count + (fat_boot.table_count * fat_boot.table_size_16) + root_dir_sectors(fat_boot));
}
fat_addr_t total_clusters(){
	return (data_sectors(fat_boot) / fat_boot.sectors_per_cluster);
}
