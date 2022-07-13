#ifndef LIBHRD_H
#define LIBHRD_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define DEPRECATED __attribute__((deprecated))

#define DO_PRAGMA(x) _Pragma(#x)
#define TODO(x) DO_PRAGMA(message("TODO: " #x))

#define _HRD_TYPE_CLEANUP_FUNC(type) __cleanup__(type##_autofree)
#define hrd_auto_ptr(type) __attribute__((_HRD_TYPE_CLEANUP_FUNC(type))) type*
#define hrd_autofree __attribute__((__cleanup__(_hrd_autofree)))

/* data types */

typedef char* hrd_string_array;
/* Always referred as char**, this alias only used 
 * for hrd_auto_ptr() macro 		*/

typedef struct {
	char* key;
	char* value;
} hrd_string_pair;

typedef struct _hrd_hashmap_slot {
	struct _hrd_hashmap_slot* next;
	struct _hrd_hashmap_slot* prev;
	char* key;
	void* value;
	unsigned int hash;
} hrd_hashmap_slot;

typedef struct {
	hrd_hashmap_slot* slots;
	hrd_hashmap_slot* first;
	hrd_hashmap_slot* last;
	int occupied_slots;
	int total_slots;
} hrd_hashmap;

typedef struct {
	hrd_hashmap* global_keys;
	hrd_hashmap* sections;
} hrd_config;

/* fs */
enum {
	HRDFS_ISDIR = 1, /* is a directory			*/
	HRDFS_ISFILE = 2, /* is a regular file			*/
	HRDFS_READBL = 4, /* is readable				*/
	HRDFS_WRITBL = 8, /* is writable				*/
	HRDFS_EXECBL = 16, /* is executable			*/
};

extern bool
hrd_file_exist(char* filename,
	       int flags); /* is file exist 			*/
/* cache */
extern int
hrd_cache_set_dir(char* filename); /* Set cache file directory 		*/
extern int
hrd_cache_data(char* filename, void* data,
	       size_t size); /* Serialize data			*/
extern size_t
hrd_uncache_get_size(char* filename); /* Get size of cached data		*/
extern int
hrd_uncache_get_data(char* filename, void* dest,
		     size_t size); /* Uncache data				*/
extern void*
hrd_cache_edit_data(char* filename,
		    size_t size); /* Edit cached data			*/
extern int
hrd_cache_edit_end(void* addr,
		   size_t size); /* End editing cached data		*/

/* config */
extern char* hrd_cfg_get_string_at(char* filename, char* section, char* key);
extern char*
hrd_cfg_get_string(hrd_config* cfg, char* section,
		   char* key); /* Get value for key in cfg file	*/
extern void hrd_cfg_set_string(hrd_config* cfg, char* section, char* key,
			       char* value);

extern hrd_config* hrd_cfg_read(FILE* stream);
extern hrd_config* hrd_cfg_read_at(char* filename);
extern void hrd_cfg_free(hrd_config* cfg);

extern void hrd_config_autofree(hrd_config** cfg);

extern int hrd_cfg_put_string_at(char* filename, char* key, char* value);
extern int
hrd_cfg_put_string(FILE* stream, char* key,
		   char* value); /* Put key=value to cfg file		*/
extern int hrd_cfg_put_strings_at(char* filename, hrd_string_pair keys[]);
extern int hrd_cfg_put_strings(
	FILE* stream,
	hrd_string_pair keys[]); /* Put (key=value)n to cfg file		*/
extern int hrd_cfg_write(hrd_config* cfg, FILE* stream);
extern int hrd_cfg_write_at(hrd_config* cfg, char* filename);

/* hashmap */
hrd_hashmap* hrd_hashmap_create(unsigned int slots);
void hrd_hashmap_resize(hrd_hashmap* map, unsigned int size);
void hrd_hashmap_set_value(hrd_hashmap* map, char* key, void* value);
void* hrd_hashmap_get_value(hrd_hashmap* map, char* key);
void hrd_hashmap_remove_value(hrd_hashmap* map, char* key);
void hrd_hashmap_free(hrd_hashmap* map);
void hrd_hashmap_autofree(hrd_hashmap** map);

/* strings */
extern size_t hrd_string_discard_chars(
	char* string,
	char to_discard); /* discard chars from string		*/
extern char**
hrd_string_split(char* string,
		 char* delimiters); /* split string				*/
extern void
hrd_string_array_free(char** array); /* free string array (splitted string)	*/
extern void hrd_string_array_push(char*** array, char* string);
extern void hrd_string_array_autofree(char*** array);
extern char* hrd_sprintf(char* fmt, ...);
#define hrd_array_free(x) hrd_string_array_free(x);

inline bool hrd_string_has_prefix(char* prefix, char* string)
{
	return !(strncmp(prefix, string, strlen(prefix)));
}

extern void hrd_string_pair_array_free(
	hrd_string_pair* array); /* free string pair array		*/
extern void hrd_string_pair_array_free_stackkeys(hrd_string_pair* array);
extern void hrd_string_pair_array_free_keysonly(hrd_string_pair* array);
#define hrd_string_array_foreach(__hrd_iter, x)                                \
	for (int __hrd_iter = 0; x[__hrd_iter];                                \
	     __hrd_iter++) /* iterate thru string array		*/
#define hrd_string_pair_array_foreach(iter, x)                                 \
	for (int iter = 0; (x)[iter].key;                                      \
	     iter++) /* iterate thru string pair array	*/
extern void
hrd_trim_spaces(char* string); /* trim leading and trailing spaces	*/
extern void _hrd_autofree(void* mem);
#endif
