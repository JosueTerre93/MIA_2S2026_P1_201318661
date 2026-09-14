#include "Cat.h"

#include "Mount.h"
#include "Session.h"

#include "../Estructuras/Ext2Structures.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;


/*
    ============================================================
    FUNCIONES AUXILIARES
    ============================================================
*/


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


/*
    Leer SuperBlock
*/

static bool leerSuperBlock(
    fstream& archivo,
    int inicioParticion,
    SuperBlock& sb)
{
    archivo.clear();

    archivo.seekg(
        inicioParticion
    );

    if (!archivo)
        return false;

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    return static_cast<bool>(archivo);
}


/*
    Leer inode
*/

static bool leerInodo(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    Inode& inode)
{
    int posicion =
        sb.s_inode_start +
        indice *
        static_cast<int>(
            sizeof(Inode)
        );

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


/*
    Leer bloque de carpeta
*/

static bool leerBloqueCarpeta(
    fstream& archivo,
    const SuperBlock& sb,
    int indiceBloque,
    FolderBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indiceBloque *
        static_cast<int>(
            sizeof(FolderBlock)
        );

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


/*
    Leer bloque de archivo
*/

static bool leerBloqueArchivo(
    fstream& archivo,
    const SuperBlock& sb,
    int indiceBloque,
    FileBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indiceBloque *
        static_cast<int>(
            sizeof(FileBlock)
        );

    archivo.clear();

    archivo.seekg(posicion);

    if (!archivo)
        return false;

    archivo.read(
        reinterpret_cast<char*>(&bloque),
        sizeof(FileBlock)
    );

    return static_cast<bool>(archivo);
}


/*
    ============================================================
    BUSCAR UNA ENTRADA DENTRO DE UNA CARPETA
    ============================================================

    Recibe el inode de una carpeta y busca:

        usuarios
        home
        archivo.txt
        etc.

    Devuelve el índice del inode encontrado.
*/

static int buscarEnCarpeta(
    fstream& archivo,
    const SuperBlock& sb,
    int indiceInodoCarpeta,
    const string& nombre)
{
    Inode inode{};

    if (!leerInodo(
            archivo,
            sb,
            indiceInodoCarpeta,
            inode))
    {
        return -1;
    }

    /*
        Por ahora utilizamos los
        12 apuntadores directos.
    */

    for (int i = 0; i < 12; i++)
    {
        int indiceBloque =
            inode.i_block[i];

        if (indiceBloque == -1)
            continue;

        FolderBlock bloque{};

        if (!leerBloqueCarpeta(
                archivo,
                sb,
                indiceBloque,
                bloque))
        {
            continue;
        }

        /*
            Normalmente un FolderBlock
            contiene 4 entradas.
        */

        for (int j = 0; j < 4; j++)
        {
            if (bloque.b_content[j].b_inodo == -1)
                continue;

            string nombreEntrada(
                bloque.b_content[j].b_name
            );

            if (nombreEntrada == nombre)
            {
                return
                    bloque
                    .b_content[j]
                    .b_inodo;
            }
        }
    }

    return -1;
}


/*
    ============================================================
    BUSCAR INODE MEDIANTE RUTA
    ============================================================

    Ejemplo:

        /users.txt

    partes:

        users.txt


    Ejemplo:

        /home/josue/prueba.txt

    partes:

        home
        josue
        prueba.txt
*/

static int buscarInodoPorRuta(
    fstream& archivo,
    const SuperBlock& sb,
    const string& ruta)
{
    if (ruta.empty())
        return -1;

    /*
        La raíz siempre es el inode 0.
    */

    if (ruta == "/")
        return 0;

    vector<string> partes =
        dividirRuta(ruta);

    if (partes.empty())
        return -1;

    int inodeActual = 0;

    for (const string& parte : partes)
    {
        int siguiente =
            buscarEnCarpeta(
                archivo,
                sb,
                inodeActual,
                parte
            );

        if (siguiente == -1)
            return -1;

        inodeActual =
            siguiente;
    }

    return inodeActual;
}


/*
    ============================================================
    LEER CONTENIDO DEL ARCHIVO
    ============================================================
*/

static bool leerContenidoArchivo(
    fstream& archivo,
    const SuperBlock& sb,
    int indiceInodo,
    string& contenido)
{
    Inode inode{};

    if (!leerInodo(
            archivo,
            sb,
            indiceInodo,
            inode))
    {
        return false;
    }

    contenido.clear();

    /*
        Leer apuntadores directos:

        i_block[0]
        ...
        i_block[11]
    */

    for (int i = 0; i < 12; i++)
    {
        int indiceBloque =
            inode.i_block[i];

        if (indiceBloque == -1)
            continue;

        FileBlock bloque{};

        if (!leerBloqueArchivo(
                archivo,
                sb,
                indiceBloque,
                bloque))
        {
            return false;
        }

        for (int j = 0; j < 64; j++)
        {
            if (bloque.b_content[j] == '\0')
                break;

            contenido +=
                bloque.b_content[j];
        }
    }

    /*
        Respetar tamaño real del archivo.
    */

    if (inode.i_s >= 0 &&
        static_cast<int>(
            contenido.size()
        ) > inode.i_s)
    {
        contenido.resize(
            inode.i_s
        );
    }

    return true;
}


/*
    ============================================================
    CAT
    ============================================================
*/

string ejecutarCat(
    const CatParams& params)
{
    /*
        1. Validar que existan archivos.
    */

    if (params.files.empty())
    {
        return
            "Error: debe especificar al menos un archivo.";
    }


    /*
        2. Verificar sesión activa.
    */

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
    {
        return
            "Error: debe iniciar sesión para utilizar CAT.";
    }


    /*
        3. Obtener partición montada.
    */

    MountedPartition mounted;

    if (!buscarParticionMontada(
            sesion.partitionId,
            mounted))
    {
        return
            "Error: no se encontró la partición de la sesión activa.";
    }


    /*
        4. Abrir disco.
    */

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


    /*
        5. Leer SuperBlock.
    */

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


    /*
        Validar EXT2.
    */

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema de archivos EXT2 válido.";
    }


    /*
        6. Leer todos los archivos solicitados.
    */

    string salida;


    for (const string& ruta : params.files)
    {
        /*
            Buscar inode correspondiente.
        */

        int indiceInodo =
            buscarInodoPorRuta(
                archivo,
                sb,
                ruta
            );


        if (indiceInodo == -1)
        {
            salida +=
                "Error: no se encontró el archivo "
                + ruta
                + ".\n";

            continue;
        }


        /*
            Leer contenido.
        */

        string contenido;

        if (!leerContenidoArchivo(
                archivo,
                sb,
                indiceInodo,
                contenido))
        {
            salida +=
                "Error: no se pudo leer el archivo "
                + ruta
                + ".\n";

            continue;
        }


        /*
            Concatenamos el contenido.

            Si CAT recibe varios archivos,
            aparecerán uno después del otro.
        */

        salida += contenido;

        /*
            Para evitar que dos archivos
            queden pegados.
        */

        if (!contenido.empty() &&
            contenido.back() != '\n')
        {
            salida += '\n';
        }
    }


    archivo.close();


    /*
        7. Retornar resultado.
    */

    if (salida.empty())
    {
        return
            "CAT: no se pudo mostrar ningún archivo.";
    }

    return salida;
}