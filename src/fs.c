// In The Name Of God
// ========================================
// * File Name : fs.c
// 
// * Creation Date : 19-12-2014
//
// * Last Modified : Sat 20 Dec 2014 12:33:25 AM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>

#include "FAT.h"

static int fd = -1;

void init_fs(int dev){
	fd = dev;
}

fat_dir_layout_t* find(const char* path){
	int i = 0;
	fprintf(stderr, "%s\n", path);
	for(i = 0; i < 512; i++){
		if(!is_directory(root_dir[i].attr) && root_dir[i].file_size && !is_special(root_dir[i].attr)){
			char dis_name[255];
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
			
			if(!strcmp(dis_name, path))
				return &root_dir[i];
			
		}else if(is_directory(root_dir[i].attr) && root_dir[i].file_size){
		}
	}
	return NULL;
}

fat_dir_layout_t* parse_dir(fat_dir_layout_t dir, int* dir_size){
	*dir_size = dir.file_size / 32;
	if(!is_directory(dir.attr))
		return NULL;
	
	fat_dir_layout_t* entries = malloc(sizeof(fat_dir_layout_t) * (*dir_size));
	
	unsigned int size = dir.file_size;
	fat_addr_t cluster = dir.first_cluster;
	int index = 0;
	while(cluster){
		if(size < fat_boot.sectors_per_cluster * 512){
			lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, entries + index, size);
		}else{
			lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
			read(fd, entries + index, 512 * fat_boot.sectors_per_cluster);
			size -= 512 * fat_boot.sectors_per_cluster;
			index += 512 * fat_boot.sectors_per_cluster;
		}
		cluster = next_cluster(cluster);
	}

	return entries;
}
