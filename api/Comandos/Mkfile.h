#ifndef MKFILE_H
#define MKFILE_H

#include <string>

using namespace std;

struct MkfileParams
{
    string path;

    bool r = false;

    long long size = 0;
    bool sizeIngresado = false;

    string cont;
};

string ejecutarMkfile(
    const MkfileParams& params
);

#endif