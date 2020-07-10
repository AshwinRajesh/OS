#include "types.h"

void disk_read(u16 cylinder, u8 head, u8 sector, u32 size, u32 address);
void disk_write(u16 cylinder, u8 head, u8 sector, u32 size, u32 address);