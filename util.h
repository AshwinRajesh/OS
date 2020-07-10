#ifndef UTIL_H
#define UTIL_H

#include "types.h"

void memory_copy(char *source, char *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);
void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);
void append(char s[], char n);
void backspace(char s[]);
int strcmp(char s1[], char s2[]);
u32 kmalloc_int(u32 size, int align, u32 *phys_addr);
u32 kmalloc_a(u32 sz);  // page aligned.
u32 kmalloc_p(u32 sz, u32 *phys); // returns a physical address.
u32 kmalloc_ap(u32 sz, u32 *phys); // page aligned and returns a physical address.
u32 kmalloc(u32 sz); // vanilla (normal).

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32 line);
extern void panic_assert(const char *file, u32 line, const char *desc);


#endif