#ifndef MKDIR_H
#define MKDIR_H

#include <string>

using namespace std;

struct MkdirParams
{
    string path;
    bool p = false;
};

string ejecutarMkdir(
    const MkdirParams& params
);

#endif