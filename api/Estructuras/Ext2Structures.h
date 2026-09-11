#ifndef EXT2_STRUCTURES_H
#define EXT2_STRUCTURES_H

#include <ctime>

// ======================================================
// SUPER BLOQUE
// ======================================================

struct SuperBlock
{
    int s_filesystem_type;

    int s_inodes_count;
    int s_blocks_count;

    int s_free_blocks_count;
    int s_free_inodes_count;

    time_t s_mtime;
    time_t s_umtime;

    int s_mnt_count;

    int s_magic;

    int s_inode_s;
    int s_block_s;

    int s_firts_ino;
    int s_first_blo;

    int s_bm_inode_start;
    int s_bm_block_start;

    int s_inode_start;
    int s_block_start;
};


// ======================================================
// INODO
// ======================================================

struct Inode
{
    int i_uid;
    int i_gid;

    int i_s;

    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;

    // 0 - 11  -> directos
    // 12      -> indirecto simple
    // 13      -> indirecto doble
    // 14      -> indirecto triple
    int i_block[15];

    // '0' = carpeta
    // '1' = archivo
    char i_type;

    // permisos UGO, por ejemplo {'6','6','4'}
    char i_perm[3];
};


// ======================================================
// CONTENIDO DE CARPETA
// ======================================================

struct Content
{
    char b_name[12];

    int b_inodo;
};


// ======================================================
// BLOQUE DE CARPETA
// 4 * (12 + 4) = 64 bytes
// ======================================================

struct FolderBlock
{
    Content b_content[4];
};


// ======================================================
// BLOQUE DE ARCHIVO
// 64 bytes
// ======================================================

struct FileBlock
{
    char b_content[64];
};


// ======================================================
// BLOQUE DE APUNTADORES
// 16 * 4 = 64 bytes
// ======================================================

struct PointerBlock
{
    int b_pointers[16];
};

#endif