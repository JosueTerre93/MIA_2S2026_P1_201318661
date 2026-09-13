#include "Mkusr.h"

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
    Divide una cadena utilizando un separador.
*/
static vector<string> dividirMkusr(
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
static bool leerSuperBlockMkusr(
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
static bool escribirSuperBlockMkusr(
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
static bool leerInodoMkusr(
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
static bool escribirInodoMkusr(
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
    Leer un bloque de archivo.
*/
static bool leerBloqueMkusr(
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
    Escribir un bloque de archivo.
*/
static bool escribirBloqueMkusr(
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

    inode 1 = users.txt
*/
static bool leerUsersMkusr(
    fstream& archivo,
    const SuperBlock& sb,
    Inode& users,
    string& contenido)
{
    if (!leerInodoMkusr(
            archivo,
            sb,
            1,
            users))
    {
        return false;
    }

    contenido.clear();

    int restantes =
        users.i_s;

    /*
        Leer apuntadores directos:

        i_block[0] ... i_block[11]
    */
    for (int i = 0;
         i < 12 && restantes > 0;
         i++)
    {
        if (users.i_block[i] == -1)
            continue;

        FileBlock bloque{};

        if (!leerBloqueMkusr(
                archivo,
                sb,
                users.i_block[i],
                bloque))
        {
            return false;
        }

        int cantidad =
            restantes;

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
    Verificar si existe un grupo activo.

    Formato:

    id,G,nombre
*/
static bool existeGrupoMkusr(
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
            dividirMkusr(
                linea,
                ','
            );

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

        /*
            ID 0 = grupo eliminado.
        */
        if (id == 0)
            continue;

        if (campos[2] ==
            nombreGrupo)
        {
            return true;
        }
    }

    return false;
}


/*
    Verificar si existe un usuario activo.

    Formato:

    id,U,grupo,usuario,password
*/
static bool existeUsuarioMkusr(
    const string& contenido,
    const string& nombreUsuario)
{
    stringstream ss(contenido);

    string linea;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividirMkusr(
                linea,
                ','
            );

        if (campos.size() != 5)
            continue;

        if (campos[1] != "U")
            continue;

        int id;

        try
        {
            id =
                stoi(campos[0]);
        }
        catch (...)
        {
            continue;
        }

        /*
            ID 0 = usuario eliminado.
        */
        if (id == 0)
            continue;

        /*
            campo 3 = nombre del usuario

            id,U,grupo,usuario,password
        */
        if (campos[3] ==
            nombreUsuario)
        {
            return true;
        }
    }

    return false;
}


/*
    Obtener el siguiente ID disponible
    para usuarios.

    Busca únicamente registros tipo U.
*/
static int obtenerSiguienteIdUsuarioMkusr(
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
            dividirMkusr(
                linea,
                ','
            );

        if (campos.size() != 5)
            continue;

        if (campos[1] != "U")
            continue;

        int id;

        try
        {
            id =
                stoi(campos[0]);
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


/*
    Buscar bloque libre utilizando
    el bitmap de bloques.
*/
static int buscarBloqueLibreMkusr(
    fstream& archivo,
    const SuperBlock& sb)
{
    archivo.clear();

    archivo.seekg(
        sb.s_bm_block_start
    );

    if (!archivo)
        return -1;

    for (int i = 0;
         i < sb.s_blocks_count;
         i++)
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


/*
    Marcar bloque como ocupado
    dentro del bitmap.
*/
static bool marcarBloqueOcupadoMkusr(
    fstream& archivo,
    const SuperBlock& sb,
    int indice)
{
    archivo.clear();

    archivo.seekp(
        sb.s_bm_block_start +
        indice
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


/*
    Escribir nuevamente users.txt.

    Si users.txt necesita más bloques,
    se buscan bloques libres en el bitmap.
*/
static bool escribirUsersMkusr(
    fstream& archivo,
    SuperBlock& sb,
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

    /*
        De momento soportamos únicamente
        los 12 apuntadores directos.
    */
    if (bloquesNecesarios > 12)
        return false;


    /*
        Asignar bloques nuevos
        cuando sean necesarios.
    */
    for (int i = 0;
         i < bloquesNecesarios;
         i++)
    {
        /*
            Si ya existe un bloque
            asignado a esta posición,
            se reutiliza.
        */
        if (users.i_block[i] != -1)
            continue;


        int bloqueLibre =
            buscarBloqueLibreMkusr(
                archivo,
                sb
            );

        if (bloqueLibre == -1)
            return false;


        users.i_block[i] =
            bloqueLibre;


        if (!marcarBloqueOcupadoMkusr(
                archivo,
                sb,
                bloqueLibre))
        {
            return false;
        }


        /*
            Un bloque menos disponible.
        */
        sb.s_free_blocks_count--;


        /*
            Actualizar la referencia
            al siguiente bloque libre.
        */
        int siguiente =
            buscarBloqueLibreMkusr(
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
        Distribuir el contenido en
        bloques de 64 bytes.
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
            contenido.size()
            - offset;


        size_t cantidad =
            restantes > 64
                ? 64
                : restantes;


        if (cantidad > 0)
        {
            memcpy(
                bloque.b_content,
                contenido.data()
                    + offset,
                cantidad
            );
        }


        if (!escribirBloqueMkusr(
                archivo,
                sb,
                users.i_block[i],
                bloque))
        {
            return false;
        }


        offset += cantidad;
    }


    /*
        Actualizar tamaño y fecha
        de modificación de users.txt.
    */
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
                    EJECUTAR MKUSR
    ==================================================
*/
string ejecutarMkusr(
    const MkusrParams& params)
{
    /*
        1. Validar parámetros.
    */

    if (params.user.empty())
        return "Error: debe especificar -user.";

    if (params.password.empty())
        return "Error: debe especificar -pass.";

    if (params.group.empty())
        return "Error: debe especificar -grp.";


    /*
        Máximo 10 caracteres según
        las especificaciones.
    */

    if (params.user.length() > 10)
    {
        return
            "Error: el nombre del usuario no puede superar 10 caracteres.";
    }

    if (params.password.length() > 10)
    {
        return
            "Error: la contraseña no puede superar 10 caracteres.";
    }

    if (params.group.length() > 10)
    {
        return
            "Error: el nombre del grupo no puede superar 10 caracteres.";
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
        3. Solo root puede
        crear usuarios.
    */

    if (sesion.user != "root")
    {
        return
            "Error: solamente el usuario root puede crear usuarios.";
    }


    /*
        4. Obtener la partición
        correspondiente a la sesión.
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

    if (!leerSuperBlockMkusr(
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
        7. Leer users.txt.
    */

    Inode users{};

    string contenido;

    if (!leerUsersMkusr(
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
        8. Verificar que exista
        el grupo solicitado.
    */

    if (!existeGrupoMkusr(
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
        9. Verificar que el usuario
        no exista.
    */

    if (existeUsuarioMkusr(
            contenido,
            params.user))
    {
        archivo.close();

        return
            "Error: el usuario "
            + params.user
            + " ya existe.";
    }


    /*
        10. Obtener nuevo ID.
    */

    int nuevoId =
        obtenerSiguienteIdUsuarioMkusr(
            contenido
        );


    /*
        11. Crear nuevo registro.

        Formato:

        id,U,grupo,usuario,password
    */

    string nuevoRegistro =
        to_string(nuevoId)
        + ",U,"
        + params.group
        + ","
        + params.user
        + ","
        + params.password
        + "\n";


    /*
        Agregar el registro al
        contenido existente.
    */

    contenido += nuevoRegistro;


    /*
        12. Escribir users.txt.
    */

    if (!escribirUsersMkusr(
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
        13. Guardar cambios
        en el inode 1.
    */

    if (!escribirInodoMkusr(
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
        14. Actualizar SuperBlock.
    */

    if (!escribirSuperBlockMkusr(
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
        "Usuario creado correctamente. Usuario: "
        + params.user
        + ", grupo: "
        + params.group
        + ".";
}