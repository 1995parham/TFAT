// In The Name Of God
// ========================================
// * File Name : user.c
// 
// * Creation Date : 08-12-2014
//
// * Last Modified : Fri 19 Dec 2014 09:19:15 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>

#include "FAT.h"
#include "common.h"
#include "fs.h"
#include "user.h"

static int fd;

void mount(const char* dev){
	fd = open(dev, O_RDONLY); 
	
	if(fd <= 0)
		die("cannot open %s", dev);

	init_fat(fd);	

	info();
}


void info(){
	printf("Boot Jump: %2X %2X %2X\n", fat_boot.bootjmp[0], fat_boot.bootjmp[1], fat_boot.bootjmp[2]);
	printf("OEM Media: %8s\n", fat_boot.oem_name);
	printf("Media Type: %2X\n", fat_boot.media_type);
	printf("Bytes Per Sector: %hu\n", fat_boot.bytes_per_sector);
	printf("Sectors Per Track: %hu\n", fat_boot.sectors_per_track);
	printf("Sectors Per Cluster: %hu\n", fat_boot.sectors_per_cluster);
	printf("Root Entry Count: %hu\n", fat_boot.root_entry_count);
	printf("Reserved Sectors: %hu\n", fat_boot.reserved_sector_count);
	printf("FAT Tables: %hhu\n", fat_boot.table_count); 
	printf("Table Size: %hu\n", fat_boot.table_size_16);
	printf("Total Sectors: %hu\n", fat_boot.total_sectors_16); 

	fat_addr_t root_cluster = first_data_sector();

	printf("Root Cluster Sector: %hu\n", root_cluster);

}

void ls(const char* dir){
	if(fd == 0)
		die("Please open valid device first");

	int i = 0;
	for(i = 0; i < 512; i++){
		if(!is_directory(root_dir[i].attr) && root_dir[i].file_size && !is_special(root_dir[i].attr)){
			char dis_name[255];
			char dis_time[255];
			// Store file name
			char* temp = get_name(root_dir[i].name);
			if(temp){
				strcpy(dis_name, temp);
				free(temp);
			}else{
				continue;
			}
			dis_name[strlen(dis_name) + 1] = 0;
			dis_name[strlen(dis_name)] = '.';
			temp = get_extention(root_dir[i].extention);
			strcpy(dis_name + strlen(dis_name), temp);
			free(temp);
			
			struct tm file_tm = create_time(root_dir[i].create_time, root_dir[i].create_date);
			strftime(dis_time, 255, "%b %d %T %Y", &file_tm);
			printf("%s %4u %12s %hu\n", dis_time, root_dir[i].file_size, dis_name, root_dir[i].first_cluster);
		}else if(is_directory(root_dir[i].attr) && root_dir[i].file_size){
		}
	}
}

void hdump(const char* dir){
	if(fd == 0)
		die("Please open valid device first");

	fat_dir_layout_t* file = find(dir);
	if(file == NULL){
		printf("File %s Not Found\n", dir);
		return;
	}
	fat_addr_t cluster = file->first_cluster;
	unsigned int size = file->file_size;
	printf("File: %s, Size: %d\n",dir, size);
	printf("================####====================\n");
	while(cluster){
		if(size < fat_boot.sectors_per_cluster * 512){
			uint8_t buff[size];
			lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, size);
			int j = 0;
			for(j = 0; j < size; j++)
				printf("%02X", buff[j]);
		}else{
			uint8_t buff[512 * fat_boot.sectors_per_cluster];
			lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, 512 * fat_boot.sectors_per_cluster);
			int j = 0;
			for(j = 0; j < 512 * fat_boot.sectors_per_cluster; j++)
				printf("%02X", buff[j]);
			size -= 512 * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}
	printf("\n========================================\n");
}

void dump(const char* dir){
	if(fd == 0)
		die("Please open valid device first");

	fat_dir_layout_t* file = find(dir);
	if(file == NULL){
		printf("File %s Not Found\n", dir);
		return;
	}
	fat_addr_t cluster = file->first_cluster;
	unsigned int size = file->file_size;
	printf("File: %s, Size: %d\n",dir, size);
	printf("================####====================\n");
	while(cluster){
		if(size < fat_boot.sectors_per_cluster * 512){
			uint8_t buff[size];
			lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, size);
			int j = 0;
			for(j = 0; j < size; j++)
				printf("%c", buff[j]);
		}else{
			uint8_t buff[512 * fat_boot.sectors_per_cluster];
			lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, buff, 512 * fat_boot.sectors_per_cluster);
			int j = 0;
			for(j = 0; j < 512 * fat_boot.sectors_per_cluster; j++)
				printf("%c", buff[j]);
			size -= 512 * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}
	printf("\n========================================\n");
}

void chain(fat_addr_t cluster){
	if(fd == 0)
		die("Please open valid device first");

	while(cluster){
		if(next_cluster(cluster) != 0)
			printf("%hu -> ", cluster);
		else
			printf("%hu\n", cluster);
		cluster = next_cluster(cluster);
	}
}

void fat(){
	if(fd == 0)
		die("Please open valid device first");

	fat_addr_t i = 0;
	for(i = 0; i < fat_boot.table_size_16 * 256; i++){
		if(i % 10 == 0){
			printf("Do you want to continue ?(Y/n)");
			char c = getchar();
			getchar();
			if(c != 'Y')
				return; 
		}
		printf("%hX <--> %hX\n", i, fat_table[i]);
	}
}


void dump_fat(const char* path){
	if(fd == 0)
		die("Please open valid device first");
	int file = open(path, O_WRONLY | O_CREAT, 0644);
	if(file <= 0)
		die("cannot open %s", path);	
	write(file, fat_table, 512 * fat_boot.table_size_16);
	close(file);
}

void umount(){
	if(fd > 0)
		close(fd);
}
