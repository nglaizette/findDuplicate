#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
// TODO: recursively traverse the file system
// TODO: compute the hashes of the files
// TODO: build the hash table
int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;
	const char *dir_path = ".";
	DIR *dir = opendir(dir_path);
	if (dir == NULL){
		fprintf(stderr, "ERROR: Could not open directory %s: %s\n",
		dir_path, strerror(errno));
		exit(1);
	}

	errno = 0;
	struct dirent *ent = readdir(dir);
	while(ent != NULL) {
		printf("Processing object: %s\n", ent->d_name);
		if(ent->d_type == DT_DIR){
			printf("dir: %s\n", ent->d_name);
		} else {
			printf("file: %s\n", ent->d_name);
		}

		ent = readdir(dir);
	}

	if (errno != 0) {
		fprintf(stderr, "Error could not read directory %s: %s\n",
			dir_path, strerror(errno));
		exit(1);
	}
	closedir(dir);
	return 0;
}
