#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#define __LOG(color, level, fmt, ...) \
  fprintf(stderr, color "[" level "] %s:%d (%s): " fmt "\033[0m\n", \
      __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define DEBUG(fmt, ...) __LOG("\033[1;90m", "DEBUG", fmt, ##__VA_ARGS__) //gray
#define INFO(fmt, ...) __LOG("\033[1;32m", "INFO", fmt, ##__VA_ARGS__) //green
#define WARN(fmt, ...) __LOG("\033[1;33m", "WARN", fmt, ##__VA_ARGS__) //yellow
#define ERRORL(fmt, ...) __LOG("\033[1;31m", "ERROR", fmt, ##__VA_ARGS__) //red
#define FATAL(fmt, ...) do { __LOG("\033[1;31m", "FATAL", fmt, ##__VA_ARGS__); exit(1); } while (0) //red

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t   u8;
typedef int64_t  i64;
typedef int32_t  i32;
typedef int16_t  i16;
typedef int8_t    i8;
typedef float    f32;
typedef double   f64;
typedef size_t usize;

typedef struct { f32 x; f32 y; } vec2f;
typedef struct { i32 x; i32 y; } vec2i;
typedef struct { u32 x; u32 y; } vec2u;

typedef vec2f vec2;

vec2f vec2u_tof(vec2u v);
vec2f vec2i_tof(vec2i v);
vec2u vec2f_tou(vec2f v);
vec2u vec2i_tou(vec2i v);
vec2i vec2f_toi(vec2f v);
vec2i vec2u_toi(vec2u v);

#define __VEC2OP__(T) \
  vec2##T vec2##T##_add(vec2##T a, vec2##T b); \
  vec2##T vec2##T##_sub(vec2##T a, vec2##T b); \
  vec2##T vec2##T##_mul(vec2##T a, vec2##T b); \
  vec2##T vec2##T##_div(vec2##T a, vec2##T b); \

__VEC2OP__(f);
__VEC2OP__(i);
__VEC2OP__(u);

typedef struct {
  void *items;
  usize capacity;
  usize len;
  usize item_size;
} ArrayList;

ArrayList *arraylist_create(usize item_size, usize initial_capacity);
usize arraylist_append(ArrayList *arr, void *item);
void *arraylist_get(ArrayList *arr, usize index);
u8 arraylist_remove(ArrayList *arr, usize index);

typedef struct {
  f32 i;
  bool end;
  bool ended_once;
  bool repeat;
  f32 time;
} Timer;

Timer timer_create(f32 time, bool repeat);
void timer_update(Timer *t);
