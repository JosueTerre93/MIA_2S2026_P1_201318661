#include "Ext2Utils.h"

#include <ctime>

// ======================================================
// CALCULAR NÚMERO DE INODOS
// ======================================================

int calcularNumeroInodos(int tamanioParticion)
{
    /*
        Fórmula:

        tamaño_particion =
            sizeof(SuperBlock)
            + n
            + 3n
            + n * sizeof(Inode)
            + 3n * sizeof(Block)

        Despejando n:

        n =
            (tamaño_particion - sizeof(SuperBlock))
            /
            (
                1
                + 3
                + sizeof(Inode)
                + 3 * sizeof(Block)
            )

        Todos los bloques miden 64 bytes.
    */

    int numerador =
        tamanioParticion
        - static_cast<int>(sizeof(SuperBlock));

    int denominador =
        1
        + 3
        + static_cast<int>(sizeof(Inode))
        + (3 * static_cast<int>(sizeof(FileBlock)));

    if (numerador <= 0)
    {
        return 0;
    }

    return numerador / denominador;
}


// ======================================================
// CALCULAR NÚMERO DE BLOQUES
// ======================================================

int calcularNumeroBloques(int numeroInodos)
{
    return numeroInodos * 3;
}


// ======================================================
// CREAR SUPER BLOQUE
// ======================================================

SuperBlock crearSuperBlock(
    int inicioParticion,
    int tamanioParticion
)
{
    SuperBlock sb{};

    int numeroInodos =
        calcularNumeroInodos(
            tamanioParticion
        );

    int numeroBloques =
        calcularNumeroBloques(
            numeroInodos
        );

    // EXT2
    sb.s_filesystem_type = 2;

    // Cantidades totales
    sb.s_inodes_count = numeroInodos;
    sb.s_blocks_count = numeroBloques;

    // Inicialmente todo está libre
    sb.s_free_inodes_count = numeroInodos;
    sb.s_free_blocks_count = numeroBloques;

    // Fechas
    sb.s_mtime = std::time(nullptr);
    sb.s_umtime = 0;

    // Cantidad de montajes
    sb.s_mnt_count = 0;

    // Magic EXT2
    sb.s_magic = 0xEF53;

    // Tamaños
    sb.s_inode_s =
        static_cast<int>(
            sizeof(Inode)
        );

    sb.s_block_s =
        static_cast<int>(
            sizeof(FileBlock)
        );

    // =====================================
    // POSICIONES DENTRO DE LA PARTICIÓN
    // =====================================

    // SuperBlock
    //
    // inicioParticion
    //
    // luego viene bitmap de inodos

    sb.s_bm_inode_start =
        inicioParticion
        + static_cast<int>(
            sizeof(SuperBlock)
        );

    // Bitmap bloques empieza después
    // del bitmap de inodos
    sb.s_bm_block_start =
        sb.s_bm_inode_start
        + numeroInodos;

    // Tabla de inodos empieza después
    // del bitmap de bloques
    sb.s_inode_start =
        sb.s_bm_block_start
        + numeroBloques;

    // Bloques empiezan después
    // de la tabla de inodos
    sb.s_block_start =
        sb.s_inode_start
        + (
            numeroInodos
            * static_cast<int>(
                sizeof(Inode)
            )
        );

    // Primer inodo libre
    sb.s_firts_ino =
        sb.s_inode_start;

    // Primer bloque libre
    sb.s_first_blo =
        sb.s_block_start;

    return sb;
}


// ======================================================
// CREAR INODO VACÍO
// ======================================================

Inode crearInodoVacio()
{
    Inode inode{};

    inode.i_uid = -1;
    inode.i_gid = -1;

    inode.i_s = 0;

    inode.i_atime = 0;
    inode.i_ctime = 0;
    inode.i_mtime = 0;

    for (int i = 0; i < 15; i++)
    {
        inode.i_block[i] = -1;
    }

    // Por defecto lo dejamos como carpeta,
    // aunque al utilizarlo se debe sobrescribir
    inode.i_type = '0';

    inode.i_perm[0] = '0';
    inode.i_perm[1] = '0';
    inode.i_perm[2] = '0';

    return inode;
}


// ======================================================
// CREAR BLOQUE DE CARPETA VACÍO
// ======================================================

FolderBlock crearFolderBlockVacio()
{
    FolderBlock bloque{};

    for (int i = 0; i < 4; i++)
    {
        bloque.b_content[i].b_name[0] = '\0';
        bloque.b_content[i].b_inodo = -1;
    }

    return bloque;
}


// ======================================================
// CREAR BLOQUE DE APUNTADORES VACÍO
// ======================================================

PointerBlock crearPointerBlockVacio()
{
    PointerBlock bloque{};

    for (int i = 0; i < 16; i++)
    {
        bloque.b_pointers[i] = -1;
    }

    return bloque;
}