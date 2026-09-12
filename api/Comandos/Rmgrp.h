#ifndef RMGRP_H
#define RMGRP_H

#include <string>

struct RmgrpParams
{
    std::string name;
};

std::string ejecutarRmgrp(const RmgrpParams& params);

#endif