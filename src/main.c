// In The Name Of God
// ========================================
// * File Name : main.c
// 
// * Creation Date : 08-12-2014
//
// * Last Modified : Wed 17 Dec 2014 12:44:39 AM IRST
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
	init_fat(fd);
	
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

	fat_addr_t root_cluster = first_data_sector();

	printf("Root Cluster Sector: %hu\n", root_cluster);

	// Now you are at the start of root directory structure
	int i = 0;
	for(i = 0; i < 512; i++){
		if(!is_directory(root_dir[i].attr) && root_dir[i].file_size){
			char dis_name[255];
			char dis_time[255];
			int j = 0;
			int index = 0;
			// Store file name
			dis_name[index] = 0;
			for(j = 0; j < 8; j++){
				if(root_dir[i].name[j] == 0 || root_dir[i].name[j] == ' ' || root_dir[i].name[j] == 0xE5)
					break;
				dis_name[index] = root_dir[i].name[j];
				index++;
			}
			dis_name[index] = '.';
			index++;
			for(j = 0; j < 3; j++){
				if(root_dir[i].extention[j] == 0 || root_dir[i].name[j] == ' ')
					break;
				dis_name[index] = root_dir[i].extention[j];
				index++;
			}	
			dis_name[index] = 0;	
			//fat_addr_t cluster = root_dir[i].first_cluster;
			//while(cluster){
			//	uint8_t buff[512 * fat_boot.sectors_per_cluster];
			//	int ret = lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + sk, SEEK_SET);
			//	printf("%d\n", ret);
			//	read(fd, buff, 512 * fat_boot.sectors_per_cluster);
			//	printf("%hu\n", cluster);
			//	int j = 0;
			//	for(j = 0; j < 512 * fat_boot.sectors_per_cluster; j++)
			//		printf("%c", buff[j]);
			//	cluster = next_cluster(cluster);
			//}
			struct tm file_tm = create_time(root_dir[i].create_time, root_dir[i].create_date);
			strftime(dis_time, 255, "%b %d %T %Y", &file_tm);
			printf("%s %4u %s\n", dis_time, root_dir[i].file_size, dis_name);
		}
	}
	
	close(fd);

	return 0;
}
