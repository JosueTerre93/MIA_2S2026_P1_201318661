#include "Mkfs.h"

#include "Mount.h"

#include "../Estructuras/Ext2Structures.h"
#include "../Utils/Ext2Utils.h"

#include <fstream>
#include <cstring>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;


// ======================================================
// ESCRIBIR CEROS EN UNA REGION
// ======================================================

static bool limpiarRegion(
    fstream& archivo,
    int inicio,
    int tamanio
)
{
    archivo.clear();
    archivo.seekp(inicio);

    if (!archivo)
    {
        return false;
    }

    char buffer[1024] = {0};

    int restante =
        tamanio;

    while (restante > 0)
    {
        int cantidad =
            min(
                restante,
                static_cast<int>(sizeof(buffer))
            );

        archivo.write(
            buffer,
            cantidad
        );

        if (!archivo)
        {
            return false;
        }

        restante -=
            cantidad;
    }

    return true;
}


// ======================================================
// ESCRIBIR BITMAP INICIAL
// ======================================================

static bool escribirBitmap(
    fstream& archivo,
    int inicio,
    int cantidad
)
{
    archivo.clear();
    archivo.seekp(inicio);

    if (!archivo)
    {
        return false;
    }

    char libre =
        '0';

    for (int i = 0; i < cantidad; i++)
    {
        archivo.write(
            &libre,
            sizeof(char)
        );

        if (!archivo)
        {
            return false;
        }
    }

    return true;
}


// ======================================================
// MARCAR POSICION OCUPADA EN BITMAP
// ======================================================

static bool marcarBitmap(
    fstream& archivo,
    int inicioBitmap,
    int indice
)
{
    archivo.clear();

    archivo.seekp(
        inicioBitmap + indice
    );

    if (!archivo)
    {
        return false;
    }

    char ocupado =
        '1';

    archivo.write(
        &ocupado,
        sizeof(char)
    );

    return
        static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR TABLA DE INODOS VACIOS
// ======================================================

static bool escribirInodosVacios(
    fstream& archivo,
    const SuperBlock& sb
)
{
    Inode vacio =
        crearInodoVacio();

    archivo.clear();
    archivo.seekp(
        sb.s_inode_start
    );

    if (!archivo)
    {
        return false;
    }

    for (
        int i = 0;
        i < sb.s_inodes_count;
        i++
    )
    {
        archivo.write(
            reinterpret_cast<char*>(&vacio),
            sizeof(Inode)
        );

        if (!archivo)
        {
            return false;
        }
    }

    return true;
}


// ======================================================
// CREAR INODO RAIZ
// ======================================================

static Inode crearInodoRaiz()
{
    Inode raiz =
        crearInodoVacio();

    raiz.i_uid =
        1;

    raiz.i_gid =
        1;

    raiz.i_s =
        0;

    raiz.i_atime =
        time(nullptr);

    raiz.i_ctime =
        time(nullptr);

    raiz.i_mtime =
        time(nullptr);

    raiz.i_block[0] =
        0;

    raiz.i_type =
        '0';

    raiz.i_perm[0] =
        '6';

    raiz.i_perm[1] =
        '6';

    raiz.i_perm[2] =
        '4';

    return raiz;
}


// ======================================================
// CREAR BLOQUE DE CARPETA RAIZ
// ======================================================

static FolderBlock crearBloqueRaiz()
{
    FolderBlock bloque =
        crearFolderBlockVacio();

    memset(
        bloque.b_content[0].b_name,
        0,
        sizeof(
            bloque.b_content[0].b_name
        )
    );

    strncpy(
        bloque.b_content[0].b_name,
        ".",
        sizeof(
            bloque.b_content[0].b_name
        )
        - 1
    );

    bloque.b_content[0].b_inodo =
        0;


    memset(
        bloque.b_content[1].b_name,
        0,
        sizeof(
            bloque.b_content[1].b_name
        )
    );

    strncpy(
        bloque.b_content[1].b_name,
        "..",
        sizeof(
            bloque.b_content[1].b_name
        )
        - 1
    );

    bloque.b_content[1].b_inodo =
        0;


    memset(
        bloque.b_content[2].b_name,
        0,
        sizeof(
            bloque.b_content[2].b_name
        )
    );

    strncpy(
        bloque.b_content[2].b_name,
        "users.txt",
        sizeof(
            bloque.b_content[2].b_name
        )
        - 1
    );

    bloque.b_content[2].b_inodo =
        1;


    return bloque;
}


// ======================================================
// CREAR INODO DE USERS.TXT
// ======================================================

static Inode crearInodoUsers(
    int tamanioContenido
)
{
    Inode users =
        crearInodoVacio();

    users.i_uid =
        1;

    users.i_gid =
        1;

    users.i_s =
        tamanioContenido;

    users.i_atime =
        time(nullptr);

    users.i_ctime =
        time(nullptr);

    users.i_mtime =
        time(nullptr);

    users.i_block[0] =
        1;

    users.i_type =
        '1';

    users.i_perm[0] =
        '6';

    users.i_perm[1] =
        '6';

    users.i_perm[2] =
        '4';

    return users;
}


// ======================================================
// CREAR BLOQUE USERS.TXT
// ======================================================

static FileBlock crearBloqueUsers()
{
    FileBlock bloque{};

    string contenido =
        "1,G,root\n"
        "1,U,root,root,123\n";

    memset(
        bloque.b_content,
        0,
        sizeof(
            bloque.b_content
        )
    );

    memcpy(
        bloque.b_content,
        contenido.c_str(),
        contenido.size()
    );

    return bloque;
}


// ======================================================
// ACTUALIZAR SUPER BLOQUE
// ======================================================

static void actualizarSuperBlockInicial(
    SuperBlock& sb
)
{
    // Se ocupan:
    //
    // Inodo 0 -> /
    // Inodo 1 -> users.txt
    //
    // Bloque 0 -> carpeta raíz
    // Bloque 1 -> users.txt

    sb.s_free_inodes_count -=
        2;

    sb.s_free_blocks_count -=
        2;


    sb.s_firts_ino =
        sb.s_inode_start
        +
        2 * static_cast<int>(
            sizeof(Inode)
        );


    sb.s_first_blo =
        sb.s_block_start
        +
        2 * static_cast<int>(
            sizeof(FileBlock)
        );
}


// ======================================================
// ESCRIBIR SUPER BLOQUE
// ======================================================

static bool escribirSuperBlock(
    fstream& archivo,
    int inicio,
    const SuperBlock& sb
)
{
    archivo.clear();

    archivo.seekp(
        inicio
    );

    if (!archivo)
    {
        return false;
    }

    archivo.write(
        reinterpret_cast<const char*>(&sb),
        sizeof(SuperBlock)
    );

    return
        static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR INODO EN POSICION
// ======================================================

static bool escribirInodo(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const Inode& inode
)
{
    int posicion =
        sb.s_inode_start
        +
        indice
        * static_cast<int>(
            sizeof(Inode)
        );


    archivo.clear();

    archivo.seekp(
        posicion
    );


    if (!archivo)
    {
        return false;
    }


    archivo.write(
        reinterpret_cast<const char*>(&inode),
        sizeof(Inode)
    );


    return
        static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR BLOQUE
// ======================================================

template <typename T>
static bool escribirBloque(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const T& bloque
)
{
    int posicion =
        sb.s_block_start
        +
        indice
        * static_cast<int>(
            sizeof(FileBlock)
        );


    archivo.clear();

    archivo.seekp(
        posicion
    );


    if (!archivo)
    {
        return false;
    }


    archivo.write(
        reinterpret_cast<const char*>(&bloque),
        sizeof(T)
    );


    return
        static_cast<bool>(archivo);
}


// ======================================================
// MKFS
// ======================================================

string ejecutarMkfs(
    const MkfsParams& params
)
{
    // =====================================
    // VALIDACIONES
    // =====================================

    if (
        params.id.empty()
    )
    {
        return
            "Error: debe especificar -id.";
    }


    if (
        params.type != "full"
    )
    {
        return
            "Error: solamente se admite "
            "-type=full.";
    }


    // =====================================
    // BUSCAR PARTICION MONTADA
    // =====================================

    MountedPartition mounted;


    if (
        !buscarParticionMontada(
            params.id,
            mounted
        )
    )
    {
        return
            "Error: no existe una partición "
            "montada con el ID "
            + params.id
            + ".";
    }


    int inicioParticion =
        mounted.partition.part_start;


    int tamanioParticion =
        mounted.partition.part_s;


    // =====================================
    // VALIDAR TAMANIO
    // =====================================

    int numeroInodos =
        calcularNumeroInodos(
            tamanioParticion
        );


    if (
        numeroInodos < 2
    )
    {
        return
            "Error: la partición es demasiado "
            "pequeña para crear EXT2.";
    }


    // =====================================
    // ABRIR DISCO
    // =====================================

    fstream archivo(
        mounted.path,
        ios::binary
        |
        ios::in
        |
        ios::out
    );


    if (
        !archivo.is_open()
    )
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    // =====================================
    // FORMATEO FULL
    // =====================================

    if (
        !limpiarRegion(
            archivo,
            inicioParticion,
            tamanioParticion
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo limpiar "
            "la partición.";
    }


    // =====================================
    // CREAR SUPER BLOQUE
    // =====================================

    SuperBlock sb =
        crearSuperBlock(
            inicioParticion,
            tamanioParticion
        );


    // =====================================
    // INICIALIZAR BITMAPS
    // =====================================

    if (
        !escribirBitmap(
            archivo,
            sb.s_bm_inode_start,
            sb.s_inodes_count
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo inicializar "
            "el bitmap de inodos.";
    }


    if (
        !escribirBitmap(
            archivo,
            sb.s_bm_block_start,
            sb.s_blocks_count
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo inicializar "
            "el bitmap de bloques.";
    }


    // =====================================
    // INICIALIZAR TABLA DE INODOS
    // =====================================

    if (
        !escribirInodosVacios(
            archivo,
            sb
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo inicializar "
            "la tabla de inodos.";
    }


    // =====================================
    // CREAR /
    // =====================================

    Inode inodoRaiz =
        crearInodoRaiz();


    FolderBlock bloqueRaiz =
        crearBloqueRaiz();


    // =====================================
    // CREAR users.txt
    // =====================================

    string contenidoUsers =
        "1,G,root\n"
        "1,U,root,root,123\n";


    Inode inodoUsers =
        crearInodoUsers(
            static_cast<int>(
                contenidoUsers.size()
            )
        );


    FileBlock bloqueUsers =
        crearBloqueUsers();


    // =====================================
    // ESCRIBIR INODOS
    // =====================================

    if (
        !escribirInodo(
            archivo,
            sb,
            0,
            inodoRaiz
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo escribir "
            "el inodo raíz.";
    }


    if (
        !escribirInodo(
            archivo,
            sb,
            1,
            inodoUsers
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo escribir "
            "el inodo de users.txt.";
    }


    // =====================================
    // ESCRIBIR BLOQUES
    // =====================================

    if (
        !escribirBloque(
            archivo,
            sb,
            0,
            bloqueRaiz
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo escribir "
            "el bloque de la raíz.";
    }


    if (
        !escribirBloque(
            archivo,
            sb,
            1,
            bloqueUsers
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo escribir "
            "users.txt.";
    }


    // =====================================
    // ACTUALIZAR BITMAPS
    // =====================================

    if (
        !marcarBitmap(
            archivo,
            sb.s_bm_inode_start,
            0
        )
        ||
        !marcarBitmap(
            archivo,
            sb.s_bm_inode_start,
            1
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo actualizar "
            "el bitmap de inodos.";
    }


    if (
        !marcarBitmap(
            archivo,
            sb.s_bm_block_start,
            0
        )
        ||
        !marcarBitmap(
            archivo,
            sb.s_bm_block_start,
            1
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo actualizar "
            "el bitmap de bloques.";
    }


    // =====================================
    // ACTUALIZAR CONTADORES
    // =====================================

    actualizarSuperBlockInicial(
        sb
    );


    // =====================================
    // ESCRIBIR SUPER BLOQUE
    // =====================================

    if (
        !escribirSuperBlock(
            archivo,
            inicioParticion,
            sb
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo escribir "
            "el SuperBlock.";
    }


    archivo.flush();

    archivo.close();


    return
        "Sistema de archivos EXT2 creado "
        "correctamente en la partición "
        + params.id
        + ".";
}