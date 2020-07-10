#include "screen.h"
#include "util.h"
#include "isr.h"
#include "idt.h"
#include "timer.h"
#include "kernel.h"
#include "keyboard.h"
#include "paging.h"
#include "types.h"
#include "kheap.h"
#include "disk.h"
#include "filesystem.h"

node_t *root;
node_t *child;

void main() {
    clear_screen();
    isr_install();
    irq_install();
    //kprint("Type something, it will go through the kernel\n"
    //  "Type END to halt the CPU\n> ");

	//kprint(" paging world!");
    //u32 *ptr = (u32*)0x1000;
    //u32 do_page_fault = *ptr;
    /*kprint("Initialized paging.");

    u32 a = kmalloc(8);
    u32 b = kmalloc(8);

    kprint("\na: ");
    kprint_hex(a);
    kprint("\nb: ");
    kprint_hex(b);

    //kfree(a);
    kfree(b);

    u32 c = kmalloc(8);
    kprint("\nc: ");
    kprint_hex(c);*/

    initialise_paging();

    /*disk_write(0, 0, 40, 512, 0x1000);
    disk_write(0, 0, 45, 512, 0x1200);

    u32 a = kmalloc(32);
    kprint_hex(a);
    disk_read(0, 0, 3, 32, a);
    kprint("\n");
    u16 *p1 = a;
    kprint_hex(p1[0]);
    kprint("\n");

    u32 b = kmalloc(32);
    kprint_hex(b);
    disk_read(0, 0, 40, 32, b);
    kprint("\n");
    u16 *p2 = b;
    kprint_hex(p2[0]);
    kprint("\n");
    
    u32 c = kmalloc(32);
    kprint_hex(c);
    disk_read(0, 0, 41, 32, c);
    kprint("\n");
    u16 *p3 = c;
    kprint_hex(p3[0]);
    kprint("\n");*/

    //char* output = decode("Hello World!", 1);
    //kprint(output[1]);


}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } /*else if (strcmp(input, "PAGE") == 0) {
        // Lesson 22: Code to test kmalloc, the rest is unchanged
        u32 phys_addr;
        u32 page = kmalloc_ap(1000, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
    }*/ 
    else if (strcmp(input, "WRITE") == 0) {
        //disk_write(0, 0, 40, 32, 0xc008000c);
        //u32 addr = kmalloc(sizeof(file_t));
        root = create_file("Hello", "How are you");
        child = create_file("Child", "child");
        //add_child(child, root);
    } else if (strcmp(input, "READ") == 0) {
        file_t *f1 = read_file(root);
        kprint("Content: "); 
        kprint(f1->content);
        kprint("\n");
    }
    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}
