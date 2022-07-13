#ifndef LIBHRD_PSEUDONAMES_H
#define LIBHRD_PSEUDONAMES_H

#define file_exist(filename, flags) hrd_file_exist(filename, flags)
#define set_cache_dir(filename) hrd_cache_set_dir(filename)
#define cache_data(filename, data, size) hrd_cache_data(filename, data, size)
#define get_cache_size(filename) hrd_uncache_get_size(filename)
#define get_cached_data(filename, dest, size)                                  \
	hrd_uncache_get_data(filename, dest, size)
#define start_cache_edit(filename, size) hrd_cache_edit_data(filename, size)
#define end_cache_edit(addr, size) hrd_cache_edit_enf(addr, size)
#define get_config_entry(config, key) hrd_cfg_get_string(config, key)
#define discard_chars(string, to_discard)                                      \
	hrd_string_discard_chars(string, to_discard)
#define split_string(string, delimiters) hrd_string_split(string, delimiters)
#define free_array(array) hrd_array_free(array)
#define trim_spaces(string) hrd_trim_spaces(string)

#endif
