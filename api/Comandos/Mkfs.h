#ifndef MKFS_H
#define MKFS_H

#include <string>

struct MkfsParams
{
    std::string id;
    std::string type = "full";
};

std::string ejecutarMkfs(
    const MkfsParams& params
);

#endif