#include "DiskUtils.h"

#include <fstream>
#include <iostream>

using namespace std;

long long convertirABytes(
    long long size,
    char unit
)
{
    switch (unit)
    {
        case 'B':
            return size;

        case 'K':
            return size * 1024LL;

        case 'M':
            return size * 1024LL * 1024LL;

        default:
            return size;
    }
}

bool leerMBR(
    const string& path,
    MBR& mbr
)
{
    ifstream archivo(
        path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return false;
    }

    archivo.seekg(0);

    archivo.read(
        reinterpret_cast<char*>(&mbr),
        sizeof(MBR)
    );

    bool correcto = archivo.good();

    archivo.close();

    return correcto;
}

bool escribirMBR(
    const string& path,
    const MBR& mbr
)
{
    fstream archivo(
        path,
        ios::in |
        ios::out |
        ios::binary
    );

    if (!archivo.is_open())
    {
        return false;
    }

    archivo.seekp(0);

    archivo.write(
        reinterpret_cast<const char*>(&mbr),
        sizeof(MBR)
    );

    bool correcto = archivo.good();

    archivo.close();

    return correcto;
}

void imprimirMBR(
    const string& path
)
{
    MBR mbr;

    if (!leerMBR(path, mbr))
    {
        cout << "No se pudo leer el disco." << endl;
        return;
    }

    cout << "========= MBR =========" << endl;

    cout << "Tamaño: "
         << mbr.mbr_tamano
         << endl;

    cout << "Signature: "
         << mbr.mbr_dsk_signature
         << endl;

    cout << "Fit: "
         << mbr.dsk_fit
         << endl;

    for (int i = 0; i < 4; i++)
    {
        Partition& p =
            mbr.mbr_partitions[i];

        cout << endl;

        cout << "Particion "
             << i + 1
             << endl;

        cout << "Status: "
             << p.part_status
             << endl;

        cout << "Type: "
             << p.part_type
             << endl;

        cout << "Start: "
             << p.part_start
             << endl;

        cout << "Size: "
             << p.part_s
             << endl;

        cout << "Name: "
             << p.part_name
             << endl;
    }
}

