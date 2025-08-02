#define _DEFAULT_SOURCE_
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#define PATH_SEP  "/"
#define PATH_SEP_LEN (sizeof(PATH_SEP) - 1)

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

# define DIRS_CAP 1024

typedef struct {
	DIR *dirs[DIRS_CAP];
	size_t dirs_size;
} RECDIR;

int recdir_push(RECDIR * recdir, const char *dir_path){
	
	assert(recdir->dirs_size < DIRS_CAP);
	DIR **dir = &recdir->dirs[recdir->dirs_size];
	*dir = opendir(dir_path);
	if(*dir == NULL){
		return -1;
	}
	recdir->dirs_size++;
	return 0;
}

RECDIR *openrecdir(const char *dir_path){
	RECDIR *recdir = malloc(sizeof(RECDIR));
	assert(recdir != NULL);
	memset(recdir, 0, sizeof(RECDIR));

	/*static_assert(DIRS_CAP > 0, "");
	recdir->dirs[recdir->dirs_size] = opendir(dir_path);
	if(recdir->dirs[recdir->dirs_size] == NULL){
		free(recdir);
		return NULL;
	}
	recdir->dirs_size = 1;*/

	if(recdir_push(recdir, dir_path) < 0){
		free(recdir);
		return NULL;
	}
	return recdir;
}

struct dirent *readrecdir(RECDIR *recdirp){
	(void) recdirp;
	return NULL;
}

void closerecdir(RECDIR *recdirp){
	for (size_t i = 0; i < recdirp->dirs_size; ++i){
		int ret = closedir(recdirp->dirs[i]);
		assert(ret == 0);
	}
	free(recdirp);
}

/*{
	RECDIR *dir = openrecdir(".");
	stuct dirent *end = readrecdir(dir);
	while (ent != NULL){
		file_action(ent->d_name);
	}
	closerecdir(dir);
}*/

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
