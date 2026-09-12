#ifndef SESSION_H
#define SESSION_H

#include <string>

struct Session
{
    bool active = false;

    int uid = -1;
    int gid = -1;

    std::string user;
    std::string group;
    std::string partitionId;
};

Session& obtenerSesion();

void cerrarSesion();

#endif