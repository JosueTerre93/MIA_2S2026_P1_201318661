#include "Mkdir.h"

#include "Mount.h"
#include "Session.h"

#include "../Estructuras/Ext2Structures.h"
#include "../Utils/Ext2Utils.h"

#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>

using namespace std;


// ======================================================
// DIVIDIR RUTA
// ======================================================

static vector<string> dividirRuta(
    const string& ruta)
{
    vector<string> partes;
    string actual;

    for (char c : ruta)
    {
        if (c == '/')
        {
            if (!actual.empty())
            {
                partes.push_back(actual);
                actual.clear();
            }
        }
        else
        {
            actual += c;
        }
    }

    if (!actual.empty())
        partes.push_back(actual);

    return partes;
}


// ======================================================
// LEER SUPERBLOCK
// ======================================================

static bool leerSuperBlock(
    fstream& archivo,
    int inicio,
    SuperBlock& sb)
{
    archivo.clear();
    archivo.seekg(inicio);

    if (!archivo)
        return false;

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR SUPERBLOCK
// ======================================================

static bool escribirSuperBlock(
    fstream& archivo,
    int inicio,
    const SuperBlock& sb)
{
    archivo.clear();
    archivo.seekp(inicio);

    if (!archivo)
        return false;

    archivo.write(
        reinterpret_cast<const char*>(&sb),
        sizeof(SuperBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// LEER INODO
// ======================================================

static bool leerInodo(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    Inode& inode)
{
    int posicion =
        sb.s_inode_start +
        indice * static_cast<int>(sizeof(Inode));

    archivo.clear();
    archivo.seekg(posicion);

    if (!archivo)
        return false;

    archivo.read(
        reinterpret_cast<char*>(&inode),
        sizeof(Inode)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR INODO
// ======================================================

static bool escribirInodo(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const Inode& inode)
{
    int posicion =
        sb.s_inode_start +
        indice * static_cast<int>(sizeof(Inode));

    archivo.clear();
    archivo.seekp(posicion);

    if (!archivo)
        return false;

    archivo.write(
        reinterpret_cast<const char*>(&inode),
        sizeof(Inode)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// LEER FOLDER BLOCK
// ======================================================

static bool leerFolderBlock(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    FolderBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indice * static_cast<int>(sizeof(FileBlock));

    archivo.clear();
    archivo.seekg(posicion);

    if (!archivo)
        return false;

    archivo.read(
        reinterpret_cast<char*>(&bloque),
        sizeof(FolderBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR FOLDER BLOCK
// ======================================================

static bool escribirFolderBlock(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const FolderBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indice * static_cast<int>(sizeof(FileBlock));

    archivo.clear();
    archivo.seekp(posicion);

    if (!archivo)
        return false;

    archivo.write(
        reinterpret_cast<const char*>(&bloque),
        sizeof(FolderBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// BUSCAR LIBRE EN BITMAP
// ======================================================

static int buscarLibreBitmap(
    fstream& archivo,
    int inicio,
    int cantidad)
{
    archivo.clear();
    archivo.seekg(inicio);

    if (!archivo)
        return -1;

    for (int i = 0; i < cantidad; i++)
    {
        char estado;

        archivo.read(
            &estado,
            sizeof(char)
        );

        if (!archivo)
            return -1;

        if (estado == '0')
            return i;
    }

    return -1;
}


// ======================================================
// MARCAR BITMAP
// ======================================================

static bool marcarBitmap(
    fstream& archivo,
    int inicio,
    int indice)
{
    archivo.clear();

    archivo.seekp(
        inicio + indice
    );

    if (!archivo)
        return false;

    char ocupado = '1';

    archivo.write(
        &ocupado,
        sizeof(char)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// BUSCAR ENTRADA EN CARPETA
// ======================================================

static int buscarEntrada(
    fstream& archivo,
    const SuperBlock& sb,
    int indiceInodo,
    const string& nombre)
{
    Inode inode{};

    if (!leerInodo(
            archivo,
            sb,
            indiceInodo,
            inode))
    {
        return -1;
    }

    if (inode.i_type != '0')
        return -1;

    for (int i = 0; i < 12; i++)
    {
        int indiceBloque =
            inode.i_block[i];

        if (indiceBloque == -1)
            continue;

        FolderBlock bloque{};

        if (!leerFolderBlock(
                archivo,
                sb,
                indiceBloque,
                bloque))
        {
            continue;
        }

        for (int j = 0; j < 4; j++)
        {
            if (bloque.b_content[j].b_inodo == -1)
                continue;

            string nombreActual(
                bloque.b_content[j].b_name
            );

            if (nombreActual == nombre)
            {
                return
                    bloque.b_content[j].b_inodo;
            }
        }
    }

    return -1;
}


// ======================================================
// VERIFICAR PERMISO DE ESCRITURA
// ======================================================

static bool tienePermisoEscritura(
    const Inode& inode,
    const Session& sesion)
{
    // Root
    if (sesion.uid == 1)
        return true;

    int permiso;

    if (inode.i_uid == sesion.uid)
    {
        permiso =
            inode.i_perm[0] - '0';
    }
    else if (inode.i_gid == sesion.gid)
    {
        permiso =
            inode.i_perm[1] - '0';
    }
    else
    {
        permiso =
            inode.i_perm[2] - '0';
    }

    // Bit de escritura = 2
    return (permiso & 2) != 0;
}


// ======================================================
// CREAR INODO DE CARPETA
// ======================================================

static Inode crearInodoCarpeta(
    const Session& sesion,
    int bloque)
{
    Inode inode =
        crearInodoVacio();

    inode.i_uid =
        sesion.uid;

    inode.i_gid =
        sesion.gid;

    inode.i_s =
        0;

    inode.i_atime =
        time(nullptr);

    inode.i_ctime =
        time(nullptr);

    inode.i_mtime =
        time(nullptr);

    inode.i_block[0] =
        bloque;

    inode.i_type =
        '0';

    inode.i_perm[0] = '6';
    inode.i_perm[1] = '6';
    inode.i_perm[2] = '4';

    return inode;
}


// ======================================================
// CREAR BLOQUE DE CARPETA
// ======================================================

static FolderBlock crearBloqueCarpeta(
    int inodoActual,
    int inodoPadre)
{
    FolderBlock bloque =
        crearFolderBlockVacio();

    strncpy(
        bloque.b_content[0].b_name,
        ".",
        sizeof(
            bloque.b_content[0].b_name
        ) - 1
    );

    bloque.b_content[0].b_inodo =
        inodoActual;


    strncpy(
        bloque.b_content[1].b_name,
        "..",
        sizeof(
            bloque.b_content[1].b_name
        ) - 1
    );

    bloque.b_content[1].b_inodo =
        inodoPadre;

    return bloque;
}


// ======================================================
// AGREGAR ENTRADA A CARPETA PADRE
// ======================================================

static bool agregarEntradaCarpeta(
    fstream& archivo,
    SuperBlock& sb,
    int indicePadre,
    const string& nombre,
    int indiceNuevoInodo)
{
    Inode padre{};

    if (!leerInodo(
            archivo,
            sb,
            indicePadre,
            padre))
    {
        return false;
    }


    // --------------------------------------------------
    // Buscar espacio en bloques existentes
    // --------------------------------------------------

    for (int i = 0; i < 12; i++)
    {
        int indiceBloque =
            padre.i_block[i];

        if (indiceBloque == -1)
            continue;

        FolderBlock bloque{};

        if (!leerFolderBlock(
                archivo,
                sb,
                indiceBloque,
                bloque))
        {
            return false;
        }

        for (int j = 0; j < 4; j++)
        {
            if (bloque.b_content[j].b_inodo != -1)
                continue;

            memset(
                bloque.b_content[j].b_name,
                0,
                sizeof(
                    bloque.b_content[j].b_name
                )
            );

            strncpy(
                bloque.b_content[j].b_name,
                nombre.c_str(),
                sizeof(
                    bloque.b_content[j].b_name
                ) - 1
            );

            bloque.b_content[j].b_inodo =
                indiceNuevoInodo;

            padre.i_mtime =
                time(nullptr);

            if (!escribirFolderBlock(
                    archivo,
                    sb,
                    indiceBloque,
                    bloque))
            {
                return false;
            }

            return escribirInodo(
                archivo,
                sb,
                indicePadre,
                padre
            );
        }
    }


    // --------------------------------------------------
    // No había espacio: crear otro FolderBlock
    // --------------------------------------------------

    int apuntadorLibre = -1;

    for (int i = 0; i < 12; i++)
    {
        if (padre.i_block[i] == -1)
        {
            apuntadorLibre = i;
            break;
        }
    }

    if (apuntadorLibre == -1)
        return false;


    int nuevoBloque =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_block_start,
            sb.s_blocks_count
        );

    if (nuevoBloque == -1)
        return false;


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
        nombre.c_str(),
        sizeof(
            bloque.b_content[0].b_name
        ) - 1
    );

    bloque.b_content[0].b_inodo =
        indiceNuevoInodo;


    if (!escribirFolderBlock(
            archivo,
            sb,
            nuevoBloque,
            bloque))
    {
        return false;
    }


    if (!marcarBitmap(
            archivo,
            sb.s_bm_block_start,
            nuevoBloque))
    {
        return false;
    }


    padre.i_block[apuntadorLibre] =
        nuevoBloque;

    padre.i_mtime =
        time(nullptr);


    if (!escribirInodo(
            archivo,
            sb,
            indicePadre,
            padre))
    {
        return false;
    }


    sb.s_free_blocks_count--;

    return true;
}


// ======================================================
// CREAR UNA CARPETA
// ======================================================

static int crearCarpeta(
    fstream& archivo,
    SuperBlock& sb,
    const Session& sesion,
    int indicePadre,
    const string& nombre)
{
    if (nombre.empty())
        return -1;

    /*
        b_name solamente tiene 12 bytes.

        Dejamos uno para '\0'.
    */

    if (nombre.size() >= 12)
        return -1;


    // Buscar inode libre

    int nuevoInodo =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_inode_start,
            sb.s_inodes_count
        );

    if (nuevoInodo == -1)
        return -1;


    // Buscar bloque libre

    int nuevoBloque =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_block_start,
            sb.s_blocks_count
        );

    if (nuevoBloque == -1)
        return -1;


    // Crear estructuras

    Inode inode =
        crearInodoCarpeta(
            sesion,
            nuevoBloque
        );

    FolderBlock bloque =
        crearBloqueCarpeta(
            nuevoInodo,
            indicePadre
        );


    // Escribir inode

    if (!escribirInodo(
            archivo,
            sb,
            nuevoInodo,
            inode))
    {
        return -1;
    }


    // Escribir bloque

    if (!escribirFolderBlock(
            archivo,
            sb,
            nuevoBloque,
            bloque))
    {
        return -1;
    }


    // Marcar bitmap inode

    if (!marcarBitmap(
            archivo,
            sb.s_bm_inode_start,
            nuevoInodo))
    {
        return -1;
    }


    // Marcar bitmap bloque

    if (!marcarBitmap(
            archivo,
            sb.s_bm_block_start,
            nuevoBloque))
    {
        return -1;
    }


    // Agregar al padre

    if (!agregarEntradaCarpeta(
            archivo,
            sb,
            indicePadre,
            nombre,
            nuevoInodo))
    {
        return -1;
    }


    // Actualizar contadores

    sb.s_free_inodes_count--;
    sb.s_free_blocks_count--;


    return nuevoInodo;
}


// ======================================================
// MKDIR
// ======================================================

string ejecutarMkdir(
    const MkdirParams& params)
{
    // --------------------------------------------------
    // Validar path
    // --------------------------------------------------

    if (params.path.empty())
    {
        return
            "Error: debe especificar -path.";
    }

    if (params.path[0] != '/')
    {
        return
            "Error: -path debe ser una ruta absoluta.";
    }


    // --------------------------------------------------
    // Sesión
    // --------------------------------------------------

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
    {
        return
            "Error: debe iniciar sesión para utilizar MKDIR.";
    }


    // --------------------------------------------------
    // Partición
    // --------------------------------------------------

    MountedPartition mounted;

    if (!buscarParticionMontada(
            sesion.partitionId,
            mounted))
    {
        return
            "Error: no se encontró la partición de la sesión.";
    }


    // --------------------------------------------------
    // Abrir disco
    // --------------------------------------------------

    fstream archivo(
        mounted.path,
        ios::binary |
        ios::in |
        ios::out
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    // --------------------------------------------------
    // SuperBlock
    // --------------------------------------------------

    SuperBlock sb{};

    if (!leerSuperBlock(
            archivo,
            mounted.partition.part_start,
            sb))
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene EXT2 válido.";
    }


    // --------------------------------------------------
    // Dividir ruta
    // --------------------------------------------------

    vector<string> partes =
        dividirRuta(params.path);

    if (partes.empty())
    {
        archivo.close();

        return
            "Error: no se puede crear la carpeta raíz.";
    }


    int actual = 0;


    // --------------------------------------------------
    // Recorrer ruta
    // --------------------------------------------------

    for (
        size_t i = 0;
        i < partes.size();
        i++
    )
    {
        const string& nombre =
            partes[i];

        bool esUltima =
            i == partes.size() - 1;


        int encontrado =
            buscarEntrada(
                archivo,
                sb,
                actual,
                nombre
            );


        // ----------------------------------------------
        // Ya existe
        // ----------------------------------------------

        if (encontrado != -1)
        {
            Inode existente{};

            if (!leerInodo(
                    archivo,
                    sb,
                    encontrado,
                    existente))
            {
                archivo.close();

                return
                    "Error: no se pudo leer una carpeta de la ruta.";
            }


            if (existente.i_type != '0')
            {
                archivo.close();

                return
                    "Error: "
                    + nombre
                    + " no es una carpeta.";
            }


            /*
                Si es la carpeta final y ya existe,
                según la especificación no hacemos nada.
            */

            if (esUltima)
            {
                archivo.close();

                return
                    "La carpeta ya existe.";
            }


            actual =
                encontrado;

            continue;
        }


        // ----------------------------------------------
        // No existe y no es la última
        // ----------------------------------------------

        if (!esUltima &&
            !params.p)
        {
            archivo.close();

            return
                "Error: no existe la carpeta padre "
                + nombre
                + ". Use -p para crear las carpetas padres.";
        }


        // ----------------------------------------------
        // Verificar permiso sobre padre
        // ----------------------------------------------

        Inode padre{};

        if (!leerInodo(
                archivo,
                sb,
                actual,
                padre))
        {
            archivo.close();

            return
                "Error: no se pudo leer la carpeta padre.";
        }


        if (!tienePermisoEscritura(
                padre,
                sesion))
        {
            archivo.close();

            return
                "Error: el usuario no tiene permiso de escritura en la carpeta padre.";
        }


        // ----------------------------------------------
        // Crear carpeta
        // ----------------------------------------------

        int nuevo =
            crearCarpeta(
                archivo,
                sb,
                sesion,
                actual,
                nombre
            );


        if (nuevo == -1)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta "
                + nombre
                + ".";
        }


        actual =
            nuevo;
    }


    // --------------------------------------------------
    // Actualizar first inode/block
    // --------------------------------------------------

    int siguienteInodo =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_inode_start,
            sb.s_inodes_count
        );

    if (siguienteInodo != -1)
    {
        sb.s_firts_ino =
            sb.s_inode_start +
            siguienteInodo *
            static_cast<int>(sizeof(Inode));
    }


    int siguienteBloque =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_block_start,
            sb.s_blocks_count
        );

    if (siguienteBloque != -1)
    {
        sb.s_first_blo =
            sb.s_block_start +
            siguienteBloque *
            static_cast<int>(sizeof(FileBlock));
    }


    // --------------------------------------------------
    // Guardar SuperBlock
    // --------------------------------------------------

    if (!escribirSuperBlock(
            archivo,
            mounted.partition.part_start,
            sb))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar el SuperBlock.";
    }


    archivo.flush();
    archivo.close();


    return
        "Carpeta creada correctamente: "
        + params.path;
}