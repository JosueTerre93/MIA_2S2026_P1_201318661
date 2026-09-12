#ifndef LOGIN_H
#define LOGIN_H

#include <string>

struct LoginParams
{
    std::string user;
    std::string password;
    std::string id;
};

std::string ejecutarLogin(
    const LoginParams& params
);

#endif