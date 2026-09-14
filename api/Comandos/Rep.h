#ifndef REP_H
#define REP_H

#include <string>

using namespace std;

struct RepParams
{
    string name;
    string path;
    string id;
    string pathFileLs;
};

string ejecutarRep(
    const RepParams& params
);

#endif