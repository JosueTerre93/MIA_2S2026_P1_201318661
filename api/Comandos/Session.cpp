#include "Session.h"

static Session sesionActual;

Session& obtenerSesion()
{
    return sesionActual;
}

void cerrarSesion()
{
    sesionActual = Session();
}