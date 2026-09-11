#ifndef FDISK_H
#define FDISK_H

#include <string>

struct FdiskParams
{
    long long size = 0;

    // B, K o M
    std::string unit = "K";

    std::string path;

    // P, E o L
    std::string type = "P";

    // BF, FF o WF
    std::string fit = "WF";

    std::string name;
};

std::string ejecutarFdisk(
    const FdiskParams& params
);

#endif