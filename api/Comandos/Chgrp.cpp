#include "Chgrp.h"

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


static vector<string> dividirChgrp(
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


static bool leerSuperBlockChgrp(
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


static bool escribirSuperBlockChgrp(
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


static bool leerInodoChgrp(
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


static bool escribirInodoChgrp(
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


static bool leerBloqueChgrp(
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


static bool escribirBloqueChgrp(
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


static bool leerUsersChgrp(
    fstream& archivo,
    const SuperBlock& sb,
    Inode& users,
    string& contenido)
{
    if (!leerInodoChgrp(
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

        if (!leerBloqueChgrp(
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


static bool existeGrupoChgrp(
    const string& contenido,
    const string& nombreGrupo)
{
    stringstream ss(contenido);
    string linea;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividirChgrp(linea, ',');

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

        if (id == 0)
            continue;

        if (campos[2] == nombreGrupo)
            return true;
    }

    return false;
}


static bool cambiarGrupoUsuarioChgrp(
    string& contenido,
    const string& usuarioBuscado,
    const string& grupoNuevo)
{
    stringstream ss(contenido);

    string linea;
    string contenidoNuevo;

    bool encontrado = false;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividirChgrp(linea, ',');

        /*
            Usuario:

            id,U,grupo,usuario,password
        */
        if (campos.size() == 5 &&
            campos[1] == "U")
        {
            int id;

            try
            {
                id = stoi(campos[0]);
            }
            catch (...)
            {
                id = -1;
            }

            /*
                Solo modificar usuarios activos.
            */
            if (id != 0 &&
                campos[3] == usuarioBuscado)
            {
                campos[2] = grupoNuevo;

                linea =
                    campos[0] + "," +
                    campos[1] + "," +
                    campos[2] + "," +
                    campos[3] + "," +
                    campos[4];

                encontrado = true;
            }
        }

        contenidoNuevo += linea + "\n";
    }

    if (encontrado)
        contenido = contenidoNuevo;

    return encontrado;
}


static bool escribirUsersChgrp(
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

    /*
        CHGRP no aumenta realmente el tamaño
        de users.txt, solamente cambia el nombre
        de un grupo.

        Sin embargo, si el nuevo grupo tiene más
        caracteres, el archivo podría crecer.
    */
    if (bloquesNecesarios > 12)
        return false;

    /*
        En este comando asumimos que los bloques
        necesarios ya están asignados.

        Si quieres permitir que el cambio hacia un
        nombre de grupo más largo cruce a un nuevo
        bloque, después reutilizamos la misma lógica
        de asignación de bloques de MKUSR.
    */
    for (int i = 0; i < bloquesNecesarios; i++)
    {
        if (users.i_block[i] == -1)
            return false;
    }

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

        if (!escribirBloqueChgrp(
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


string ejecutarChgrp(
    const ChgrpParams& params)
{
    /*
        1. Validar parámetros.
    */

    if (params.user.empty())
        return "Error: debe especificar -user.";

    if (params.group.empty())
        return "Error: debe especificar -grp.";

    if (params.user.length() > 10)
    {
        return
            "Error: el nombre del usuario no puede superar 10 caracteres.";
    }

    if (params.group.length() > 10)
    {
        return
            "Error: el nombre del grupo no puede superar 10 caracteres.";
    }


    /*
        2. Validar sesión.
    */

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
    {
        return
            "Error: debe iniciar sesión.";
    }


    /*
        3. Solo root.
    */

    if (sesion.user != "root")
    {
        return
            "Error: solamente el usuario root puede cambiar grupos.";
    }


    /*
        4. Obtener partición.
    */

    MountedPartition mounted;

    if (!buscarParticionMontada(
            sesion.partitionId,
            mounted))
    {
        return
            "Error: la partición de la sesión ya no está montada.";
    }


    /*
        5. Abrir disco.
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
        6. Leer SuperBlock.
    */

    SuperBlock sb{};

    if (!leerSuperBlockChgrp(
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


    /*
        7. Leer users.txt.
    */

    Inode users{};
    string contenido;

    if (!leerUsersChgrp(
            archivo,
            sb,
            users,
            contenido))
    {
        archivo.close();

        return
            "Error: no se pudo leer users.txt.";
    }


    /*
        8. Verificar grupo destino.
    */

    if (!existeGrupoChgrp(
            contenido,
            params.group))
    {
        archivo.close();

        return
            "Error: el grupo "
            + params.group
            + " no existe.";
    }


    /*
        9. Cambiar grupo del usuario.
    */

    if (!cambiarGrupoUsuarioChgrp(
            contenido,
            params.user,
            params.group))
    {
        archivo.close();

        return
            "Error: el usuario "
            + params.user
            + " no existe.";
    }


    /*
        10. Escribir users.txt.
    */

    if (!escribirUsersChgrp(
            archivo,
            sb,
            users,
            contenido))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar users.txt.";
    }


    /*
        11. Guardar inodo.
    */

    if (!escribirInodoChgrp(
            archivo,
            sb,
            1,
            users))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar el inodo de users.txt.";
    }


    /*
        12. Guardar SuperBlock.
    */

    if (!escribirSuperBlockChgrp(
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
        "Grupo del usuario cambiado correctamente. Usuario: "
        + params.user
        + ", nuevo grupo: "
        + params.group
        + ".";
}