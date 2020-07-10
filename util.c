#include "util.h"
#include "screen.h"
#include "kheap.h"
#include "paging.h"
#include "types.h"

extern heap_t *kheap;
extern page_directory_t *kernel_directory;

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) {
        n = -n;
    }
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) {
        str[i++] = '-';
    }
    str[i] = '\0';

    /* TODO: implement "reverse" */
    reverse(str);
}

int ascii_to_int(char c) {
    int asc = (int)c;
    if (asc > 47 & asc < 58) {
        return asc - 48;
    } else {
        return -1;
    } 
}

/* K&R */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

/* K&R 
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        //kprint(s1[i]);
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

char* decode(char s[], int num) {
    char result[sizeof(s)];
    int j = 1;
    int index = 0;
    for (int i = 0; i < strlen(s); i++) {
        if ((s[i] == " ") && (j == num)) {
            for (int k = 0; k < (i - index); k++) {
                result[k] = s[index + k];
                kprint(s[index + k]);
            }
            index = i + 1;
            j++;
        }
    }
    return result;
}

char *string_copy(char *dest, const char *src)
{
    do {
      *dest++ = *src++;
    }
    while (*src != 0);
}

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
u32 placement_address = 0x10000;

/* Implementation is just a pointer to some free memory which
 * keeps growing */
u32 kmalloc_int(u32 size, int align, u32 *phys_addr) {
    // Pages are aligned to 4K, or 0x1000
    
    if (placement_address == 0x0000) {
        placement_address += 0x10000;
    }

    if (kheap != 0) {
        void *addr = alloc(size, (u8)align, kheap);
        if (phys_addr != 0) {
            page_t *page = get_page((u32)addr, 0, kernel_directory);
            *phys_addr = page->frame * 0x1000 + (u32)addr & 0xFFF;
        }
        return (u32)addr;
    } else {

        if (align == 1 && (placement_address & 0x00000FFF)) {
            placement_address &= 0xFFFFF000;
            placement_address += 0x1000;
        }
        // Save also the physical address
        if (phys_addr) {
            *phys_addr = placement_address;
        }

        u32 ret = placement_address;
        placement_address += size; // Remember to increment the pointer

        return ret;
    }
}

void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    s32 tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

u32 kmalloc_a(u32 sz) {
    return kmalloc_int(sz, 1, 0);
}  // page aligned.
u32 kmalloc_p(u32 sz, u32 *phys) {
    return kmalloc_int(sz, 0, phys);
} // returns a physical address.
u32 kmalloc_ap(u32 sz, u32 *phys){
    return kmalloc_int(sz, 1, phys);
} // page aligned and returns a physical address.
u32 kmalloc(u32 sz){
    return kmalloc_int(sz, 0, 0);
} // vanilla (normal).

void panic(const char *message, const char *file, u32 line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    kprint("PANIC(");
    kprint(message);
    kprint(") at ");
    kprint(file);
    kprint(":");
    kprint(line);
    kprint("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, u32 line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    kprint("ASSERTION-FAILED(");
    kprint(desc);
    kprint(") at ");
    kprint(file);
    kprint(":");
    kprint(line);
    kprint("\n");
    // Halt by going into an infinite loop.
    for(;;);
}