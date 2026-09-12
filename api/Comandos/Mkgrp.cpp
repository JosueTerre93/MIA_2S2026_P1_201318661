#include "Mkgrp.h"

#include "Session.h"
#include "Mount.h"

#include "../Estructuras/Ext2Structures.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

static vector<string> dividirMkgrp(
    const string& texto,
    char separador)
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

static bool leerSuperBlockMkgrp(
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

static bool escribirSuperBlockMkgrp(
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

static bool leerInodoMkgrp(
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

static bool escribirInodoMkgrp(
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

static bool leerBloqueMkgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    FileBlock& bloque)
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

static bool escribirBloqueMkgrp(
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

static bool leerUsersMkgrp(
    fstream& archivo,
    const SuperBlock& sb,
    Inode& users,
    string& contenido)
{
    // Inodo 1 = users.txt
    if (!leerInodoMkgrp(
            archivo,
            sb,
            1,
            users))
    {
        return false;
    }

    contenido.clear();

    int restantes = users.i_s;

    for (int i = 0;
         i < 12 && restantes > 0;
         i++)
    {
        if (users.i_block[i] == -1)
            continue;

        FileBlock bloque{};

        if (!leerBloqueMkgrp(
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

static bool existeGrupoMkgrp(
    const string& contenido,
    const string& nombre)
{
    stringstream ss(contenido);
    string linea;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividirMkgrp(linea, ',');

        if (campos.size() != 3)
            continue;

        if (campos[1] != "G")
            continue;

        int id;

        try
        {
            id = stoi(campos[0]);
        }
        catch (...)
        {
            continue;
        }

        // ID 0 = grupo eliminado
        if (id == 0)
            continue;

        if (campos[2] == nombre)
            return true;
    }

    return false;
}

static int obtenerSiguienteIdGrupo(
    const string& contenido)
{
    stringstream ss(contenido);
    string linea;

    int mayor = 0;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividirMkgrp(linea, ',');

        if (campos.size() != 3)
            continue;

        if (campos[1] != "G")
            continue;

        int id;

        try
        {
            id = stoi(campos[0]);
        }
        catch (...)
        {
            continue;
        }

        if (id > mayor)
            mayor = id;
    }

    return mayor + 1;
}

static int buscarBloqueLibreMkgrp(
    fstream& archivo,
    const SuperBlock& sb)
{
    archivo.clear();
    archivo.seekg(sb.s_bm_block_start);

    if (!archivo)
        return -1;

    for (int i = 0;
         i < sb.s_blocks_count;
         i++)
    {
        char estado;

        archivo.read(&estado, sizeof(char));

        if (!archivo)
            return -1;

        if (estado == '0')
            return i;
    }

    return -1;
}

static bool marcarBloqueOcupadoMkgrp(
    fstream& archivo,
    const SuperBlock& sb,
    int indice)
{
    archivo.clear();

    archivo.seekp(
        sb.s_bm_block_start + indice
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

static bool escribirUsersMkgrp(
    fstream& archivo,
    SuperBlock& sb,
    Inode& users,
    const string& contenido)
{
    int bloquesNecesarios =
        static_cast<int>(
            (contenido.size() + 63) / 64
        );

    if (bloquesNecesarios == 0)
        bloquesNecesarios = 1;

    if (bloquesNecesarios > 12)
        return false;

    /*
        Asignar bloques que hagan falta.
    */

    for (int i = 0;
         i < bloquesNecesarios;
         i++)
    {
        if (users.i_block[i] != -1)
            continue;

        int bloqueLibre =
            buscarBloqueLibreMkgrp(
                archivo,
                sb
            );

        if (bloqueLibre == -1)
            return false;

        users.i_block[i] =
            bloqueLibre;

        if (!marcarBloqueOcupadoMkgrp(
                archivo,
                sb,
                bloqueLibre))
        {
            return false;
        }

        sb.s_free_blocks_count--;

        /*
            Guardamos la dirección del siguiente
            bloque libre conocido.
        */

        int siguiente =
            buscarBloqueLibreMkgrp(
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
    }

    /*
        Escribir el contenido distribuido
        en bloques de 64 bytes.
    */

    size_t offset = 0;

    for (int i = 0;
         i < bloquesNecesarios;
         i++)
    {
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

        if (!escribirBloqueMkgrp(
                archivo,
                sb,
                users.i_block[i],
                bloque))
        {
            return false;
        }

        offset += cantidad;
    }

    users.i_s =
        static_cast<int>(contenido.size());

    users.i_mtime =
        time(nullptr);

    return true;
}

string ejecutarMkgrp(
    const MkgrpParams& params)
{
    /*
        1. Validar parámetro
    */

    if (params.name.empty())
        return "Error: debe especificar -name.";

    if (params.name.length() > 10)
        return "Error: el nombre del grupo no puede superar 10 caracteres.";

    /*
        2. Validar sesión
    */

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
        return "Error: debe iniciar sesión.";

    /*
        3. Solo root
    */

    if (sesion.user != "root")
        return "Error: solamente el usuario root puede crear grupos.";

    /*
        4. Obtener partición de la sesión
    */

    MountedPartition mounted;

    if (!buscarParticionMontada(
            sesion.partitionId,
            mounted))
    {
        return "Error: la partición de la sesión ya no está montada.";
    }

    /*
        5. Abrir disco
    */

    fstream archivo(
        mounted.path,
        ios::binary |
        ios::in |
        ios::out
    );

    if (!archivo.is_open())
        return "Error: no se pudo abrir el disco.";

    /*
        6. Leer SuperBlock
    */

    SuperBlock sb{};

    if (!leerSuperBlockMkgrp(
            archivo,
            mounted.partition.part_start,
            sb))
    {
        archivo.close();

        return "Error: no se pudo leer el SuperBlock.";
    }

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return "Error: la partición no contiene EXT2.";
    }

    /*
        7. Leer users.txt
    */

    Inode users{};
    string contenido;

    if (!leerUsersMkgrp(
            archivo,
            sb,
            users,
            contenido))
    {
        archivo.close();

        return "Error: no se pudo leer users.txt.";
    }

    /*
        8. Verificar grupo duplicado
    */

    if (existeGrupoMkgrp(
            contenido,
            params.name))
    {
        archivo.close();

        return "Error: el grupo ya existe.";
    }

    /*
        9. Generar nuevo ID
    */

    int nuevoId =
        obtenerSiguienteIdGrupo(
            contenido
        );

    /*
        10. Agregar registro
    */

    string nuevoRegistro =
        to_string(nuevoId)
        + ",G,"
        + params.name
        + "\n";

    contenido += nuevoRegistro;

    /*
        11. Escribir users.txt
    */

    if (!escribirUsersMkgrp(
            archivo,
            sb,
            users,
            contenido))
    {
        archivo.close();

        return "Error: no se pudo actualizar users.txt.";
    }

    /*
        12. Guardar inodo 1
    */

    if (!escribirInodoMkgrp(
            archivo,
            sb,
            1,
            users))
    {
        archivo.close();

        return "Error: no se pudo actualizar el inodo de users.txt.";
    }

    /*
        13. Actualizar SuperBlock
    */

    if (!escribirSuperBlockMkgrp(
            archivo,
            mounted.partition.part_start,
            sb))
    {
        archivo.close();

        return "Error: no se pudo actualizar el SuperBlock.";
    }

    archivo.flush();
    archivo.close();

    return "Grupo creado correctamente. Grupo: "
        + params.name + ".";
}