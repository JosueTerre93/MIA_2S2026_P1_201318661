#include "Rmusr.h"

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


/*
    Divide una cadena utilizando
    un separador.
*/
static vector<string> dividirRmusr(
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


/*
    Leer SuperBlock.
*/
static bool leerSuperBlockRmusr(
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


/*
    Escribir SuperBlock.
*/
static bool escribirSuperBlockRmusr(
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


/*
    Leer inodo.
*/
static bool leerInodoRmusr(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    Inode& inode)
{
    int posicion =
        sb.s_inode_start +
        indice *
        static_cast<int>(sizeof(Inode));

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
    Escribir inodo.
*/
static bool escribirInodoRmusr(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const Inode& inode)
{
    int posicion =
        sb.s_inode_start +
        indice *
        static_cast<int>(sizeof(Inode));

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


/*
    Leer bloque.
*/
static bool leerBloqueRmusr(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    FileBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indice *
        static_cast<int>(sizeof(FileBlock));

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
    Escribir bloque.
*/
static bool escribirBloqueRmusr(
    fstream& archivo,
    const SuperBlock& sb,
    int indice,
    const FileBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indice *
        static_cast<int>(sizeof(FileBlock));

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


/*
    Leer users.txt.

    Inodo 1 = users.txt.
*/
static bool leerUsersRmusr(
    fstream& archivo,
    const SuperBlock& sb,
    Inode& users,
    string& contenido)
{
    if (!leerInodoRmusr(
            archivo,
            sb,
            1,
            users))
    {
        return false;
    }

    contenido.clear();

    int restantes = users.i_s;

    /*
        Leer apuntadores directos.
    */
    for (int i = 0;
         i < 12 && restantes > 0;
         i++)
    {
        if (users.i_block[i] == -1)
            continue;

        FileBlock bloque{};

        if (!leerBloqueRmusr(
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


/*
    Buscar y eliminar lógicamente
    un usuario.

    Formato:

    id,U,grupo,usuario,password

    Para eliminarlo:

    0,U,grupo,usuario,password
*/
static bool eliminarUsuarioRmusr(
    const string& contenido,
    const string& usuarioBuscado,
    string& nuevoContenido)
{
    stringstream ss(contenido);

    string linea;

    nuevoContenido.clear();

    bool encontrado = false;

    while (getline(ss, linea))
    {
        if (linea.empty())
        {
            continue;
        }

        vector<string> campos =
            dividirRmusr(
                linea,
                ','
            );

        /*
            Registro de usuario:
            id,U,grupo,usuario,password
        */
        if (campos.size() == 5 &&
            campos[1] == "U")
        {
            int id = -1;

            try
            {
                id = stoi(campos[0]);
            }
            catch (...)
            {
                id = -1;
            }

            /*
                Solo eliminar usuarios activos.
            */
            if (id != 0 &&
                campos[3] == usuarioBuscado)
            {
                campos[0] = "0";

                linea =
                    campos[0] + "," +
                    campos[1] + "," +
                    campos[2] + "," +
                    campos[3] + "," +
                    campos[4];

                encontrado = true;
            }
        }

        nuevoContenido += linea;
        nuevoContenido += "\n";
    }

    return encontrado;
}


/*
    Escribir nuevamente users.txt.

    RMUSR no cambia el tamaño del archivo,
    solamente cambia un ID por 0.
*/
static bool escribirUsersRmusr(
    fstream& archivo,
    const SuperBlock& sb,
    Inode& users,
    const string& contenido)
{
    int bloquesNecesarios =
        static_cast<int>(
            (contenido.size() + 63)
            / 64
        );

    if (bloquesNecesarios == 0)
        bloquesNecesarios = 1;

    if (bloquesNecesarios > 12)
        return false;

    size_t offset = 0;

    for (int i = 0;
         i < bloquesNecesarios;
         i++)
    {
        /*
            Como RMUSR no aumenta users.txt,
            los bloques ya deberían existir.
        */
        if (users.i_block[i] == -1)
        {
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

        if (!escribirBloqueRmusr(
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
        static_cast<int>(
            contenido.size()
        );

    users.i_mtime =
        time(nullptr);

    return true;
}


/*
    ==================================================
                    EJECUTAR RMUSR
    ==================================================
*/
string ejecutarRmusr(
    const RmusrParams& params)
{
    /*
        1. Validar parámetro.
    */

    if (params.user.empty())
    {
        return
            "Error: debe especificar -user.";
    }

    if (params.user.length() > 10)
    {
        return
            "Error: el nombre del usuario no puede superar 10 caracteres.";
    }


    /*
        2. Verificar sesión.
    */

    Session& sesion =
        obtenerSesion();

    if (!sesion.active)
    {
        return
            "Error: debe iniciar sesión.";
    }


    /*
        3. Solo root puede eliminar usuarios.
    */

    if (sesion.user != "root")
    {
        return
            "Error: solamente el usuario root puede eliminar usuarios.";
    }


    /*
        4. Evitar eliminar root.
    */

    if (params.user == "root")
    {
        return
            "Error: no se puede eliminar el usuario root.";
    }


    /*
        5. Obtener partición montada.
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
        6. Abrir disco.
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
        7. Leer SuperBlock.
    */

    SuperBlock sb{};

    if (!leerSuperBlockRmusr(
            archivo,
            mounted.partition.part_start,
            sb))
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    /*
        Verificar EXT2.
    */

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene EXT2.";
    }


    /*
        8. Leer users.txt.
    */

    Inode users{};

    string contenido;

    if (!leerUsersRmusr(
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
        9. Buscar usuario y cambiar
        su ID a 0.
    */

    string nuevoContenido;

    bool eliminado =
        eliminarUsuarioRmusr(
            contenido,
            params.user,
            nuevoContenido
        );

    if (!eliminado)
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

    if (!escribirUsersRmusr(
            archivo,
            sb,
            users,
            nuevoContenido))
    {
        archivo.close();

        return
            "Error: no se pudo actualizar users.txt.";
    }


    /*
        11. Actualizar inodo 1.
    */

    if (!escribirInodoRmusr(
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

    if (!escribirSuperBlockRmusr(
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
        "Usuario eliminado correctamente. Usuario: "
        + params.user
        + ".";
}