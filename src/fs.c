// In The Name Of God
// ========================================
// * File Name : fs.c
// 
// * Creation Date : 19-12-2014
//
// * Last Modified : Fri 19 Dec 2014 08:51:37 PM IRST
//
// * Created By : Parham Alvani (parham.alvani@gmail.com)
// =======================================
#include <string.h>
#include <malloc.h>

#include "FAT.h"

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
