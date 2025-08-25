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
