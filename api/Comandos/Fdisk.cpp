#include "Fdisk.h"

#include "../Estructuras/DiskStructures.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <vector>
#include <climits>

using namespace std;


// ======================================================
// ESTRUCTURA AUXILIAR PARA ESPACIOS LIBRES
// ======================================================

struct EspacioLibre
{
    int inicio;
    int tamanio;
};


// ======================================================
// LIMPIAR RUTA
// ======================================================

static string limpiarRuta(string path)
{
    path.erase(
        remove(
            path.begin(),
            path.end(),
            '"'
        ),
        path.end()
    );

    return path;
}


// ======================================================
// CONVERTIR TAMAÑO A BYTES
// ======================================================

static long long convertirABytes(
    long long size,
    const string& unit
)
{
    if (unit == "B")
    {
        return size;
    }

    if (unit == "K")
    {
        return size * 1024LL;
    }

    if (unit == "M")
    {
        return size * 1024LL * 1024LL;
    }

    return size;
}


// ======================================================
// CONVERTIR FIT
// ======================================================

static char convertirFit(
    const string& fit
)
{
    if (fit == "BF")
    {
        return 'B';
    }

    if (fit == "WF")
    {
        return 'W';
    }

    return 'F';
}


// ======================================================
// VERIFICAR SI EXISTE UNA PARTICIÓN EXTENDIDA
// ======================================================

static bool existeExtendida(
    const MBR& mbr
)
{
    for (int i = 0; i < 4; i++)
    {
        const Partition& p =
            mbr.mbr_partitions[i];

        if (
            p.part_status != '0'
            &&
            p.part_type == 'E'
        )
        {
            return true;
        }
    }

    return false;
}


// ======================================================
// VERIFICAR SI EL NOMBRE YA EXISTE
// ======================================================

static bool existeNombre(
    const MBR& mbr,
    const string& name
)
{
    for (int i = 0; i < 4; i++)
    {
        const Partition& p =
            mbr.mbr_partitions[i];

        if (p.part_status == '0')
        {
            continue;
        }

        if (name == p.part_name)
        {
            return true;
        }
    }

    return false;
}


// ======================================================
// OBTENER POSICIÓN LIBRE EN TABLA MBR
// ======================================================

static int obtenerIndiceLibre(
    const MBR& mbr
)
{
    for (int i = 0; i < 4; i++)
    {
        if (
            mbr.mbr_partitions[i].part_status
            == '0'
        )
        {
            return i;
        }
    }

    return -1;
}


// ======================================================
// OBTENER PARTICIONES ACTIVAS ORDENADAS
// ======================================================

static vector<Partition> obtenerParticionesActivas(
    const MBR& mbr
)
{
    vector<Partition> particiones;

    for (int i = 0; i < 4; i++)
    {
        const Partition& p =
            mbr.mbr_partitions[i];

        if (
            p.part_status != '0'
            &&
            p.part_start >= 0
            &&
            p.part_s > 0
        )
        {
            particiones.push_back(p);
        }
    }

    sort(
        particiones.begin(),
        particiones.end(),
        [](
            const Partition& a,
            const Partition& b
        )
        {
            return
                a.part_start
                <
                b.part_start;
        }
    );

    return particiones;
}


// ======================================================
// CALCULAR ESPACIOS LIBRES
// ======================================================

static vector<EspacioLibre> obtenerEspaciosLibres(
    const MBR& mbr
)
{
    vector<EspacioLibre> espacios;

    vector<Partition> particiones =
        obtenerParticionesActivas(mbr);

    int inicioDisponible =
        static_cast<int>(
            sizeof(MBR)
        );

    // Si no existen particiones
    if (particiones.empty())
    {
        int disponible =
            mbr.mbr_tamano
            - inicioDisponible;

        if (disponible > 0)
        {
            espacios.push_back(
                {
                    inicioDisponible,
                    disponible
                }
            );
        }

        return espacios;
    }


    // Espacio antes de la primera partición
    if (
        particiones[0].part_start
        >
        inicioDisponible
    )
    {
        espacios.push_back(
            {
                inicioDisponible,
                particiones[0].part_start
                - inicioDisponible
            }
        );
    }


    // Espacios entre particiones
    for (
        size_t i = 0;
        i + 1 < particiones.size();
        i++
    )
    {
        int finActual =
            particiones[i].part_start
            +
            particiones[i].part_s;

        int inicioSiguiente =
            particiones[i + 1].part_start;

        if (inicioSiguiente > finActual)
        {
            espacios.push_back(
                {
                    finActual,
                    inicioSiguiente - finActual
                }
            );
        }
    }


    // Espacio después de la última partición
    Partition ultima =
        particiones.back();

    int finUltima =
        ultima.part_start
        +
        ultima.part_s;

    if (finUltima < mbr.mbr_tamano)
    {
        espacios.push_back(
            {
                finUltima,
                mbr.mbr_tamano
                - finUltima
            }
        );
    }

    return espacios;
}


// ======================================================
// SELECCIONAR ESPACIO SEGÚN FIT DEL DISCO
// ======================================================

static int seleccionarEspacio(
    const vector<EspacioLibre>& espacios,
    int tamanioNecesario,
    char diskFit
)
{
    vector<EspacioLibre> candidatos;

    for (const EspacioLibre& espacio : espacios)
    {
        if (
            espacio.tamanio
            >=
            tamanioNecesario
        )
        {
            candidatos.push_back(
                espacio
            );
        }
    }

    if (candidatos.empty())
    {
        return -1;
    }


    // =====================================
    // FIRST FIT
    // =====================================

    if (diskFit == 'F')
    {
        return candidatos[0].inicio;
    }


    // =====================================
    // BEST FIT
    // =====================================

    if (diskFit == 'B')
    {
        auto mejor =
            min_element(
                candidatos.begin(),
                candidatos.end(),
                [](
                    const EspacioLibre& a,
                    const EspacioLibre& b
                )
                {
                    return
                        a.tamanio
                        <
                        b.tamanio;
                }
            );

        return mejor->inicio;
    }


    // =====================================
    // WORST FIT
    // =====================================

    if (diskFit == 'W')
    {
        auto peor =
            max_element(
                candidatos.begin(),
                candidatos.end(),
                [](
                    const EspacioLibre& a,
                    const EspacioLibre& b
                )
                {
                    return
                        a.tamanio
                        <
                        b.tamanio;
                }
            );

        return peor->inicio;
    }


    // Por seguridad: FF
    return candidatos[0].inicio;
}


// ======================================================
// CREAR EBR VACÍO PARA PARTICIÓN EXTENDIDA
// ======================================================

static void crearEBRInicial(
    fstream& archivo,
    const Partition& particion
)
{
    EBR ebr{};

    ebr.part_mount = '0';

    ebr.part_fit =
        particion.part_fit;

    ebr.part_start =
        particion.part_start;

    ebr.part_s = 0;

    ebr.part_next = -1;

    ebr.part_name[0] = '\0';

    archivo.seekp(
        particion.part_start
    );

    archivo.write(
        reinterpret_cast<char*>(&ebr),
        sizeof(EBR)
    );
}


// ======================================================
// FDISK
// ======================================================

string ejecutarFdisk(
    const FdiskParams& params
)
{
    // =====================================
    // VALIDACIONES
    // =====================================

    if (params.size <= 0)
    {
        return
            "Error: el tamaño de la partición "
            "debe ser mayor que 0.";
    }


    if (params.path.empty())
    {
        return
            "Error: debe especificar -path.";
    }


    if (params.name.empty())
    {
        return
            "Error: debe especificar -name.";
    }


    if (
        params.name.length()
        > 15
    )
    {
        return
            "Error: el nombre de la partición "
            "no puede superar 15 caracteres.";
    }


    string path =
        limpiarRuta(
            params.path
        );


    if (
        !filesystem::exists(path)
    )
    {
        return
            "Error: el disco no existe.";
    }


    // =====================================
    // ABRIR DISCO
    // =====================================

    fstream archivo(
        path,
        ios::binary
        |
        ios::in
        |
        ios::out
    );


    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    // =====================================
    // LEER MBR
    // =====================================

    MBR mbr{};

    archivo.seekg(0);

    archivo.read(
        reinterpret_cast<char*>(&mbr),
        sizeof(MBR)
    );


    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el MBR.";
    }


    // =====================================
    // TIPO L SE IMPLEMENTARÁ CON EBR
    // =====================================

    if (params.type == "L")
    {
        archivo.close();

        return
            "Error: las particiones lógicas "
            "se implementarán mediante EBR.";
    }


    // =====================================
    // VALIDAR TIPO
    // =====================================

    if (
        params.type != "P"
        &&
        params.type != "E"
    )
    {
        archivo.close();

        return
            "Error: tipo de partición inválido.";
    }


    // =====================================
    // SOLO UNA EXTENDIDA
    // =====================================

    if (
        params.type == "E"
        &&
        existeExtendida(mbr)
    )
    {
        archivo.close();

        return
            "Error: ya existe una partición "
            "extendida en el disco.";
    }


    // =====================================
    // NOMBRE DUPLICADO
    // =====================================

    if (
        existeNombre(
            mbr,
            params.name
        )
    )
    {
        archivo.close();

        return
            "Error: ya existe una partición "
            "con ese nombre.";
    }


    // =====================================
    // BUSCAR ESPACIO EN TABLA MBR
    // =====================================

    int indice =
        obtenerIndiceLibre(mbr);


    if (indice == -1)
    {
        archivo.close();

        return
            "Error: el disco ya contiene "
            "4 particiones primarias/extendidas.";
    }


    // =====================================
    // TAMAÑO
    // =====================================

    long long tamanioLong =
        convertirABytes(
            params.size,
            params.unit
        );


    if (
        tamanioLong <= 0
        ||
        tamanioLong > INT_MAX
    )
    {
        archivo.close();

        return
            "Error: tamaño de partición inválido.";
    }


    int tamanio =
        static_cast<int>(
            tamanioLong
        );


    // Una extendida debe tener al menos espacio
    // suficiente para almacenar un EBR.
    if (
        params.type == "E"
        &&
        tamanio
        <= static_cast<int>(sizeof(EBR))
    )
    {
        archivo.close();

        return
            "Error: la partición extendida "
            "es demasiado pequeña.";
    }


    // =====================================
    // CALCULAR ESPACIOS DISPONIBLES
    // =====================================

    vector<EspacioLibre> espacios =
        obtenerEspaciosLibres(mbr);


    int inicio =
        seleccionarEspacio(
            espacios,
            tamanio,
            mbr.dsk_fit
        );


    if (inicio == -1)
    {
        archivo.close();

        return
            "Error: no existe suficiente espacio "
            "para crear la partición.";
    }


    // =====================================
    // CREAR PARTICIÓN
    // =====================================

    Partition nueva{};

    nueva.part_status = '1';

    nueva.part_type =
        params.type[0];

    nueva.part_fit =
        convertirFit(
            params.fit
        );

    nueva.part_start =
        inicio;

    nueva.part_s =
        tamanio;

    nueva.part_correlative =
        -1;

    nueva.part_id[0] =
        '\0';


    // Copiar nombre de forma segura
    memset(
        nueva.part_name,
        0,
        sizeof(nueva.part_name)
    );

    strncpy(
        nueva.part_name,
        params.name.c_str(),
        sizeof(nueva.part_name) - 1
    );


    // Guardar dentro del MBR
    mbr.mbr_partitions[indice] =
        nueva;


    // =====================================
    // ESCRIBIR MBR
    // =====================================

    archivo.seekp(0);

    archivo.write(
        reinterpret_cast<char*>(&mbr),
        sizeof(MBR)
    );


    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo actualizar el MBR.";
    }


    // =====================================
    // SI ES EXTENDIDA, CREAR PRIMER EBR
    // =====================================

    if (params.type == "E")
    {
        crearEBRInicial(
            archivo,
            nueva
        );
    }


    archivo.flush();
    archivo.close();


    // =====================================
    // RESPUESTA
    // =====================================

    string tipoTexto;

    if (params.type == "P")
    {
        tipoTexto = "primaria";
    }
    else
    {
        tipoTexto = "extendida";
    }


    return
        "Partición "
        + tipoTexto
        + " creada correctamente.";
}