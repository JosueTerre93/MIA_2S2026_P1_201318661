#include "Rmgrp.h"
#include "Session.h"
#include "Mount.h"

#include "../Estructuras/Ext2Structures.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>

using namespace std;


// ======================================================
// DIVIDIR STRING
// ======================================================

static vector<string> dividirRmgrp(
    const string& texto,
    char separador
)
{
    vector<string> resultado;
    string actual;

    for (char c : texto)
    {
        if (c == separador)
        {
            resultado.push_back(actual);
            actual.clear();
        }
        else
        {
            actual += c;
        }
    }

    resultado.push_back(actual);

    return resultado;
}


// ======================================================
// LEER SUPERBLOCK
// ======================================================

static bool leerSuperBlockRmgrp(
    fstream& archivo,
    int inicio,
    SuperBlock& sb
)
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

static bool escribirSuperBlockRmgrp(
    fstream& archivo,
    int inicio,
    const SuperBlock& sb
)
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

static bool leerInodoRmgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    Inode& inode
)
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

static bool escribirInodoRmgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const Inode& inode
)
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
// LEER BLOQUE
// ======================================================

static bool leerBloqueRmgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    FileBlock& bloque
)
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
        sizeof(FileBlock)
    );

    return static_cast<bool>(archivo);
}


// ======================================================
// ESCRIBIR BLOQUE
// ======================================================

static bool escribirBloqueRmgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const FileBlock& bloque
)
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
// LEER USERS.TXT
// ======================================================

static bool leerUsersRmgrp(
    fstream& archivo,
    const SuperBlock& sb,
    Inode& users,
    string& contenido
)
{
    // users.txt está en el inodo 1
    if (!leerInodoRmgrp(
            archivo,
            sb,
            1,
            users))
    {
        return false;
    }

    contenido.clear();

    int restantes = users.i_s;

    for (
        int i = 0;
        i < 12 && restantes > 0;
        i++
    )
    {
        if (users.i_block[i] == -1)
            continue;

        FileBlock bloque{};

        if (!leerBloqueRmgrp(
                archivo,
                sb,
                users.i_block[i],
                bloque))
        {
            return false;
        }

        int cantidad = restantes;

        if (cantidad > 64)
            cantidad = 64;

        contenido.append(
            bloque.b_content,
            cantidad
        );

        restantes -= cantidad;
    }

    return true;
}


// ======================================================
// BUSCAR PRIMER BLOQUE LIBRE
// ======================================================

static int buscarPrimerBloqueLibreRmgrp(
    fstream& archivo,
    const SuperBlock& sb
)
{
    archivo.clear();
    archivo.seekg(sb.s_bm_block_start);

    if (!archivo)
        return -1;

    for (
        int i = 0;
        i < sb.s_blocks_count;
        i++
    )
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
// LIBERAR BLOQUE
// ======================================================

static bool liberarBloqueRmgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice
)
{
    archivo.clear();

    archivo.seekp(
        sb.s_bm_block_start + indice
    );

    if (!archivo)
        return false;

    char libre = '0';

    archivo.write(
        &libre,
        sizeof(char)
    );

    if (!archivo)
        return false;

    /*
        También dejamos el bloque físico en cero.
    */

    FileBlock vacio{};

    memset(
        vacio.b_content,
        0,
        sizeof(vacio.b_content)
    );

    return escribirBloqueRmgrp(
        archivo,
        sb,
        indice,
        vacio
    );
}


// ======================================================
// REESCRIBIR USERS.TXT
// ======================================================

static bool escribirUsersRmgrp(
    fstream& archivo,
    SuperBlock& sb,
    Inode& users,
    const string& contenido
)
{
    int bloquesNecesarios =
        static_cast<int>(
            (contenido.size() + 63) / 64
        );

    if (bloquesNecesarios == 0)
        bloquesNecesarios = 1;

    if (bloquesNecesarios > 12)
        return false;


    // ==================================================
    // REESCRIBIR BLOQUES QUE SE SIGUEN UTILIZANDO
    // ==================================================

    size_t offset = 0;

    for (
        int i = 0;
        i < bloquesNecesarios;
        i++
    )
    {
        if (users.i_block[i] == -1)
        {
            /*
                RMGRP normalmente no debería necesitar
                bloques nuevos porque el archivo se hace
                más pequeño.

                Esto sería una inconsistencia.
            */

            return false;
        }

        FileBlock bloque{};

        memset(
            bloque.b_content,
            0,
            sizeof(bloque.b_content)
        );

        size_t restantes =
            contenido.size() - offset;

        size_t cantidad =
            restantes > 64
                ? 64
                : restantes;

        if (cantidad > 0)
        {
            memcpy(
                bloque.b_content,
                contenido.data() + offset,
                cantidad
            );
        }

        if (!escribirBloqueRmgrp(
                archivo,
                sb,
                users.i_block[i],
                bloque))
        {
            return false;
        }

        offset += cantidad;
    }


    // ==================================================
    // LIBERAR BLOQUES QUE YA NO SE NECESITAN
    // ==================================================

    for (
        int i = bloquesNecesarios;
        i < 12;
        i++
    )
    {
        if (users.i_block[i] == -1)
            continue;

        int bloqueLiberado =
            users.i_block[i];

        if (!liberarBloqueRmgrp(
                archivo,
                sb,
                bloqueLiberado))
        {
            return false;
        }

        users.i_block[i] = -1;

        sb.s_free_blocks_count++;
    }


    // ==================================================
    // ACTUALIZAR INODO
    // ==================================================

    users.i_s =
        static_cast<int>(
            contenido.size()
        );

    users.i_mtime =
        time(nullptr);


    // ==================================================
    // ACTUALIZAR FIRST_BLO
    // ==================================================

    int siguiente =
        buscarPrimerBloqueLibreRmgrp(
            archivo,
            sb
        );

    if (siguiente == -1)
    {
        sb.s_first_blo = -1;
    }
    else
    {
        sb.s_first_blo =
            sb.s_block_start +
            siguiente *
            static_cast<int>(
                sizeof(FileBlock)
            );
    }

    return true;
}


// ======================================================
// RMGRP
// ======================================================

string ejecutarRmgrp(
    const RmgrpParams& params
)
{
    // ==================================================
    // VALIDAR NAME
    // ==================================================

    if (params.name.empty())
    {
        return
            "Error: debe especificar -name.";
    }


    // ==================================================
    // VALIDAR SESIÓN
    // ==================================================

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
    {
        return
            "Error: debe iniciar sesión.";
    }


    // ==================================================
    // SOLO ROOT
    // ==================================================

    if (sesion.user != "root")
    {
        return
            "Error: solamente el usuario root "
            "puede eliminar grupos.";
    }


    // ==================================================
    // BUSCAR PARTICIÓN DE LA SESIÓN
    // ==================================================

    MountedPartition mounted;

    if (!buscarParticionMontada(
            sesion.partitionId,
            mounted))
    {
        return
            "Error: la partición de la sesión "
            "ya no está montada.";
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
    // LEER SUPERBLOCK
    // ==================================================

    SuperBlock sb{};

    if (!leerSuperBlockRmgrp(
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
            "Error: la partición no contiene EXT2.";
    }


    // ==================================================
    // LEER USERS.TXT
    // ==================================================

    Inode users{};
    string contenido;

    if (!leerUsersRmgrp(
            archivo,
            sb,
            users,
            contenido))
    {
        archivo.close();

        return
            "Error: no se pudo leer users.txt.";
    }


    // ==================================================
    // BUSCAR GRUPO
    // ==================================================

    stringstream entrada(contenido);

    string linea;
    string nuevoContenido;

    bool encontrado = false;

    while (getline(entrada, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividirRmgrp(
                linea,
                ','
            );


        /*
            Grupo:

            ID,G,NOMBRE
        */

        if (
            campos.size() == 3 &&
            campos[1] == "G"
        )
        {
            int id = 0;

            try
            {
                id = stoi(campos[0]);
            }
            catch (...)
            {
                id = 0;
            }


            /*
                Solo grupos activos.

                ID = 0 significa eliminado.
            */

            if (
                id != 0 &&
                campos[2] == params.name
            )
            {
                /*
                    Eliminación lógica:
                    cambiar ID por 0.
                */

                linea =
                    "0,G," +
                    campos[2];

                encontrado = true;
            }
        }

        nuevoContenido +=
            linea + "\n";
    }


    // ==================================================
    // NO EXISTE
    // ==================================================

    if (!encontrado)
    {
        archivo.close();

        return
            "Error: el grupo no existe.";
    }


    // ==================================================
    // ESCRIBIR USERS.TXT
    // ==================================================

    if (!escribirUsersRmgrp(
            archivo,
            sb,
            users,
            nuevoContenido))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar users.txt.";
    }


    // ==================================================
    // ACTUALIZAR INODO 1
    // ==================================================

    if (!escribirInodoRmgrp(
            archivo,
            sb,
            1,
            users))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar "
            "el inodo de users.txt.";
    }


    // ==================================================
    // ACTUALIZAR SUPERBLOCK
    // ==================================================

    if (!escribirSuperBlockRmgrp(
            archivo,
            mounted.partition.part_start,
            sb))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar "
            "el SuperBlock.";
    }


    archivo.flush();
    archivo.close();


    return
        "Grupo eliminado correctamente. Grupo: "
        + params.name + ".";
}