#define _DEFAULT_SOURCE_
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#include "./recdir.h"

void visit_files(const char *dir_path){
	DIR *dir = opendir(dir_path);
	if (dir == NULL){
		fprintf(stderr, "ERROR: Could not open directory %s: %s\n",
		dir_path, strerror(errno));
		exit(1);
	}

	errno = 0;
	struct dirent *ent = readdir(dir);
	while(ent != NULL) {
		/*if(ent->d_name[0] == '.'){
			nex;
		}*/
		printf("Processing object: %s\n", ent->d_name);
		if(ent->d_type == DT_DIR){
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
				printf("dir: %s\n", ent->d_name);
				char *child_path = join_path(dir_path, ent->d_name);
				visit_files(child_path);
				free(child_path);
			}
		} else {
			printf("file: %s/%s\n", dir_path, ent->d_name);
		}

		ent = readdir(dir);
	}

	if (errno != 0) {
		fprintf(stderr, "Error could not read directory %s: %s\n",
			dir_path, strerror(errno));
		exit(1);
	}
	closedir(dir);
}
// TODO: recursively traverse the file system
// TODO: compute the hashes of the files
// TODO: build the hash table
int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;
	const char *dir_path = ".";
	visit_files(dir_path);
	return 0;
}
