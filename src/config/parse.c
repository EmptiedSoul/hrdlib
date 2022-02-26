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

char* hrd_cfg_get_string(hrd_config* cfg, char* section, char* key){
	if (section) {
		hrd_hashmap* map = hrd_hashmap_get_value(cfg->sections, section);
		if (section)
			return hrd_hashmap_get_value(map, key);
	} else {
		return hrd_hashmap_get_value(cfg->global_keys, key);
	}
	return NULL;
}

hrd_config* hrd_cfg_read(FILE* stream) {
	char*	line		= NULL;
	size_t	buflen		= 0;
	ssize_t	nread		= 0;
	char* current_section   = NULL;

	hrd_config*  cfg = malloc(sizeof(hrd_config));
	cfg->global_keys = hrd_hashmap_create(0);
	cfg->sections = hrd_hashmap_create(20); 
	/* 20 sections should be enough for average INI file 
	 * Passing 0 will lead to memory overuse */

	while ((nread = getline(&line, &buflen, stream)) != -1) {
		if (*line == '#' || *line == ';' || *line == '\n') /* Comment */
			continue;
		if (*line == '[') { /* INI Section header */
			char* section = line + 1;
			section[strlen(section) - 2] = '\0';
			if (current_section) 
				free(current_section);
			current_section = strdup(section);
			hrd_hashmap_set_value(cfg->sections, section, hrd_hashmap_create(0));
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
		if (current_section)
			hrd_hashmap_set_value(hrd_hashmap_get_value(cfg->sections, current_section), key, strdup(value));
		else
			hrd_hashmap_set_value(cfg->global_keys, key, strdup(value));
	}
	if (current_section)
		free(current_section);
	free(line);
	return cfg;
}

hrd_config* hrd_cfg_read_at(char* filename) {
	FILE* stream = fopen(filename, "r");
	if (stream) {
		hrd_config* res = hrd_cfg_read(stream);
		fclose(stream);
		return res;
	} else 
		return NULL;
}

char* hrd_cfg_get_string_at(char* filename, char* section, char* key){
	hrd_config* cfg = hrd_cfg_read_at(filename);
	if (cfg)
		return hrd_cfg_get_string(cfg, section, key);
	else
		return NULL;
}

void hrd_cfg_free(hrd_config* cfg) {
	hrd_hashmap_free(cfg->global_keys);
	for (hrd_hashmap_slot* slot = cfg->sections->first; slot; slot = slot->next){
		if (slot->value)
			hrd_hashmap_free(slot->value);
	}
	hrd_hashmap_free(cfg->sections);
	free(cfg);
}
