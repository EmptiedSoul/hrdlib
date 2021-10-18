#ifndef LIBHRD_H
#define LIBHRD_H

#include <stdbool.h>
#include <stdio.h>

/* fs */
enum {
	HRDFS_ISDIR 	= 1,							/* is a directory			*/
	HRDFS_ISFILE 	= 2,							/* is a regular file			*/
	HRDFS_READBL	= 4,							/* is readable				*/
	HRDFS_WRITBL 	= 8,							/* is writable				*/
	HRDFS_EXECBL 	= 16,							/* is executable			*/
};

extern bool hrd_file_exist(char* filename, int flags); 				/* is file exist 			*/

/* cache */
extern int hrd_cache_set_dir(char* filename);					/* Set cache file directory 		*/
extern int hrd_cache_data(char* filename, void* data, size_t size);		/* Serialize data			*/
extern size_t hrd_uncache_get_size(char* filename);				/* Get size of cached data		*/
extern int hrd_uncache_get_data(char* filename, void* dest, size_t size);	/* Uncache data				*/
extern void* hrd_cache_edit_data(char* filename, size_t size);			/* Edit cached data			*/
extern int hrd_cache_edit_end(void* addr, size_t size);				/* End editing cached data		*/


/* config */
extern char* hrd_cfg_get_string_at(char* filename, char* key);
extern char* hrd_cfg_get_string(FILE* config, char* key);			/* Get value for key in cfg file	*/



/* strings */
extern size_t hrd_string_discard_chars(char* string, char to_discard);		/* discard chars from string		*/
extern char** hrd_string_split(char* string, char* delimiters);			/* split string				*/
extern void   hrd_array_free(void** array);					/* free array (splitted string)		*/
extern void   hrd_trim_spaces(char* string);					/* trim leading and trailing spaces	*/

#endif
