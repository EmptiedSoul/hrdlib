#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../libhrd.h"

bool hrd_file_exist(char* filename, int flags){
	struct stat file_stats;
	int retval = stat(filename, &file_stats);
	if (retval != 0) 
		return false;
	if (flags == 0 )
		return true;
	if (flags & HRDFS_ISDIR){
		if (!S_ISDIR(file_stats.st_mode))
			goto failed;
	}
	if (flags & HRDFS_ISFILE){
		if (!S_ISREG(file_stats.st_mode))
			goto failed;
	}
	if (flags & HRDFS_READBL){
		if (file_stats.st_uid == getuid()){
			if (!(S_IRUSR & file_stats.st_mode)) goto failed; 
		} else if (file_stats.st_gid == getgid()){
			if (!(S_IRGRP & file_stats.st_mode)) goto failed;
		} else {
			if (!(S_IROTH & file_stats.st_mode)) goto failed;
		}
	}
	if (flags & HRDFS_WRITBL){
		if (file_stats.st_uid == getuid()){
			if (!(S_IWUSR & file_stats.st_mode)) goto failed; 
		} else if (file_stats.st_gid == getgid()){
			if (!(S_IWGRP & file_stats.st_mode)) goto failed;
		} else {
			if (!(S_IWOTH & file_stats.st_mode)) goto failed;
		}
	}
	if (flags & HRDFS_EXECBL){
		if (file_stats.st_uid == getuid()){
			if (!(S_IXUSR & file_stats.st_mode)) goto failed; 
		} else if (file_stats.st_gid == getgid()){
			if (!(S_IXGRP & file_stats.st_mode)) goto failed;
		} else {
			if (!(S_IXOTH & file_stats.st_mode)) goto failed;
		}
	}	
	return true;
failed:
	return false;
}

