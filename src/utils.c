#include <utils.h>
#include <core.h>
#include <string.h>

ArrayList *arraylist_create(usize item_size, usize initial_capacity) {
	ArrayList *arr = malloc(sizeof(ArrayList));
	if (!arr) {
		ERRORL("failed to allocate memory for the arraylist");
    return NULL;
  }
	arr->item_size = item_size;
	arr->capacity = initial_capacity;
	arr->len = 0;
	arr->items = malloc(item_size*arr->capacity);
	if (!arr->items) {
		ERRORL("failed to allocate memory for arraylist");
    return NULL;
  }
	return arr;
}

usize arraylist_append(ArrayList *arr, void *item) {
	if (arr->capacity == arr->len) {
		arr->capacity = arr->capacity > 0 ? arr->capacity*2 : 1;
		void *items = realloc(arr->items, arr->item_size*arr->capacity);
		if (!items) {
      ERRORL("failed to realloc memory for arraylist");
      return -1;
    }
		arr->items = items;
	}
	size_t index = arr->len++;
	memcpy(arr->items+arr->item_size*index, item, arr->item_size);
	return index;
}

void *arraylist_get(ArrayList *arr, usize index) {
	if (index >= arr->len) {
		ERRORL("index out of bounds");
    return NULL;
  }
	return arr->items+index*arr->item_size;
}

u8 arraylist_remove(ArrayList *arr, usize index) {
	if (arr->len == 0) {
    ERRORL("list is empty\n");
    return 1;
  }
	if (index >= arr->len) {
    ERRORL("index out of bounds\n");
    return 1;
  }

	if (arr->len == 1) {
		arr->len = 0;
		return 0;
	}

	--arr->len;

	u8 *item_ptr = (u8*)arr->items + index * arr->item_size;
	u8 *end_ptr = (u8*)arr->items + arr->len * arr->item_size;
	memcpy(item_ptr, end_ptr, arr->item_size);

	return 0;
}

static unsigned long hash_str(const char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

HashMap hashmap_create(u32 capacity) {
  HashMap res = {0};
  res.capacity = capacity;

  res.entries = malloc(sizeof(HashEntry*)*capacity);
  memset(res.entries, 0, capacity * sizeof(HashEntry*));

  return res;
}
void hashmap_put(HashMap *map, const char *key, void *value) {
  usize i = hash_str(key) % map->capacity;

  HashEntry *e = map->entries[i];

  while (e != NULL) {
    if (!strcmp(e->key, key)) {
      e->value = value;
      return;
    }
    e = e->next;
  }

  HashEntry *new = malloc(sizeof(HashEntry));
  new->key = strdup(key);
  new->value = value;
  new->next = map->entries[i];

  map->entries[i] = new;
  map->size++;
}
void *hashmap_get(HashMap *map, const char *key) {
  u32 index = hash_str(key) % map->capacity;
  HashEntry *entry = map->entries[index];

  while (entry) {
    if (!strcmp(key, entry->key))
      return entry->value;
    entry = entry->next;
  }
  return NULL;
}
void hashmap_remove(HashMap *map, const char *key) {
  u32 index = hash_str(key) % map->capacity;
  HashEntry *entry = map->entries[index];
  HashEntry *prev = NULL;

  while (entry) {
    if (!strcmp(key, entry->key)) {
      if (prev)
        prev->next = entry->next;
      else
        map->entries[index] = entry->next;

      free(entry->key);
      free(entry);
      map->size--;
      return;
    }

    prev = entry;
    entry = entry->next;
  } 
}
void hashmap_free(HashMap *map) {
  for (u32 i = 0; i < map->capacity; i++) {
    HashEntry *entry = map->entries[i];
    while (entry) {
      HashEntry *next = entry->next;
      free(entry->key);
      free(entry);
      entry = next;
    }
  }
  free(map->entries);
  map->entries = NULL;
  map->size = 0;
  map->capacity = 0;
}

Timer timer_create(f32 time, bool repeat) {
  Timer res = (Timer) {
    .time = time,
    .repeat = repeat,
    .i = time
  };

  return res;
}

void timer_update(Timer *t) {
  t->end = 0;
  t->i -= time_getdelta();
  if (t->i <= 0) {
    t->end = 1;
    t->ended_once = 1;
    if (t->repeat)
      t->i = t->time;
  }
}

#define _IO_READ_CHUNK 2097152
#define _IO_READ_ERROR_GEN "error reading file: %s\n, errno: %d\n"
#define _IO_READ_ERROR_MEM "not enough memory to read the file: %s\n"

File file_read(const char *path) {
	File file = {.valid=0};

	FILE *fp = fopen(path, "rb");
	if (!fp || ferror(fp)) {
		ERRORL(_IO_READ_ERROR_GEN, path, errno);
    return file;
  }

	char *data = NULL;
	char *tmp;
	usize used = 0;
	usize size = 0;
	usize n;

	while (1) {
		if (used + _IO_READ_CHUNK+1 > size) {
			size = used+_IO_READ_CHUNK+1;

			if (size <= used) {
				free(data);
				ERRORL("input file too large: %s\n", path);
        return file;
			}

			tmp = realloc(data, size);
			if (!tmp) {
				free(data);
				ERRORL(_IO_READ_ERROR_MEM, path);
        return file;
			}
			data = tmp;
		}

		n = fread(data+used, 1, _IO_READ_CHUNK, fp);
		if (n == 0)
			break;

		used+=n;
	}
	
	if (ferror(fp)) {
		free(data);
		ERRORL(_IO_READ_ERROR_GEN, path, errno);
    return file;
	}
	
	tmp = realloc(data, used+1);
	if (!tmp) {
		free(data);
		ERRORL(_IO_READ_ERROR_MEM, path);
    return file;
	}
	data = tmp;
	data[used] = 0;

	file.data = data;
	file.len = used;
	file.valid = 1;

	return file;
}

bool file_write(void *buffer, usize size, const char *path) {
	FILE *fp = fopen(path, "wb");
	if (!fp || ferror(fp)) {
		ERRORL("cannot write file : %s\n", path);
    return 1;
  }
	usize chunks_written = fwrite(buffer, size, 1, fp);
	fclose(fp);
	if (chunks_written != 1) {
		ERRORL("write error: expected 1 chunk got %zu\n", chunks_written);
    return 1;
  }

	return 0;
}
