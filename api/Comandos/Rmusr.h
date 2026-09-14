#ifndef RMUSR_H
#define RMUSR_H

#include <string>

struct RmusrParams
{
    std::string user;
};

std::string ejecutarRmusr(
    const RmusrParams& params
);

#endif