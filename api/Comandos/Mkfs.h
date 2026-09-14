#ifndef MKFS_H
#define MKFS_H

#include <string>

using namespace std;

struct MkfsParams
{
    string id;
    string type = "full";
};

string ejecutarMkfs(
    const MkfsParams& params
);

#endif