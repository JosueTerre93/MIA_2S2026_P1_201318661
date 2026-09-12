#ifndef MKGRP_H
#define MKGRP_H

#include <string>

struct MkgrpParams
{
    std::string name;
};

std::string ejecutarMkgrp(const MkgrpParams& params);

#endif