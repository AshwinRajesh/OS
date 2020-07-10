#include "disk.h"
#include "ports.h"

void disk_read(u16 cylinder, u8 head, u8 sector, u32 size, u32 address) {

	u8 num_sectors = (u8)((size - 1) / 256 + 1);

	head &= 0x00001111;
	head |= 0x10100000;

	u8 low = cylinder & 0x00FF;
	u8 high = (cylinder & 0xFF00) >> 8;

	port_byte_out(0x1f6, head);
	port_byte_out(0x1f2, num_sectors);
	port_byte_out(0x1f3, sector);
	port_byte_out(0x1f4, low);
	port_byte_out(0x1f5, high);
	port_byte_out(0x1f7, 0x20);

	u8 output = port_byte_in(0x1f7);
	while ((output & 8) == 0x0) {
		output = port_byte_in(0x1f7);
	}

	for (int i = 0; i < num_sectors * 256; i++) {
		u16 data = port_word_in(0x1f0);
		if (i < size / 2) {
			u16 *p = address;
			p[i] = data;
		}
	}
}


void disk_write(u16 cylinder, u8 head, u8 sector, u32 size, u32 address) {

	head &= 0x00001111;
	head |= 0x10100000;

	u8 num_sectors = (u8)((size - 1) / 256 + 1);

	u8 low = cylinder & 0x00FF;
	u8 high = (cylinder & 0xFF00) >> 8;

	port_byte_out(0x1f6, head);
	port_byte_out(0x1f2, num_sectors);
	port_byte_out(0x1f3, sector);
	port_byte_out(0x1f4, low);
	port_byte_out(0x1f5, high);
	port_byte_out(0x1f7, 0x30);

	u8 output = port_byte_in(0x1f7);
	kprint_hex(output & 8);
	while ((output & 8) == 0x0) {
		output = port_byte_in(0x1f7);
	}

	for (int i = 0; i < num_sectors * 256; i++) {
		//port_word_out(0x1f0, 0x0000);
		if (i < (size / 2)) {
			u16 *p = address;
			u16 data = p[i];
			port_word_out(0x1f0, data);
		} else {
			u16 data = 0x3333;
			//kprint_hex(address);
			//kprint(" ");
			port_word_out(0x1f0, data);
		}
	}	
}

