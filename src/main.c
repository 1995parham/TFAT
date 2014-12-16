// In The Name Of God
// ========================================
// * File Name : main.c
// 
// * Creation Date : 08-12-2014
//
// * Last Modified : Wed 17 Dec 2014 12:06:26 AM IRST
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
	fat_dir_layout_t root_dir[512]; 
	read(fd, &root_dir, 512 * 32);
	//int sk = lseek(fd, 0, SEEK_CUR);
	for(i = 0; i < 512; i++){
		if(!is_directory(root_dir[i].attr) && root_dir[i].file_size){
			int j = 0;
			// Print file name
			for(j = 0; j < 8; j++){
				if(root_dir[i].name[j] == 0 || root_dir[i].name[j] == ' ' || root_dir[i].name[j] == 0xE5)
					break;
				fputc(root_dir[i].name[j], stdout);
			}
			fputc('.', stdout);
			// Print file extention
			for(j = 0; j < 3; j++){
				if(root_dir[i].extention[j] == 0 || root_dir[i].name[j] == ' ')
					break;
				fputc(root_dir[i].extention[j], stdout);
			}	
			fputc('\t', stdout);

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
			printf("\nFirst Cluster: %hu\n", root_dir[i].first_cluster);
			printf("File Size: %u\n", root_dir[i].file_size);
			struct tm file_tm = create_time(root_dir[i].create_time, root_dir[i].create_date);
			printf("Create Time: %s\n", asctime(&file_tm));
			printf("------------------------------------------------\n");
		}
	}
	
	close(fd);

	return 0;
}
