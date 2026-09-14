#ifndef CAT_H
#define CAT_H

#include <string>
#include <vector>

using namespace std;

struct CatParams
{
    vector<string> files;
};

string ejecutarCat(
    const CatParams& params
);

#endif