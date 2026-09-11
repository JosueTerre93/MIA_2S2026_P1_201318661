#include "Mkdisk.h"
#include "../Estructuras/DiskStructures.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

static long long convertirABytes(
    long long size,
    const string& unit
)
{
    if (unit == "K")
        return size * 1024LL;

    if (unit == "M")
        return size * 1024LL * 1024LL;

    return size;
}

static char convertirFit(
    const string& fit
)
{
    if (fit == "BF")
        return 'B';

    if (fit == "WF")
        return 'W';

    return 'F';
}

#include <algorithm>

std::string limpiarRuta(std::string path)
{
    path.erase(
        std::remove(
            path.begin(),
            path.end(),
            '"'
        ),
        path.end()
    );

    return path;
}

string ejecutarMkdisk(
    const MkdiskParams& params
)
{
    std::cout << "ENTRO A ejecutarMkdisk" << std::endl;

    std::cout << "size = " << params.size << std::endl;
    std::cout << "fit = " << params.fit << std::endl;
    std::cout << "unit = " << params.unit << std::endl;
    std::cout << "path = [" << params.path << "]" << std::endl;


    std::string path = limpiarRuta(params.path);

    std::cout << "===== MKDISK =====" << std::endl;

    std::cout << "Size: "
            << params.size
            << std::endl;

    std::cout << "Fit: "
            << params.fit
            << std::endl;

    std::cout << "Unit: "
            << params.unit
            << std::endl;

    std::cout << "Path recibido: "
            << params.path
            << std::endl;

    std::cout << "Path limpio: "
            << path
            << std::endl;

    if (params.size <= 0)
    {
        return "Error: el tamaño debe ser mayor que 0.";
    }

    if (path.empty())
    {
        return "Error: debe especificar -path.";
    }

    long long tamanioBytes =
        convertirABytes(
            params.size,
            params.unit
        );

    filesystem::path ruta(path);

    if (ruta.has_parent_path())
    {
        filesystem::create_directories(
            ruta.parent_path()
        );
    }

    ofstream archivo(
        path,
        ios::binary | ios::trunc
    );

    if (!archivo.is_open())
    {
        return "Error: no se pudo crear el disco.";
    }

    char buffer[1024] = {0};

    long long restante =
        tamanioBytes;

    while (restante > 0)
    {
        long long cantidad =
            min<long long>(
                sizeof(buffer),
                restante
            );

        archivo.write(
            buffer,
            cantidad
        );

        restante -= cantidad;
    }

    MBR mbr{};

    mbr.mbr_tamano =
        static_cast<int>(tamanioBytes);

    mbr.mbr_fecha_creacion =
        time(nullptr);

    mbr.mbr_dsk_signature =
        rand();

    mbr.dsk_fit =
        convertirFit(params.fit);

    for (int i = 0; i < 4; i++)
    {
        Partition& p =
            mbr.mbr_partitions[i];

        p.part_status = '0';
        p.part_type = '0';
        p.part_fit = '0';

        p.part_start = -1;
        p.part_s = 0;

        p.part_name[0] = '\0';

        p.part_correlative = -1;

        p.part_id[0] = '\0';
    }

    archivo.seekp(0);

    archivo.write(
        reinterpret_cast<char*>(&mbr),
        sizeof(MBR)
    );

    archivo.close();

    return "Disco creado correctamente.";
}