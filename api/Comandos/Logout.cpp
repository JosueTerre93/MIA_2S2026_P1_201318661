#include "Logout.h"
#include "Session.h"

#include <string>

using namespace std;

string ejecutarLogout()
{
    Session& sesion = obtenerSesion();

    if (!sesion.active)
    {
        return "Error: no existe una sesión activa.";
    }

    string usuario = sesion.user;

    cerrarSesion();

    return "Sesión cerrada correctamente. Usuario: "
        + usuario + ".";
}