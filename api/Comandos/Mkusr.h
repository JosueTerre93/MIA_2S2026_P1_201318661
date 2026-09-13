#ifndef MKUSR_H
#define MKUSR_H

#include <string>

struct MkusrParams
{
    std::string user;
    std::string password;
    std::string group;
};

std::string ejecutarMkusr(
    const MkusrParams& params
);

#endif