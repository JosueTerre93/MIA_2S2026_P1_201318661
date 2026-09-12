#include "Login.h"

#include "Mount.h"
#include "Session.h"

#include "../Estructuras/Ext2Structures.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct UsuarioEncontrado
{
    int id = -1;
    string grupo;
    string usuario;
    string password;
};

static vector<string> dividir(
    const string& texto,
    char separador)
{
    vector<string> partes;
    string actual;

    for (char c : texto)
    {
        if (c == separador)
        {
            partes.push_back(actual);
            actual.clear();
        }
        else
        {
            actual += c;
        }
    }

    partes.push_back(actual);

    return partes;
}

static bool leerSuperBlock(
    fstream& archivo,
    int inicioParticion,
    SuperBlock& sb)
{
    archivo.clear();

    archivo.seekg(inicioParticion);

    if (!archivo)
        return false;

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    return static_cast<bool>(archivo);
}

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

static bool leerBloqueArchivo(
    fstream& archivo,
    const SuperBlock& sb,
    int indiceBloque,
    FileBlock& bloque)
{
    int posicion =
        sb.s_block_start +
        indiceBloque * static_cast<int>(sizeof(FileBlock));

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

static bool leerUsersTxt(
    fstream& archivo,
    const SuperBlock& sb,
    string& contenido)
{
    /*
        En MKFS:

        inode 0 -> /
        inode 1 -> users.txt
    */

    Inode usersInode{};

    if (!leerInodo(
            archivo,
            sb,
            1,
            usersInode))
    {
        return false;
    }

    contenido.clear();

    /*
        De momento leemos los 12 apuntadores directos.

        i_block[0] ... i_block[11]

        Esto nos permite que users.txt crezca más adelante
        a varios bloques.
    */

    for (int i = 0; i < 12; i++)
    {
        int indiceBloque =
            usersInode.i_block[i];

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

            contenido += bloque.b_content[j];
        }
    }

    /*
        El inode conoce el tamaño real del archivo.

        Esto evita leer basura si algún bloque tuviera
        contenido después de los datos válidos.
    */

    if (usersInode.i_s >= 0 &&
        static_cast<int>(contenido.size()) >
            usersInode.i_s)
    {
        contenido.resize(usersInode.i_s);
    }

    return true;
}

static bool buscarUsuario(
    const string& contenido,
    const string& usuarioBuscado,
    UsuarioEncontrado& resultado)
{
    stringstream ss(contenido);

    string linea;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividir(linea, ',');

        /*
            Formato esperado:

            id,U,grupo,usuario,password

            Ejemplo:

            1,U,root,root,123
        */

        if (campos.size() != 5)
            continue;

        int idUsuario;

        try
        {
            idUsuario = stoi(campos[0]);
        }
        catch (...)
        {
            continue;
        }

        /*
            ID 0 = usuario eliminado
        */

        if (idUsuario == 0)
            continue;

        if (campos[1] != "U")
            continue;

        if (campos[3] != usuarioBuscado)
            continue;

        resultado.id = idUsuario;
        resultado.grupo = campos[2];
        resultado.usuario = campos[3];
        resultado.password = campos[4];

        return true;
    }

    return false;
}

static int buscarIdGrupo(
    const string& contenido,
    const string& grupoBuscado)
{
    stringstream ss(contenido);

    string linea;

    while (getline(ss, linea))
    {
        if (linea.empty())
            continue;

        vector<string> campos =
            dividir(linea, ',');

        /*
            Grupo:

            id,G,nombre

            Ejemplo:

            1,G,root
        */

        if (campos.size() != 3)
            continue;

        int idGrupo;

        try
        {
            idGrupo = stoi(campos[0]);
        }
        catch (...)
        {
            continue;
        }

        if (idGrupo == 0)
            continue;

        if (campos[1] != "G")
            continue;

        if (campos[2] == grupoBuscado)
            return idGrupo;
    }

    return -1;
}

string ejecutarLogin(
    const LoginParams& params)
{
    /*
        1. Validar parámetros
    */

    if (params.user.empty())
        return "Error: debe especificar -user.";

    if (params.password.empty())
        return "Error: debe especificar -pass.";

    if (params.id.empty())
        return "Error: debe especificar -id.";

    /*
        2. Solo puede existir una sesión activa
    */

    Session& sesion =
        obtenerSesion();

    if (sesion.active)
    {
        return
            "Error: ya existe una sesión activa con el usuario "
            + sesion.user + ".";
    }

    /*
        3. Buscar partición montada
    */

    MountedPartition mounted;

    if (!buscarParticionMontada(
            params.id,
            mounted))
    {
        return
            "Error: no existe una partición montada con el ID "
            + params.id + ".";
    }

    /*
        4. Abrir disco
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
        5. Leer SuperBlock
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
        Verificar que exista EXT2
    */

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema de archivos EXT2 válido.";
    }

    /*
        6. Leer users.txt
    */

    string contenidoUsers;

    if (!leerUsersTxt(
            archivo,
            sb,
            contenidoUsers))
    {
        archivo.close();

        return
            "Error: no se pudo leer users.txt.";
    }

    archivo.close();

    /*
        7. Buscar usuario
    */

    UsuarioEncontrado usuario;

    if (!buscarUsuario(
            contenidoUsers,
            params.user,
            usuario))
    {
        return
            "Error: el usuario no existe.";
    }

    /*
        8. Verificar contraseña
    */

    if (usuario.password !=
        params.password)
    {
        return
            "Error: contraseña incorrecta.";
    }

    /*
        9. Buscar grupo
    */

    int gid =
        buscarIdGrupo(
            contenidoUsers,
            usuario.grupo
        );

    if (gid == -1)
    {
        return
            "Error: el grupo del usuario no existe o fue eliminado.";
    }

    /*
        10. Crear sesión
    */

    sesion.active = true;

    sesion.uid = usuario.id;
    sesion.gid = gid;

    sesion.user =
        usuario.usuario;

    sesion.group =
        usuario.grupo;

    sesion.partitionId =
        params.id;

    return
        "Sesión iniciada correctamente. Usuario: "
        + sesion.user
        + ".";
}