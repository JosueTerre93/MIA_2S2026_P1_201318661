#include "Rmdisk.h"

#include <string>
#include <fstream>
#include <cstdio>

using namespace std;

string ejecutarRmdisk(
    const RmdiskParams& params)
{
    // =====================================
    // VALIDAR PATH
    // =====================================

    if (params.path.empty())
    {
        return
            "Error: debe especificar -path.";
    }


    // =====================================
    // VERIFICAR QUE EL DISCO EXISTA
    // =====================================

    ifstream archivo(
        params.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: el disco no existe: "
            + params.path;
    }

    archivo.close();


    // =====================================
    // ELIMINAR DISCO
    // =====================================

    if (remove(params.path.c_str()) != 0)
    {
        return
            "Error: no se pudo eliminar el disco: "
            + params.path;
    }


    return
        "Disco eliminado correctamente: "
        + params.path;
}