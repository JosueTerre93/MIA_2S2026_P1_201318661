#include "Mkfile.h"

#include "Mount.h"
#include "Session.h"

#include "../Estructuras/Ext2Structures.h"
#include "../Utils/Ext2Utils.h"

#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <climits>

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
// LEER SUPER BLOCK
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
// ESCRIBIR SUPER BLOCK
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
// ESCRIBIR FILE BLOCK
// ======================================================

static bool escribirFileBlock(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const FileBlock& bloque)
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
        sizeof(FileBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// LEER POINTER BLOCK
// ======================================================

static bool leerPointerBlock(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    PointerBlock& bloque)
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
        sizeof(PointerBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR POINTER BLOCK
// ======================================================

static bool escribirPointerBlock(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const PointerBlock& bloque)
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
        sizeof(PointerBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// POINTER BLOCK VACIO
// ======================================================

static PointerBlock crearPointerBlockVacioLocal()
{
    PointerBlock bloque{};

    for (int i = 0; i < 16; i++)
    {
        bloque.b_pointers[i] = -1;
    }

    return bloque;
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
        char estado = 0;

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
    int indice,
    char valor)
{
    archivo.clear();

    archivo.seekp(
        inicio + indice
    );

    if (!archivo)
        return false;

    archivo.write(
        &valor,
        sizeof(char)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// RESERVAR INODO
// ======================================================

static int reservarInodo(
    fstream& archivo,
    SuperBlock& sb)
{
    if (sb.s_free_inodes_count <= 0)
        return -1;

    int indice =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_inode_start,
            sb.s_inodes_count
        );

    if (indice == -1)
        return -1;

    if (!marcarBitmap(
            archivo,
            sb.s_bm_inode_start,
            indice,
            '1'))
    {
        return -1;
    }

    sb.s_free_inodes_count--;

    return indice;
}


// ======================================================
// RESERVAR BLOQUE
// ======================================================

static int reservarBloque(
    fstream& archivo,
    SuperBlock& sb)
{
    if (sb.s_free_blocks_count <= 0)
        return -1;

    int indice =
        buscarLibreBitmap(
            archivo,
            sb.s_bm_block_start,
            sb.s_blocks_count
        );

    if (indice == -1)
        return -1;

    if (!marcarBitmap(
            archivo,
            sb.s_bm_block_start,
            indice,
            '1'))
    {
        return -1;
    }

    sb.s_free_blocks_count--;

    return indice;
}


// ======================================================
// NOMBRE DE CONTENT
// ======================================================

static string obtenerNombre(
    const Content& content)
{
    size_t longitud = 0;

    while (
        longitud < sizeof(content.b_name) &&
        content.b_name[longitud] != '\0'
    )
    {
        longitud++;
    }

    return string(
        content.b_name,
        longitud
    );
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

    /*
        De momento los directorios utilizan
        apuntadores directos.

        Es suficiente para una cantidad grande
        de entradas por carpeta.
    */

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

            if (obtenerNombre(
                    bloque.b_content[j])
                == nombre)
            {
                return
                    bloque.b_content[j].b_inodo;
            }
        }
    }

    return -1;
}


// ======================================================
// PERMISO DE ESCRITURA
// ======================================================

static bool tienePermisoEscritura(
    const Inode& inode,
    const Session& sesion)
{
    /*
        Root tiene acceso total.
    */

    if (sesion.uid == 1)
        return true;

    int permiso = 0;

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

    /*
        Escritura = bit 2.
    */

    return
        (permiso & 2) != 0;
}


// ======================================================
// AGREGAR ENTRADA A CARPETA
// ======================================================

static bool agregarEntradaCarpeta(
    fstream& archivo,
    SuperBlock& sb,
    int indicePadre,
    const string& nombre,
    int indiceHijo)
{
    if (nombre.empty() ||
        nombre.size() >= 12)
    {
        return false;
    }

    Inode padre{};

    if (!leerInodo(
            archivo,
            sb,
            indicePadre,
            padre))
    {
        return false;
    }

    if (padre.i_type != '0')
        return false;


    // ==================================================
    // BUSCAR ESPACIO EN BLOQUES EXISTENTES
    // ==================================================

    for (int i = 0; i < 12; i++)
    {
        if (padre.i_block[i] == -1)
            continue;

        FolderBlock bloque{};

        if (!leerFolderBlock(
                archivo,
                sb,
                padre.i_block[i],
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
                indiceHijo;

            if (!escribirFolderBlock(
                    archivo,
                    sb,
                    padre.i_block[i],
                    bloque))
            {
                return false;
            }

            padre.i_mtime =
                time(nullptr);

            return escribirInodo(
                archivo,
                sb,
                indicePadre,
                padre
            );
        }
    }


    // ==================================================
    // NECESITAMOS UN NUEVO FOLDER BLOCK
    // ==================================================

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
        reservarBloque(
            archivo,
            sb
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
        indiceHijo;

    if (!escribirFolderBlock(
            archivo,
            sb,
            nuevoBloque,
            bloque))
    {
        return false;
    }

    padre.i_block[apuntadorLibre] =
        nuevoBloque;

    padre.i_mtime =
        time(nullptr);

    return escribirInodo(
        archivo,
        sb,
        indicePadre,
        padre
    );
}


// ======================================================
// CREAR CARPETA PADRE
// ======================================================

static int crearCarpetaPadre(
    fstream& archivo,
    SuperBlock& sb,
    const Session& sesion,
    int indicePadre,
    const string& nombre)
{
    if (nombre.empty() ||
        nombre.size() >= 12)
    {
        return -1;
    }

    int nuevoInodo =
        reservarInodo(
            archivo,
            sb
        );

    if (nuevoInodo == -1)
        return -1;

    int nuevoBloque =
        reservarBloque(
            archivo,
            sb
        );

    if (nuevoBloque == -1)
        return -1;


    // ==================================================
    // INODO DE CARPETA
    // ==================================================

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
        nuevoBloque;

    inode.i_type =
        '0';

    inode.i_perm[0] = '6';
    inode.i_perm[1] = '6';
    inode.i_perm[2] = '4';


    // ==================================================
    // FOLDER BLOCK
    // ==================================================

    FolderBlock bloque =
        crearFolderBlockVacio();


    // .
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
        ) - 1
    );

    bloque.b_content[0].b_inodo =
        nuevoInodo;


    // ..
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
        ) - 1
    );

    bloque.b_content[1].b_inodo =
        indicePadre;


    if (!escribirInodo(
            archivo,
            sb,
            nuevoInodo,
            inode))
    {
        return -1;
    }

    if (!escribirFolderBlock(
            archivo,
            sb,
            nuevoBloque,
            bloque))
    {
        return -1;
    }

    if (!agregarEntradaCarpeta(
            archivo,
            sb,
            indicePadre,
            nombre,
            nuevoInodo))
    {
        return -1;
    }

    return nuevoInodo;
}


// ======================================================
// GENERAR CONTENIDO POR SIZE
// ======================================================

static string generarContenido(
    long long tamanio)
{
    string contenido;

    contenido.reserve(
        static_cast<size_t>(tamanio)
    );

    for (
        long long i = 0;
        i < tamanio;
        i++
    )
    {
        contenido +=
            static_cast<char>(
                '0' + (i % 10)
            );
    }

    return contenido;
}


// ======================================================
// LEER ARCHIVO DEL SISTEMA HOST
// ======================================================

static bool cargarContenidoHost(
    const string& ruta,
    string& contenido)
{
    ifstream archivo(
        ruta,
        ios::binary
    );

    if (!archivo.is_open())
        return false;

    contenido.assign(
        istreambuf_iterator<char>(archivo),
        istreambuf_iterator<char>()
    );

    archivo.close();

    return true;
}


// ======================================================
// ASIGNAR BLOQUE DE DATOS A INODO
//
// 0 - 11   -> directos
// 12       -> indirecto simple
// 13       -> indirecto doble
// 14       -> indirecto triple
// ======================================================

static bool asignarBloqueArchivo(
    fstream& archivo,
    SuperBlock& sb,
    Inode& inode,
    int numeroLogico,
    int bloqueDatos)
{
    // ==================================================
    // DIRECTOS
    // ==================================================

    if (numeroLogico < 12)
    {
        inode.i_block[numeroLogico] =
            bloqueDatos;

        return true;
    }

    numeroLogico -= 12;


    // ==================================================
    // INDIRECTO SIMPLE
    // 16 BLOQUES
    // ==================================================

    if (numeroLogico < 16)
    {
        if (inode.i_block[12] == -1)
        {
            int bloqueApuntadores =
                reservarBloque(
                    archivo,
                    sb
                );

            if (bloqueApuntadores == -1)
                return false;

            PointerBlock nuevo =
                crearPointerBlockVacioLocal();

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    bloqueApuntadores,
                    nuevo))
            {
                return false;
            }

            inode.i_block[12] =
                bloqueApuntadores;
        }

        PointerBlock apuntadores{};

        if (!leerPointerBlock(
                archivo,
                sb,
                inode.i_block[12],
                apuntadores))
        {
            return false;
        }

        apuntadores.b_pointers[numeroLogico] =
            bloqueDatos;

        return escribirPointerBlock(
            archivo,
            sb,
            inode.i_block[12],
            apuntadores
        );
    }

    numeroLogico -= 16;


    // ==================================================
    // INDIRECTO DOBLE
    // 16 * 16 = 256 BLOQUES
    // ==================================================

    if (numeroLogico < 256)
    {
        int indice1 =
            numeroLogico / 16;

        int indice2 =
            numeroLogico % 16;


        if (inode.i_block[13] == -1)
        {
            int bloqueNivel1 =
                reservarBloque(
                    archivo,
                    sb
                );

            if (bloqueNivel1 == -1)
                return false;

            PointerBlock nuevo =
                crearPointerBlockVacioLocal();

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    bloqueNivel1,
                    nuevo))
            {
                return false;
            }

            inode.i_block[13] =
                bloqueNivel1;
        }


        PointerBlock nivel1{};

        if (!leerPointerBlock(
                archivo,
                sb,
                inode.i_block[13],
                nivel1))
        {
            return false;
        }


        if (nivel1.b_pointers[indice1] == -1)
        {
            int bloqueNivel2 =
                reservarBloque(
                    archivo,
                    sb
                );

            if (bloqueNivel2 == -1)
                return false;

            PointerBlock nuevo =
                crearPointerBlockVacioLocal();

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    bloqueNivel2,
                    nuevo))
            {
                return false;
            }

            nivel1.b_pointers[indice1] =
                bloqueNivel2;

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    inode.i_block[13],
                    nivel1))
            {
                return false;
            }
        }


        PointerBlock nivel2{};

        if (!leerPointerBlock(
                archivo,
                sb,
                nivel1.b_pointers[indice1],
                nivel2))
        {
            return false;
        }

        nivel2.b_pointers[indice2] =
            bloqueDatos;

        return escribirPointerBlock(
            archivo,
            sb,
            nivel1.b_pointers[indice1],
            nivel2
        );
    }

    numeroLogico -= 256;


    // ==================================================
    // INDIRECTO TRIPLE
    // 16 * 16 * 16 = 4096 BLOQUES
    // ==================================================

    if (numeroLogico < 4096)
    {
        int indice1 =
            numeroLogico / 256;

        int restante =
            numeroLogico % 256;

        int indice2 =
            restante / 16;

        int indice3 =
            restante % 16;


        // ----------------------------------------------
        // NIVEL 1
        // ----------------------------------------------

        if (inode.i_block[14] == -1)
        {
            int bloqueNivel1 =
                reservarBloque(
                    archivo,
                    sb
                );

            if (bloqueNivel1 == -1)
                return false;

            PointerBlock nuevo =
                crearPointerBlockVacioLocal();

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    bloqueNivel1,
                    nuevo))
            {
                return false;
            }

            inode.i_block[14] =
                bloqueNivel1;
        }


        PointerBlock nivel1{};

        if (!leerPointerBlock(
                archivo,
                sb,
                inode.i_block[14],
                nivel1))
        {
            return false;
        }


        // ----------------------------------------------
        // NIVEL 2
        // ----------------------------------------------

        if (nivel1.b_pointers[indice1] == -1)
        {
            int bloqueNivel2 =
                reservarBloque(
                    archivo,
                    sb
                );

            if (bloqueNivel2 == -1)
                return false;

            PointerBlock nuevo =
                crearPointerBlockVacioLocal();

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    bloqueNivel2,
                    nuevo))
            {
                return false;
            }

            nivel1.b_pointers[indice1] =
                bloqueNivel2;

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    inode.i_block[14],
                    nivel1))
            {
                return false;
            }
        }


        PointerBlock nivel2{};

        if (!leerPointerBlock(
                archivo,
                sb,
                nivel1.b_pointers[indice1],
                nivel2))
        {
            return false;
        }


        // ----------------------------------------------
        // NIVEL 3
        // ----------------------------------------------

        if (nivel2.b_pointers[indice2] == -1)
        {
            int bloqueNivel3 =
                reservarBloque(
                    archivo,
                    sb
                );

            if (bloqueNivel3 == -1)
                return false;

            PointerBlock nuevo =
                crearPointerBlockVacioLocal();

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    bloqueNivel3,
                    nuevo))
            {
                return false;
            }

            nivel2.b_pointers[indice2] =
                bloqueNivel3;

            if (!escribirPointerBlock(
                    archivo,
                    sb,
                    nivel1.b_pointers[indice1],
                    nivel2))
            {
                return false;
            }
        }


        PointerBlock nivel3{};

        if (!leerPointerBlock(
                archivo,
                sb,
                nivel2.b_pointers[indice2],
                nivel3))
        {
            return false;
        }

        nivel3.b_pointers[indice3] =
            bloqueDatos;

        return escribirPointerBlock(
            archivo,
            sb,
            nivel2.b_pointers[indice2],
            nivel3
        );
    }


    return false;
}


// ======================================================
// CREAR ARCHIVO
// ======================================================

static bool crearArchivo(
    fstream& archivo,
    SuperBlock& sb,
    const Session& sesion,
    int indicePadre,
    const string& nombre,
    const string& contenido)
{
    if (nombre.empty() ||
        nombre.size() >= 12)
    {
        return false;
    }

    if (contenido.size() >
        static_cast<size_t>(INT_MAX))
    {
        return false;
    }


    // ==================================================
    // RESERVAR INODO
    // ==================================================

    int nuevoInodo =
        reservarInodo(
            archivo,
            sb
        );

    if (nuevoInodo == -1)
        return false;


    // ==================================================
    // CREAR INODO
    // ==================================================

    Inode inode =
        crearInodoVacio();

    inode.i_uid =
        sesion.uid;

    inode.i_gid =
        sesion.gid;

    inode.i_s =
        static_cast<int>(
            contenido.size()
        );

    inode.i_atime =
        time(nullptr);

    inode.i_ctime =
        time(nullptr);

    inode.i_mtime =
        time(nullptr);

    inode.i_type =
        '1';

    inode.i_perm[0] = '6';
    inode.i_perm[1] = '6';
    inode.i_perm[2] = '4';


    // ==================================================
    // ESCRIBIR CONTENIDO EN BLOQUES
    // ==================================================

    size_t posicion = 0;
    int numeroLogico = 0;

    while (posicion < contenido.size())
    {
        /*
            Capacidad total:

            12 directos
            16 simple
            256 doble
            4096 triple
        */

        if (numeroLogico >=
            12 + 16 + 256 + 4096)
        {
            return false;
        }


        int bloqueDatos =
            reservarBloque(
                archivo,
                sb
            );

        if (bloqueDatos == -1)
            return false;


        FileBlock bloque{};

        memset(
            bloque.b_content,
            0,
            sizeof(bloque.b_content)
        );


        size_t cantidad =
            min(
                sizeof(bloque.b_content),
                contenido.size() - posicion
            );


        memcpy(
            bloque.b_content,
            contenido.data() + posicion,
            cantidad
        );


        if (!escribirFileBlock(
                archivo,
                sb,
                bloqueDatos,
                bloque))
        {
            return false;
        }


        if (!asignarBloqueArchivo(
                archivo,
                sb,
                inode,
                numeroLogico,
                bloqueDatos))
        {
            return false;
        }


        posicion += cantidad;
        numeroLogico++;
    }


    // ==================================================
    // GUARDAR INODO
    // ==================================================

    if (!escribirInodo(
            archivo,
            sb,
            nuevoInodo,
            inode))
    {
        return false;
    }


    // ==================================================
    // AGREGARLO AL PADRE
    // ==================================================

    if (!agregarEntradaCarpeta(
            archivo,
            sb,
            indicePadre,
            nombre,
            nuevoInodo))
    {
        return false;
    }


    return true;
}


// ======================================================
// ACTUALIZAR PRIMEROS LIBRES
// ======================================================

static void actualizarPrimerosLibres(
    fstream& archivo,
    SuperBlock& sb)
{
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
            static_cast<int>(
                sizeof(Inode)
            );
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
            static_cast<int>(
                sizeof(FileBlock)
            );
    }
}


// ======================================================
// MKFILE
// ======================================================

string ejecutarMkfile(
    const MkfileParams& params)
{
    // ==================================================
    // VALIDACIONES
    // ==================================================

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

    if (params.size < 0)
    {
        return
            "Error: -size no puede ser negativo.";
    }


    // ==================================================
    // SESION
    // ==================================================

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
    {
        return
            "Error: debe iniciar sesión para utilizar MKFILE.";
    }


    // ==================================================
    // DETERMINAR CONTENIDO
    // ==================================================

    string contenido;

    /*
        -cont tiene prioridad sobre -size.
    */

    if (!params.cont.empty())
    {
        if (!cargarContenidoHost(
                params.cont,
                contenido))
        {
            return
                "Error: no existe o no se pudo leer el archivo indicado en -cont: "
                + params.cont;
        }
    }
    else
    {
        /*
            Si no viene size, params.size = 0.
        */

        contenido =
            generarContenido(
                params.size
            );
    }


    // ==================================================
    // BUSCAR PARTICION
    // ==================================================

    MountedPartition mounted;

    if (!buscarParticionMontada(
            sesion.partitionId,
            mounted))
    {
        return
            "Error: no se encontró la partición de la sesión.";
    }


    // ==================================================
    // ABRIR DISCO
    // ==================================================

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


    // ==================================================
    // SUPER BLOCK
    // ==================================================

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
            "Error: la partición no contiene un sistema de archivos EXT2 válido.";
    }


    // ==================================================
    // DIVIDIR PATH
    // ==================================================

    vector<string> partes =
        dividirRuta(params.path);

    if (partes.empty())
    {
        archivo.close();

        return
            "Error: debe indicar el nombre del archivo.";
    }


    string nombreArchivo =
        partes.back();

    if (nombreArchivo.size() >= 12)
    {
        archivo.close();

        return
            "Error: el nombre del archivo no puede superar 11 caracteres.";
    }


    int actual = 0;


    // ==================================================
    // RECORRER CARPETAS PADRE
    // ==================================================

    for (
        size_t i = 0;
        i + 1 < partes.size();
        i++
    )
    {
        const string& nombre =
            partes[i];


        if (nombre.size() >= 12)
        {
            archivo.close();

            return
                "Error: el nombre de carpeta "
                + nombre
                + " supera 11 caracteres.";
        }


        int encontrado =
            buscarEntrada(
                archivo,
                sb,
                actual,
                nombre
            );


        // ==============================================
        // EXISTE
        // ==============================================

        if (encontrado != -1)
        {
            Inode inodeExistente{};

            if (!leerInodo(
                    archivo,
                    sb,
                    encontrado,
                    inodeExistente))
            {
                archivo.close();

                return
                    "Error: no se pudo leer "
                    + nombre
                    + ".";
            }


            if (inodeExistente.i_type != '0')
            {
                archivo.close();

                return
                    "Error: "
                    + nombre
                    + " no es una carpeta.";
            }


            actual =
                encontrado;

            continue;
        }


        // ==============================================
        // NO EXISTE
        // ==============================================

        if (!params.r)
        {
            archivo.close();

            return
                "Error: no existe la carpeta padre "
                + nombre
                + ". Use -r para crear las carpetas padres.";
        }


        // ==============================================
        // PERMISOS SOBRE PADRE
        // ==============================================

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


        // ==============================================
        // CREAR PADRE
        // ==============================================

        int nuevaCarpeta =
            crearCarpetaPadre(
                archivo,
                sb,
                sesion,
                actual,
                nombre
            );

        if (nuevaCarpeta == -1)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta padre "
                + nombre
                + ".";
        }


        actual =
            nuevaCarpeta;
    }


    // ==================================================
    // VERIFICAR SI YA EXISTE ARCHIVO
    // ==================================================

    int existente =
        buscarEntrada(
            archivo,
            sb,
            actual,
            nombreArchivo
        );

    if (existente != -1)
    {
        Inode inodeExistente{};

        if (!leerInodo(
                archivo,
                sb,
                existente,
                inodeExistente))
        {
            archivo.close();

            return
                "Error: no se pudo leer el archivo existente.";
        }


        archivo.close();


        if (inodeExistente.i_type == '0')
        {
            return
                "Error: ya existe una carpeta llamada "
                + nombreArchivo
                + ".";
        }


        return
            "El archivo "
            + params.path
            + " ya existe. ¿Desea sobreescribirlo?";
    }


    // ==================================================
    // PERMISO SOBRE CARPETA PADRE
    // ==================================================

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


    // ==================================================
    // CREAR ARCHIVO
    // ==================================================

    if (!crearArchivo(
            archivo,
            sb,
            sesion,
            actual,
            nombreArchivo,
            contenido))
    {
        archivo.close();

        return
            "Error: no se pudo crear el archivo.";
    }


    // ==================================================
    // ACTUALIZAR SUPER BLOCK
    // ==================================================

    actualizarPrimerosLibres(
        archivo,
        sb
    );


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
        "Archivo creado correctamente: "
        + params.path
        + " ("
        + to_string(contenido.size())
        + " bytes).";
}