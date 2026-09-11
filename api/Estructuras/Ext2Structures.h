#ifndef EXT2_STRUCTURES_H
#define EXT2_STRUCTURES_H

#include <ctime>

// ======================================================
// SUPER BLOQUE
// ======================================================

struct SuperBlock
{
    // 2 = EXT2
    int s_filesystem_type;

    // Cantidad total de inodos
    int s_inodes_count;

    // Cantidad total de bloques
    int s_blocks_count;

    // Cantidad de bloques libres
    int s_free_blocks_count;

    // Cantidad de inodos libres
    int s_free_inodes_count;

    // Última fecha de montaje
    time_t s_mtime;

    // Última fecha de desmontaje
    time_t s_umtime;

    // Cantidad de montajes
    int s_mnt_count;

    // Identificador EXT2
    int s_magic;

    // Tamaño de un inodo
    int s_inode_s;

    // Tamaño de un bloque
    int s_block_s;

    // Dirección del primer inodo libre
    int s_first_ino;

    // Dirección del primer bloque libre
    int s_first_blo;

    // Inicio del bitmap de inodos
    int s_bm_inode_start;

    // Inicio del bitmap de bloques
    int s_bm_block_start;

    // Inicio de la tabla de inodos
    int s_inode_start;

    // Inicio de la región de bloques
    int s_block_start;
};


// ======================================================
// INODO
// ======================================================

struct Inode
{
    // UID del propietario
    int i_uid;

    // GID del grupo
    int i_gid;

    // Tamaño del archivo
    int i_s;

    // Último acceso
    time_t i_atime;

    // Fecha de creación
    time_t i_ctime;

    // Última modificación
    time_t i_mtime;

    /*
        Apuntadores:

        i_block[0]  - i_block[11]
            Bloques directos

        i_block[12]
            Indirecto simple

        i_block[13]
            Indirecto doble

        i_block[14]
            Indirecto triple

        Valor -1:
            apuntador no utilizado
    */
    int i_block[15];

    /*
        0 = carpeta
        1 = archivo
    */
    char i_type;

    /*
        Permisos UGO

        Ejemplo:
        664
        777
        755
    */
    char i_perm[3];
};


// ======================================================
// CONTENIDO DE BLOQUE DE CARPETA
// ======================================================

struct Content
{
    // Nombre de archivo o carpeta
    char b_name[12];

    // Índice del inodo asociado
    int b_inodo;
};


// ======================================================
// BLOQUE DE CARPETA
// ======================================================

struct FolderBlock
{
    /*
        4 entradas

        Cada Content debería ocupar:

        12 bytes nombre
        +
        4 bytes int

        = 16 bytes

        16 * 4 = 64 bytes
    */
    Content b_content[4];
};


// ======================================================
// BLOQUE DE ARCHIVO
// ======================================================

struct FileBlock
{
    // Contenido de archivo
    char b_content[64];
};


// ======================================================
// BLOQUE DE APUNTADORES
// ======================================================

struct PointerBlock
{
    /*
        16 apuntadores

        16 * 4 bytes = 64 bytes
    */
    int b_pointers[16];
};

#endif