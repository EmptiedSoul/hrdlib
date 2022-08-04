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

/* Parser for INI- and Bash-like configuration files */

char* hrd_cfg_get_string(hrd_config* cfg, char* section, char* key)
{
	if (section) {
		hrd_hashmap* map =
			hrd_hashmap_get_value(cfg->sections, section);
		if (section) {
			char* res = hrd_hashmap_get_value(map, key);
			if (res)
				return strdup(res);
		}
	} else {
		char* res = hrd_hashmap_get_value(cfg->global_keys, key);
		if (res)
			return strdup(res);
	}
	return NULL;
}

hrd_config* hrd_cfg_read(FILE* stream)
{
	char* line = NULL;
	size_t buflen = 0;
	ssize_t nread = 0;
	char* current_section = NULL;

	hrd_config* cfg = malloc(sizeof(hrd_config));
	if (!cfg)
		goto oom_0;

	cfg->global_keys = hrd_hashmap_create(0);
	if (!cfg->global_keys)
		goto oom_1;

	/* 20 sections should be enough for average INI file 
	 * Passing 0 will lead to memory overuse */
	cfg->sections = hrd_hashmap_create(20);
	if (!cfg->sections)
		goto oom_2;

	while ((nread = getline(&line, &buflen, stream)) != -1) {
		if (*line == '#' || *line == ';' || *line == '\n') /* Comment */
			continue;
		if (*line == '[') { /* INI Section header */
			char* section = line + 1;
			section[strlen(section) - 2] = '\0';
			if (current_section)
				free(current_section);
			current_section = strdup(section);
			if (!current_section)
				break;
			hrd_hashmap* map = hrd_hashmap_create(0);
			if (!map)
				break;
			if (!hrd_hashmap_set_value(cfg->sections, section,
						   map)) {
				hrd_hashmap_free(map);
				break;
			}
			continue;
		}
		char* equal_sign = strchr(line, '=');
		if (!equal_sign) {
			errno = EINVAL;
			break;
		}
		char* value = equal_sign + 1;
		char* value_newline = strrchr(value, '\n');
		*value_newline = '\0';
		*equal_sign = '\0';
		char* key = line;
		hrd_trim_spaces(key);
		hrd_trim_spaces(value);
		if (current_section) {
			char* dupval = strdup(value);
			if (!dupval)
				break;
			if (!hrd_hashmap_set_value(
				    hrd_hashmap_get_value(cfg->sections,
							  current_section),
				    key, dupval)) {
				free(dupval);
				break;
			}
		} else {
			char* dupval = strdup(value);
			if (!dupval)
				break;
			if (!hrd_hashmap_set_value(cfg->global_keys, key,
						   dupval)) {
				free(dupval);
				break;
			}
		}
	}
	if (current_section)
		free(current_section);
	free(line);
	return cfg;

oom_2:
	hrd_hashmap_free(cfg->global_keys);
oom_1:
	free(cfg);
oom_0:
	return NULL;
}

hrd_config* hrd_cfg_read_at(char* filename)
{
	FILE* stream = fopen(filename, "r");
	if (stream) {
		hrd_config* res = hrd_cfg_read(stream);
		fclose(stream);
		return res;
	} else
		return NULL;
}

char* hrd_cfg_get_string_at(char* filename, char* section, char* key)
{
	hrd_auto_ptr(hrd_config) cfg = hrd_cfg_read_at(filename);
	if (cfg)
		return hrd_cfg_get_string(cfg, section, key);
	else
		return NULL;
}

void hrd_cfg_free(hrd_config* cfg)
{
	if (!cfg)
		return;
	for (hrd_hashmap_slot* slot = cfg->global_keys->first; slot;
	     slot = slot->next) {
		free(slot->value);
	}
	hrd_hashmap_free(cfg->global_keys);
	for (hrd_hashmap_slot* slot = cfg->sections->first; slot;
	     slot = slot->next) {
		if (slot->value) {
			hrd_hashmap* map = slot->value;
			for (hrd_hashmap_slot* nslot = map->first; nslot;
			     nslot = nslot->next)
				free(nslot->value);
			hrd_hashmap_free(map);
		}
	}
	hrd_hashmap_free(cfg->sections);
	free(cfg);
}

void hrd_config_autofree(hrd_config** cfg)
{
	return hrd_cfg_free(*cfg);
}
