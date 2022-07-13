#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#include "../libhrd.h"

int hrd_cfg_put_string_at(char* filename, char* key, char* value)
{
	FILE* stream = fopen(filename, "w");
	if (!stream)
		return -1;
	int res = hrd_cfg_put_string(stream, key, value);
	fclose(stream);
	return res;
}

int hrd_cfg_put_strings_at(char* filename, hrd_string_pair keys[])
{
	FILE* stream = fopen(filename, "w");
	if (!stream)
		return -1;
	int res = hrd_cfg_put_strings(stream, keys);
	fclose(stream);
	return res;
}

int hrd_cfg_put_string(FILE* stream, char* key, char* value)
{
	hrd_string_pair* keys = alloca(sizeof(hrd_string_pair) * 2);
	hrd_string_pair key_arg = { key, value };
	hrd_string_pair terminator = { 0, 0 };
	keys[0] = key_arg;
	keys[1] = terminator;
	return hrd_cfg_put_strings(stream, keys);
}

int hrd_cfg_put_strings(FILE* stream, hrd_string_pair keys[])
{
	if (!stream) {
		errno = EINVAL;
		return -1;
	}
	int retval = 0;
	hrd_string_pair_array_foreach(i, keys)
	{
		retval = fprintf(stream, "%s=%s\n", keys[i].key, keys[i].value);
		if (retval < 0)
			break;
	}
	return retval;
}

int hrd_cfg_write(hrd_config* cfg, FILE* stream)
{
	int retval = 0;
	if (!cfg || !stream) {
		errno = EINVAL;
		return -1;
	}
	for (hrd_hashmap_slot* slot = cfg->global_keys->first; slot;
	     slot = slot->next) {
		retval = fprintf(stream, "%s=%s\n", slot->key,
				 slot->value ? (char*)slot->value : "");
	}
	if (retval < 0)
		return retval;
	for (hrd_hashmap_slot* slot = cfg->sections->first; slot;
	     slot = slot->next) {
		retval = fprintf(stream, "[%s]\n", slot->key);
		if (retval < 0)
			return retval;
		hrd_hashmap* map = (hrd_hashmap*)slot->value;
		for (hrd_hashmap_slot* mslot = map->first; mslot;
		     mslot = mslot->next) {
			retval = fprintf(stream, "%s=%s\n", mslot->key,
					 mslot->value ? (char*)mslot->value :
							"");
			if (retval < 0)
				return retval;
		}
	}
	return retval;
}

int hrd_cfg_write_at(hrd_config* cfg, char* filename)
{
	if (!cfg || !filename) {
		errno = EINVAL;
		return -1;
	}
	FILE* stream = fopen(filename, "w");
	if (!stream)
		return -1;
	return hrd_cfg_write(cfg, stream);
}

void hrd_cfg_set_string(hrd_config* cfg, char* section, char* key, char* value)
{
	if (section) {
		hrd_hashmap* sct =
			hrd_hashmap_get_value(cfg->sections, section);
		hrd_hashmap_set_value(sct, key, value);
	} else {
		hrd_hashmap_set_value(cfg->global_keys, key, value);
	}
}
