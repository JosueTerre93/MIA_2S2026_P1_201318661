#ifndef DISK_UTILS_H
#define DISK_UTILS_H

#include <string>
#include "../Estructuras/DiskStructures.h"

long long convertirABytes(
    long long size,
    char unit
);

bool leerMBR(
    const std::string& path,
    MBR& mbr
);

bool escribirMBR(
    const std::string& path,
    const MBR& mbr
);

void imprimirMBR(
    const std::string& path
);

#endif