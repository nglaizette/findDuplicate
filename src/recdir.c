#include <assert.h>
#include <string.h>
#include <errno.h>
#include "./recdir.h"

char *join_path(const char *base, const char *file){
	size_t base_len = strlen(base);
	size_t file_len = strlen(file);

	// TODO: Avoid using malloc here
	char *begin = malloc(base_len + file_len + PATH_SEP_LEN+ 1);
	assert(begin != NULL);
	
	char *end = begin;
	memcpy(end, base, base_len);
	end += base_len;
	memcpy(end, PATH_SEP, PATH_SEP_LEN);
	end += PATH_SEP_LEN;
	memcpy(end, file, file_len);
	end += file_len;
	*end = '\0';

	return begin;
}

int recdir_push(RECDIR * recdir, char *path){
	
	assert(recdir->stack_size < RECDIR_STACK_CAP);
	RECDIR_Frame *top = &recdir->stack[recdir->stack_size - 1];
	top->path = path;
	top->dir = opendir(top->path);
	if(top->dir == NULL){
		free(top->path);
		return -1;
	}
	recdir->stack_size++;
	return 0;
}

void recdir_pop(RECDIR * recdir){
	assert(recdir->stack_size > 0);
	RECDIR_Frame *top = &recdir->stack[--recdir->stack_size];
	int ret = closedir(top->dir);
	assert(ret == 0);
	free(top->path);
}

RECDIR *openrecdir(const char *dir_path){
	RECDIR *recdir = malloc(sizeof(RECDIR));
	assert(recdir != NULL);
	memset(recdir, 0, sizeof(RECDIR));

	/*static_assert(DIRS_CAP > 0, "");
	recdir->dirs[recdir->stack_size] = opendir(dir_path);
	if(recdir->dirs[recdir->stack_size] == NULL){
		free(recdir);
		return NULL;
	}
	recdir->stack_size = 1;*/

	if(recdir_push(recdir, strdup(dir_path)) < 0){
		free(recdir);
		return NULL;
	}
	return recdir;
}

struct dirent *readrecdir(RECDIR *recdirp){	
	while(recdirp->stack_size > 0){
		RECDIR_Frame *top = &recdirp->stack[recdirp->stack_size-1];
		
		errno = 0;
		struct dirent *entry = readdir(top->dir);
		if(entry){
			if(entry->d_type == DT_DIR){
				if (strcmp(entry->d_name, ".") == 0 && strcmp(entry->d_name, "..") == 0){
					continue;
				} else {
					recdir_push(recdirp, join_path(top->path, entry->d_name));
					continue;
				}
			} else {
				return entry;
			}
		} else {
			if(errno){
				return NULL;
			} else {
				// TODO: pop directory
				recdir_pop(recdirp);
				continue;
			}
		}
	}
	return NULL;
}

void closerecdir(RECDIR *recdirp){
	while (recdirp->stack_size > 0){
		recdir_pop(recdirp);
	}
	free(recdirp);
}
