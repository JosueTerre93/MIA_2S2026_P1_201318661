#ifndef CHGRP_H
#define CHGRP_H

#include <string>

struct ChgrpParams
{
    std::string user;
    std::string group;
};

std::string ejecutarChgrp(
    const ChgrpParams& params
);

#endif