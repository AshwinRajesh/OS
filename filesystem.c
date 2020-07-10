#include "filesystem.h"
#include "disk.h"

u32 available_sector = 0;
u32 available_id = 0;

/*fs_node_t *fs_root = 0; // The root of the filesystem.

u32 read_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer)
{
    // Has the node got a read callback?
    if (node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

u32 write_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer)
{
    // Has the node got a write callback?
    if (node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

void open_fs(fs_node_t *node, u8 read, u8 write)
{
    // Has the node got an open callback?
    if (node->open != 0)
        return node->open(node);
}

void close_fs(fs_node_t *node)
{
    // Has the node got a close callback?
    if (node->close != 0)
        return node->close(node);
}

struct dirent *readdir_fs(fs_node_t *node, u32 index)
{
    // Is the node a directory, and does it have a callback?
    if ((node->flags & 0x7) == FS_DIRECTORY &&
         node->readdir != 0)
        return node->readdir(node, index);
    else
        return 0;
}

fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
    // Is the node a directory, and does it have a callback?
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->finddir != 0 )
        return node->finddir(node, name);
    else
        return 0;
}*/


node_t *create_file(char *n, char *c) {

    available_sector = (available_sector == 0) ? 41 : available_sector;

    node_t *node = kmalloc(sizeof(node_t));
    file_t *f = kmalloc(sizeof(file_t));
    string_copy(f->content, c);
    node->name = n;
    //f->content = con;
    node->size = sizeof(file_t);
    kprint("Sector: ");
    kprint_int(available_sector);
    kprint("\n");
    node->sector = available_sector;
    node->id = available_id;
    disk_write(0, 0, node->sector, sizeof(file_t), f);
    kprint("Hello");
    available_sector++;
    available_id++;
    return node;
}

file_t *read_file(node_t *n) {
    u32 p = kmalloc(sizeof(file_t));
    disk_read(0, 0, n->sector, sizeof(file_t), p);
    file_t *f = p;
    return f;
}

void add_child(node_t *child, node_t *parent) {
    int i = 0;
    while(parent->ids[i] != 0) {
        i++;
        kprint_int(parent->ids[i]);
    }
    parent->ids[i] = child->id;
    parent->ptrs[i] = child;

}
