#ifndef MKDISK_H
#define MKDISK_H

#include <string>

struct MkdiskParams
{
    long long size = 0;
    std::string fit = "FF";
    std::string unit = "M";
    std::string path;
};

std::string ejecutarMkdisk(
    const MkdiskParams& params
);

#endif