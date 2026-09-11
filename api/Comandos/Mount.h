#ifndef MOUNT_H
#define MOUNT_H

#include <string>
#include "../Estructuras/DiskStructures.h"

struct MountParams
{
    std::string path;
    std::string name;
};

struct MountedPartition
{
    std::string id;
    std::string path;
    std::string name;

    char diskLetter = 'A';
    int correlative = 0;

    Partition partition;
};

std::string ejecutarMount(
    const MountParams& params
);

std::string mostrarParticionesMontadas();

bool buscarParticionMontada(
    const std::string& id,
    MountedPartition& resultado
);

#endif