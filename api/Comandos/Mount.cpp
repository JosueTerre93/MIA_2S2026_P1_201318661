#include "Mount.h"

#include "../Estructuras/DiskStructures.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <vector>

using namespace std;


// ======================================================
// ULTIMOS DOS DIGITOS DEL CARNET
// ======================================================
//
// CAMBIA "34" POR LOS ULTIMOS DOS DIGITOS DE TU CARNET
//
static const string CARNET = "61";


// ======================================================
// PARTICIONES MONTADAS EN RAM
// ======================================================

static vector<MountedPartition> particionesMontadas;


// ======================================================
// LIMPIAR RUTA
// ======================================================

static string limpiarRutaMount(
    string path
)
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
// VERIFICAR SI UNA PARTICION YA ESTA MONTADA
// ======================================================

static bool yaEstaMontada(
    const string& path,
    const string& name
)
{
    for (
        const MountedPartition& mounted :
        particionesMontadas
    )
    {
        if (
            mounted.path == path
            &&
            mounted.name == name
        )
        {
            return true;
        }
    }

    return false;
}


// ======================================================
// VERIFICAR SI UN DISCO YA ESTA REGISTRADO
// ======================================================

static bool obtenerLetraDisco(
    const string& path,
    char& letra
)
{
    for (
        const MountedPartition& mounted :
        particionesMontadas
    )
    {
        if (
            mounted.path == path
        )
        {
            letra = mounted.diskLetter;

            return true;
        }
    }

    return false;
}


// ======================================================
// GENERAR SIGUIENTE LETRA DE DISCO
// ======================================================

static char obtenerSiguienteLetraDisco()
{
    char mayor = '@';

    for (
        const MountedPartition& mounted :
        particionesMontadas
    )
    {
        if (
            mounted.diskLetter > mayor
        )
        {
            mayor =
                mounted.diskLetter;
        }
    }


    if (mayor == '@')
    {
        return 'A';
    }


    if (mayor >= 'Z')
    {
        return '\0';
    }


    return
        static_cast<char>(
            mayor + 1
        );
}


// ======================================================
// OBTENER SIGUIENTE CORRELATIVO DEL DISCO
// ======================================================

static int obtenerSiguienteCorrelativo(
    const string& path
)
{
    int mayor =
        0;


    for (
        const MountedPartition& mounted :
        particionesMontadas
    )
    {
        if (
            mounted.path == path
            &&
            mounted.correlative > mayor
        )
        {
            mayor =
                mounted.correlative;
        }
    }


    return
        mayor + 1;
}


// ======================================================
// GENERAR ID
// ======================================================

static string generarId(
    int correlativo,
    char letra
)
{
    return
        CARNET
        +
        to_string(correlativo)
        +
        string(1, letra);
}


// ======================================================
// BUSCAR PARTICION PRIMARIA EN EL MBR
// ======================================================

static int buscarParticionPrimaria(
    const MBR& mbr,
    const string& name
)
{
    for (int i = 0; i < 4; i++)
    {
        const Partition& p =
            mbr.mbr_partitions[i];


        if (
            p.part_status == '0'
        )
        {
            continue;
        }


        if (
            p.part_type != 'P'
        )
        {
            continue;
        }


        if (
            name == p.part_name
        )
        {
            return i;
        }
    }


    return -1;
}


// ======================================================
// VERIFICAR SI EL NOMBRE EXISTE PERO NO ES PRIMARIA
// ======================================================

static bool existeParticionNoPrimaria(
    const MBR& mbr,
    const string& name
)
{
    for (int i = 0; i < 4; i++)
    {
        const Partition& p =
            mbr.mbr_partitions[i];


        if (
            p.part_status == '0'
        )
        {
            continue;
        }


        if (
            name == p.part_name
            &&
            p.part_type != 'P'
        )
        {
            return true;
        }
    }


    return false;
}


// ======================================================
// MOUNT
// ======================================================

string ejecutarMount(
    const MountParams& params
)
{
    // =====================================
    // VALIDACIONES
    // =====================================

    if (
        params.path.empty()
    )
    {
        return
            "Error: debe especificar -path.";
    }


    if (
        params.name.empty()
    )
    {
        return
            "Error: debe especificar -name.";
    }


    string path =
        limpiarRutaMount(
            params.path
        );


    if (
        !filesystem::exists(path)
    )
    {
        return
            "Error: el disco no existe.";
    }


    if (
        yaEstaMontada(
            path,
            params.name
        )
    )
    {
        return
            "Error: la partición ya está montada.";
    }


    // =====================================
    // ABRIR DISCO SOLO PARA LECTURA
    // =====================================

    ifstream archivo(
        path,
        ios::binary
    );


    if (
        !archivo.is_open()
    )
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


    if (
        !archivo
    )
    {
        archivo.close();

        return
            "Error: no se pudo leer el MBR.";
    }


    // =====================================
    // BUSCAR PARTICION PRIMARIA
    // =====================================

    int indice =
        buscarParticionPrimaria(
            mbr,
            params.name
        );


    if (
        indice == -1
    )
    {
        // Puede existir, pero ser extendida.
        if (
            existeParticionNoPrimaria(
                mbr,
                params.name
            )
        )
        {
            archivo.close();

            return
                "Error: solo se pueden montar "
                "particiones primarias.";
        }


        archivo.close();

        return
            "Error: no existe una partición "
            "primaria con el nombre indicado.";
    }


    // =====================================
    // DETERMINAR LETRA DEL DISCO
    // =====================================

    char letra;


    bool discoYaMontado =
        obtenerLetraDisco(
            path,
            letra
        );


    if (
        !discoYaMontado
    )
    {
        letra =
            obtenerSiguienteLetraDisco();


        if (
            letra == '\0'
        )
        {
            archivo.close();

            return
                "Error: se alcanzó el límite "
                "de letras disponibles para discos.";
        }
    }


    // =====================================
    // CORRELATIVO
    // =====================================

    int correlativo =
        obtenerSiguienteCorrelativo(
            path
        );


    // =====================================
    // GENERAR ID
    // =====================================

    string id =
        generarId(
            correlativo,
            letra
        );


    // =====================================
    // COPIAR PARTICION A RAM
    // =====================================

    Partition particion =
        mbr.mbr_partitions[indice];


    // IMPORTANTE:
    // Estos cambios SOLO ocurren sobre la copia
    // almacenada en memoria RAM.

    particion.part_status =
        '1';


    particion.part_correlative =
        correlativo;


    // El ID tiene exactamente 4 caracteres
    // en ejemplos como 341A.
    memset(
        particion.part_id,
        0,
        sizeof(particion.part_id)
    );


    if (
        id.length()
        <= sizeof(particion.part_id)
    )
    {
        memcpy(
            particion.part_id,
            id.data(),
            id.length()
        );
    }
    else
    {
        archivo.close();

        return
            "Error: el ID generado supera "
            "el tamaño permitido por part_id.";
    }


    // =====================================
    // REGISTRAR MONTAJE EN RAM
    // =====================================

    MountedPartition mounted;


    mounted.id =
        id;


    mounted.path =
        path;


    mounted.name =
        params.name;


    mounted.diskLetter =
        letra;


    mounted.correlative =
        correlativo;


    mounted.partition =
        particion;


    particionesMontadas.push_back(
        mounted
    );


    archivo.close();


    // =====================================
    // RESPUESTA
    // =====================================

    return
        "Partición montada correctamente. ID: "
        + id;
}


// ======================================================
// MOSTRAR PARTICIONES MONTADAS
// ======================================================

string mostrarParticionesMontadas()
{
    if (particionesMontadas.empty())
    {
        return "No existen particiones montadas.";
    }

    stringstream salida;

    salida << "========== PARTICIONES MONTADAS ==========" << endl;

    for (const MountedPartition& mounted : particionesMontadas)
    {
        salida << "ID: "
               << mounted.id
               << endl;

        salida << "Nombre: "
               << mounted.name
               << endl;

        salida << "Path: "
               << mounted.path
               << endl;

        salida << "Disco: "
               << mounted.diskLetter
               << endl;

        salida << "Correlativo: "
               << mounted.correlative
               << endl;

        salida << "Tipo: "
               << mounted.partition.part_type
               << endl;

        salida << "Inicio: "
               << mounted.partition.part_start
               << endl;

        salida << "Tamaño: "
               << mounted.partition.part_s
               << endl;

        salida << "------------------------------------------"
               << endl;
    }

    return salida.str();
}


// ======================================================
// BUSCAR PARTICION MONTADA POR ID
// ======================================================

bool buscarParticionMontada(
    const string& id,
    MountedPartition& resultado
)
{
    for (
        const MountedPartition& mounted :
        particionesMontadas
    )
    {
        if (
            mounted.id == id
        )
        {
            resultado =
                mounted;

            return true;
        }
    }


    return false;
}