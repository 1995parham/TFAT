// In The Name Of God
// ========================================
// * File Name : fs.c
// 
// * Creation Date : 19-12-2014
//
// * Last Modified : Sat 20 Dec 2014 02:24:00 AM IRST
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
	for(i = 0; i < fat_boot.root_entry_count; i++){
		if(root_dir[i].file_size && !is_special(root_dir[i].attr)){
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
			
		}if(is_directory(root_dir[i].attr)){
			char dis_name[255];
			// Store file name
			char* temp = get_name(root_dir[i].name);
			if(temp){
				strcpy(dis_name, temp);
				free(temp);
			}else{
				continue;
			}
			
			if(!strcmp(dis_name, path))
				return &root_dir[i];
		}
	}
	return NULL;
}

// TODO add directory chain handling with realoc
fat_dir_layout_t* parse_dir(fat_dir_layout_t dir, int* dir_size){
	if(!is_directory(dir.attr))
		return NULL;
	
	fat_dir_layout_t* entries = malloc(512 * fat_boot.sectors_per_cluster);
	
	int index = 0;
	fat_addr_t cluster = dir.first_cluster;
	while(cluster){
		lseek(fd, 512 * fat_boot.sectors_per_cluster * (cluster - 2) + data_offset, SEEK_SET);
		read(fd, entries + index, 512 * fat_boot.sectors_per_cluster);
		*dir_size += (512 * fat_boot.sectors_per_cluster) / sizeof(fat_dir_layout_t);
		index += 512 * fat_boot.sectors_per_cluster;
		cluster = next_cluster(cluster);
	}

	return entries;
}
