#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <inttypes.h>
#include <limits.h>
#include <unistd.h>

#include "../libhrd.h"

/* Allocate 1 page for hashmap (most efficient way, i guess) */
#define DEFAULT_MAP_SIZE ( (sysconf(_SC_PAGESIZE) - sizeof(hrd_hashmap)) / sizeof(hrd_hashmap_slot) )

/* FNV1a from wikipedia */
static inline unsigned int get_index(hrd_hashmap* map, hrd_hashmap_slot* slot) {
	unsigned int hval = 0x811c9dc5;
	const unsigned int FNV_32_PRIME = 0x01000193;
	char* key = slot->key;
	while (*key != '\0') {
		hval ^= (unsigned int)*key++;
		hval *= FNV_32_PRIME;
	}
	slot->hash = hval;
	unsigned int index = hval % map->total_slots;
	for (;;) {
		if (map->slots[index].key == NULL || !strcmp(slot->key, map->slots[index].key)) {
			return index;
		} else {
			index = (index + 1) % map->total_slots;
		}
	}
}

hrd_hashmap* hrd_hashmap_create(unsigned int slots) {
	hrd_hashmap* map = malloc(sizeof(hrd_hashmap));
	if (!slots) 
		slots = DEFAULT_MAP_SIZE;
	map->slots = calloc(sizeof(hrd_hashmap_slot), slots);
	map->total_slots = slots;
	map->first = NULL;
	map->last = NULL;
	map->occupied_slots = 0;
	return map;
}

static hrd_hashmap_slot* resize_entry(hrd_hashmap* m, hrd_hashmap_slot* old_entry)
{
	uint32_t index = old_entry->hash % m->total_slots;
	for (;;)
	{
		hrd_hashmap_slot* entry = (hrd_hashmap_slot*) &m->slots[index];

		if (entry->key == NULL)
		{
			*entry = *old_entry; // copy data from old entry
			return entry;
		}

		index = (index + 1) % m->total_slots;
	}
}

void hrd_hashmap_resize(hrd_hashmap* map, unsigned int size) {
	hrd_hashmap_slot* old_slots = map->slots;
	map->total_slots = size;
	map->slots = calloc(sizeof(hrd_hashmap_slot), size);
	map->last = (hrd_hashmap_slot*) &map->first;
	do {
		hrd_hashmap_slot* current = map->last->next;
		if (current->key == NULL)
		{
			map->last->next = current->next;
			continue;
		}
		
		map->last->next = resize_entry(map, map->last->next);
		map->last = map->last->next;
	} while (map->last->next != NULL);
	free(old_slots);
} 

void hrd_hashmap_set_value(hrd_hashmap* map, char* key, void* value) {
	if (((float)map->occupied_slots / (float)map->total_slots) > 0.75)
		hrd_hashmap_resize(map, map->total_slots * 2);
	hrd_hashmap_slot slot;
	slot.key = strdup(key);
	slot.value = value;
	unsigned int index = get_index(map, &slot);
	hrd_hashmap_slot* last = map->last;
	map->slots[index] = slot;
	if (last)
		map->last->next = &map->slots[index];
	map->last = &map->slots[index];
	map->slots[index].next = NULL;
	map->slots[index].prev = last;
	map->occupied_slots += 1;
	if (map->first == NULL)
		map->first = &map->slots[index];
}

void* hrd_hashmap_get_value(hrd_hashmap* map, char* key) {
	hrd_hashmap_slot slot;
	slot.key = key;
	unsigned int index = get_index(map, &slot);
	return map->slots[index].value;
}

void hrd_hashmap_remove_value(hrd_hashmap* map, char* key) {
	hrd_hashmap_slot slot;
	slot.key = key;
	unsigned int index = get_index(map, &slot);
	map->slots[index].prev->next = map->slots[index].next;
	map->slots[index].next->prev = map->slots[index].prev;
	free(map->slots[index].key);
	map->slots[index].key = NULL;
	map->slots[index].value = NULL;
	map->slots[index].next = NULL;
	map->slots[index].prev = NULL;
	map->slots[index].hash = 0;
}

void hrd_hashmap_free(hrd_hashmap* map) {
	for (hrd_hashmap_slot* next = map->first; next; next = next->next) {
		free(next->key);
	}
	free(map->slots);
	free(map);
}
