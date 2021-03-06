#include "types.h"
/*
#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

struct fs_node;

u32 available_sector = 40;

// These typedefs define the type of callbacks - called when read/write/open/close
// are called.
typedef u32 (*read_type_t)(struct fs_node*, u32, u32, u8*);
typedef u32 (*write_type_t)(struct fs_node*, u32, u32, u8*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent* (*readdir_type_t)(struct fs_node*, u32);
typedef struct fs_node* (*finddir_type_t)(struct fs_node*,char *name);

typedef struct fs_node {
    char name[128];     // The filename.
    u32 mask;        // The permissions mask.
    u32 uid;         // The owning user.
    u32 gid;         // The owning group.
    u32 flags;       // Includes the node type. See #defines above.
    u32 inode;       // This is device-specific - provides a way for a filesystem to identify files.
    u32 length;      // Size of the file, in bytes.
    u32 impl;        // An implementation-defined number.
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    struct fs_node *ptr; // Used by mountpoints and symlinks.
} fs_node_t;

struct dirent {
    char name[128]; // Filename.
    u32 ino;     // Inode number. Required by POSIX.
};
*/
typedef struct node {
	u32 sector;
	u32 id;
	u32 ids[8];
	u32 *ptrs[8];
	u32 size;
	char *name;
} node_t;

typedef struct file {
	char content[32];
} file_t;

node_t *create_file(char *n, char *c);
file_t *read_file(node_t *n);
void add_child(node_t *child, node_t *parent);

/*

extern fs_node_t *fs_root; // The root of the filesystem.

// Standard read/write/open/close functions. Note that these are all suffixed with
// _fs to distinguish them from the read/write/open/close which deal with file descriptors
// , not file nodes.
u32 read_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer);
u32 write_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer);
void open_fs(fs_node_t *node, u8 read, u8 write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, u32 index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);*/
