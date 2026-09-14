#ifndef RMDISK_H
#define RMDISK_H

#include <string>

using namespace std;

struct RmdiskParams
{
    string path;
};

string ejecutarRmdisk(
    const RmdiskParams& params
);

#endif