#include "Rep.h"

#include "Mount.h"

#include "../Estructuras/Ext2Structures.h"
#include "../Estructuras/DiskStructures.h"

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;

namespace fs = std::filesystem;


/*
    ==================================================
                VALIDAR TIPO DE REPORTE
    ==================================================
*/

static bool nombreReporteValido(
    const string& nombre)
{
    return
        nombre == "mbr" ||
        nombre == "disk" ||
        nombre == "inode" ||
        nombre == "block" ||
        nombre == "bm_inode" ||
        nombre == "bm_block" ||
        nombre == "tree" ||
        nombre == "sb" ||
        nombre == "file" ||
        nombre == "ls";
}


/*
    ==================================================
                FORMATEAR FECHA
    ==================================================
*/

static string formatearFechaRep(
    time_t fecha)
{
    if (fecha == 0)
    {
        return "-";
    }

    struct tm tiempo{};

    localtime_r(
        &fecha,
        &tiempo
    );

    char buffer[64];

    strftime(
        buffer,
        sizeof(buffer),
        "%d/%m/%Y %H:%M:%S",
        &tiempo
    );

    return string(buffer);
}


/*
    ==================================================
              OBTENER FORMATO GRAPHVIZ
    ==================================================
*/

static string obtenerFormatoRep(
    const string& path)
{
    fs::path ruta(path);

    string extension =
        ruta.extension().string();

    if (extension == ".png")
        return "png";

    if (
        extension == ".jpg" ||
        extension == ".jpeg"
    )
    {
        return "jpg";
    }

    if (extension == ".pdf")
        return "pdf";

    if (extension == ".svg")
        return "svg";

    return "";
}


/*
    ==================================================
             ESCAPAR RUTA PARA TERMINAL
    ==================================================
*/

static string escaparShellRep(
    const string& texto)
{
    string resultado = "'";

    for (char c : texto)
    {
        if (c == '\'')
        {
            resultado += "'\\''";
        }
        else
        {
            resultado += c;
        }
    }

    resultado += "'";

    return resultado;
}

/*
    ==================================================
                  REPORTE DE INODOS
    ==================================================
*/

static string escaparRutaShell(
    const string& ruta)
{
    string resultado = "'";

    for (char c : ruta)
    {
        if (c == '\'')
        {
            resultado += "'\\''";
        }
        else
        {
            resultado += c;
        }
    }

    resultado += "'";

    return resultado;
}

static string generarReporteInode(
    const RepParams& params,
    const MountedPartition& mounted)
{
    /*
        Abrir el disco.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    int inicioParticion =
        mounted.partition.part_start;

    SuperBlock sb{};

    archivo.seekg(
        inicioParticion,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    /*
        Validar EXT2.
    */

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Crear carpeta de salida.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        Archivo DOT temporal.
    */

    string dotPath =
        params.path + ".dot";

    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear el archivo DOT.";
    }


    /*
        Inicio del documento Graphviz.
    */

    dot << "digraph G {\n";
    dot << "rankdir=LR;\n";
    dot << "node [shape=plaintext];\n\n";


    int inodosOcupados = 0;


    /*
        Recorrer bitmap de inodos.
    */

    for (
        int i = 0;
        i < sb.s_inodes_count;
        i++
    )
    {
        char estado;


        /*
            Leer estado del inodo
            desde el bitmap.
        */

        archivo.seekg(
            sb.s_bm_inode_start + i,
            ios::beg
        );

        archivo.read(
            &estado,
            sizeof(char)
        );

        if (!archivo)
        {
            dot.close();
            archivo.close();

            remove(
                dotPath.c_str()
            );

            return
                "Error: no se pudo leer el bitmap de inodos.";
        }


        /*
            Solo interesan los
            inodos ocupados.
        */

        if (estado != '1')
        {
            continue;
        }


        /*
            Leer el Inode correspondiente.
        */

        Inode inode{};

        long long posicionInodo =
            static_cast<long long>(
                sb.s_inode_start
            )
            +
            static_cast<long long>(i)
            *
            sizeof(Inode);


        archivo.seekg(
            posicionInodo,
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&inode),
            sizeof(Inode)
        );

        if (!archivo)
        {
            dot.close();
            archivo.close();

            remove(
                dotPath.c_str()
            );

            return
                "Error: no se pudo leer el inodo "
                + to_string(i)
                + ".";
        }


        inodosOcupados++;


        /*
            Crear tabla para este inodo.
        */

        dot
            << "inode"
            << i
            << " [label=<\n";

        dot
            << "<TABLE BORDER=\"1\" "
            << "CELLBORDER=\"1\" "
            << "CELLSPACING=\"0\">\n";


        /*
            Encabezado.
        */

        dot
            << "<TR>"
            << "<TD COLSPAN=\"2\">"
            << "<B>INODO "
            << i
            << "</B>"
            << "</TD>"
            << "</TR>\n";


        /*
            UID
        */

        dot
            << "<TR>"
            << "<TD>i_uid</TD>"
            << "<TD>"
            << inode.i_uid
            << "</TD>"
            << "</TR>\n";


        /*
            GID
        */

        dot
            << "<TR>"
            << "<TD>i_gid</TD>"
            << "<TD>"
            << inode.i_gid
            << "</TD>"
            << "</TR>\n";


        /*
            Tamaño
        */

        dot
            << "<TR>"
            << "<TD>i_s</TD>"
            << "<TD>"
            << inode.i_s
            << "</TD>"
            << "</TR>\n";


        /*
            Fecha de último acceso.
        */

        dot
            << "<TR>"
            << "<TD>i_atime</TD>"
            << "<TD>"
            << formatearFechaRep(
                inode.i_atime
            )
            << "</TD>"
            << "</TR>\n";


        /*
            Fecha de creación.
        */

        dot
            << "<TR>"
            << "<TD>i_ctime</TD>"
            << "<TD>"
            << formatearFechaRep(
                inode.i_ctime
            )
            << "</TD>"
            << "</TR>\n";


        /*
            Fecha de modificación.
        */

        dot
            << "<TR>"
            << "<TD>i_mtime</TD>"
            << "<TD>"
            << formatearFechaRep(
                inode.i_mtime
            )
            << "</TD>"
            << "</TR>\n";


        /*
            Mostrar los 15 apuntadores.
        */

        for (
            int j = 0;
            j < 15;
            j++
        )
        {
            dot
                << "<TR>"
                << "<TD>i_block["
                << j
                << "]</TD>"
                << "<TD>"
                << inode.i_block[j]
                << "</TD>"
                << "</TR>\n";
        }


        /*
            Tipo.

            0 = carpeta
            1 = archivo
        */

        dot
            << "<TR>"
            << "<TD>i_type</TD>"
            << "<TD>"
            << inode.i_type
            << "</TD>"
            << "</TR>\n";


        /*
            Permisos.
        */

        string permisos;

        permisos += inode.i_perm[0];
        permisos += inode.i_perm[1];
        permisos += inode.i_perm[2];


        dot
            << "<TR>"
            << "<TD>i_perm</TD>"
            << "<TD>"
            << permisos
            << "</TD>"
            << "</TR>\n";


        /*
            Cerrar tabla.
        */

        dot
            << "</TABLE>\n";

        dot
            << ">];\n\n";
    }


    /*
        Si no encontramos inodos ocupados,
        agregar un mensaje al gráfico.
    */

    if (inodosOcupados == 0)
    {
        dot
            << "vacio "
            << "[label=\"No hay inodos ocupados\"];\n";
    }


    /*
        Cerrar Graphviz.
    */

    dot << "}\n";

    dot.close();
    archivo.close();


    /*
        Obtener formato según extensión
        solicitada en -path.
    */

    string extension =
        rutaSalida.extension().string();

    if (
        !extension.empty()
        &&
        extension[0] == '.'
    )
    {
        extension.erase(
            0,
            1
        );
    }


    /*
        Graphviz usa jpg,
        no jpeg.
    */

    if (extension == "jpeg")
    {
        extension = "jpg";
    }


    if (
        extension != "png"
        &&
        extension != "jpg"
        &&
        extension != "pdf"
        &&
        extension != "svg"
    )
    {
        remove(
            dotPath.c_str()
        );

        return
            "Error: formato no soportado para el reporte inode.";
    }


    /*
        Ejecutar Graphviz.
    */

    string comando =
        "dot -T"
        + extension
        + " "
        + escaparRutaShell(dotPath)
        + " -o "
        + escaparRutaShell(params.path);


    int resultado =
        system(
            comando.c_str()
        );


    if (resultado != 0)
    {
        return
            "Error: Graphviz no pudo generar el reporte inode.";
    }


    /*
        Eliminar DOT temporal.
    */

    remove(
        dotPath.c_str()
    );


    return
        "Reporte INODE generado correctamente: "
        + params.path;
}

/*
    ==================================================
              GENERAR ARCHIVO GRAPHVIZ
    ==================================================
*/

static bool ejecutarGraphvizRep(
    const string& dotPath,
    const string& outputPath,
    const string& formato)
{
    string comando =
        "dot -T"
        + formato
        + " "
        + escaparShellRep(dotPath)
        + " -o "
        + escaparShellRep(outputPath);

    int resultado =
        system(comando.c_str());

    return resultado == 0;
}


/*
    ==================================================
                  REPORTE SUPERBLOCK
    ==================================================
*/

static string generarReporteSb(
    const RepParams& params,
    const MountedPartition& mounted)
{
    /*
        Abrir disco.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.

        El SuperBlock comienza al inicio
        de la partición.
    */

    int inicioParticion =
        mounted.partition.part_start;

    SuperBlock sb{};

    archivo.seekg(
        inicioParticion,
        ios::beg
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo posicionar en la partición.";
    }


    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }

    archivo.close();


    /*
        Validar EXT2.
    */

    if (sb.s_magic != 0xEF53)
    {
        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Crear carpeta del reporte.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        Determinar formato.
    */

    string formato =
        obtenerFormatoRep(
            params.path
        );

    if (formato.empty())
    {
        return
            "Error: extensión de reporte no soportada. "
            "Utilice .png, .jpg, .jpeg, .pdf o .svg.";
    }


    /*
        Archivo .dot temporal.
    */

    string dotPath =
        params.path
        + ".dot";


    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        return
            "Error: no se pudo crear el archivo DOT.";
    }


    /*
        ==============================================
                    GENERAR GRAPHVIZ
        ==============================================
    */

    dot << "digraph G {\n";

    dot << "    graph [pad=\"0.5\", nodesep=\"0.5\", ranksep=\"1\"];\n";

    dot << "    node [shape=plain];\n\n";


    dot << "    superbloque [label=<\n";

    dot << "    <table border=\"1\" "
           "cellborder=\"1\" "
           "cellspacing=\"0\" "
           "cellpadding=\"6\">\n";


    /*
        Encabezado.
    */

    dot << "        <tr>\n";

    dot << "            <td colspan=\"2\" "
           "bgcolor=\"lightblue\">"
           "<b>SUPER BLOQUE</b>"
           "</td>\n";

    dot << "        </tr>\n";


    /*
        Valores.
    */

    dot << "        <tr>"
           "<td><b>s_filesystem_type</b></td>"
           "<td>"
        << sb.s_filesystem_type
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_inodes_count</b></td>"
           "<td>"
        << sb.s_inodes_count
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_blocks_count</b></td>"
           "<td>"
        << sb.s_blocks_count
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_free_blocks_count</b></td>"
           "<td>"
        << sb.s_free_blocks_count
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_free_inodes_count</b></td>"
           "<td>"
        << sb.s_free_inodes_count
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_mtime</b></td>"
           "<td>"
        << formatearFechaRep(
               sb.s_mtime
           )
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_umtime</b></td>"
           "<td>"
        << formatearFechaRep(
               sb.s_umtime
           )
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_mnt_count</b></td>"
           "<td>"
        << sb.s_mnt_count
        << "</td></tr>\n";


    /*
        Mostrar magic en hexadecimal.
    */

    stringstream magic;

    magic
        << "0x"
        << hex
        << uppercase
        << sb.s_magic;


    dot << "        <tr>"
           "<td><b>s_magic</b></td>"
           "<td>"
        << magic.str()
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_inode_s</b></td>"
           "<td>"
        << sb.s_inode_s
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_block_s</b></td>"
           "<td>"
        << sb.s_block_s
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_firts_ino</b></td>"
           "<td>"
        << sb.s_firts_ino
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_first_blo</b></td>"
           "<td>"
        << sb.s_first_blo
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_bm_inode_start</b></td>"
           "<td>"
        << sb.s_bm_inode_start
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_bm_block_start</b></td>"
           "<td>"
        << sb.s_bm_block_start
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_inode_start</b></td>"
           "<td>"
        << sb.s_inode_start
        << "</td></tr>\n";


    dot << "        <tr>"
           "<td><b>s_block_start</b></td>"
           "<td>"
        << sb.s_block_start
        << "</td></tr>\n";


    dot << "    </table>\n";
    dot << "    >];\n";

    dot << "}\n";

    dot.close();


    /*
        Ejecutar Graphviz.
    */

    if (!ejecutarGraphvizRep(
            dotPath,
            params.path,
            formato))
    {
        return
            "Error: Graphviz no pudo generar el reporte.";
    }


    /*
        Eliminar .dot temporal.

        Si quieres conservarlo para depuración,
        puedes comentar esta línea.
    */

    remove(
        dotPath.c_str()
    );


    return
        "Reporte SB generado correctamente: "
        + params.path;
}

/*
    ==================================================
              REPORTE BITMAP DE INODOS
    ==================================================
*/

static string generarReporteBmInode(
    const RepParams& params,
    const MountedPartition& mounted)
{
    /*
        Abrir disco.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    int inicioParticion =
        mounted.partition.part_start;

    SuperBlock sb{};

    archivo.seekg(
        inicioParticion,
        ios::beg
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo posicionar en la partición.";
    }


    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    /*
        Validar EXT2.
    */

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Crear carpeta del reporte si
        todavía no existe.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        Abrir archivo TXT del reporte.
    */

    ofstream reporte(
        params.path
    );

    if (!reporte.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear el reporte.";
    }


    /*
        Posicionarnos al inicio del
        bitmap de inodos.
    */

    archivo.seekg(
        sb.s_bm_inode_start,
        ios::beg
    );

    if (!archivo)
    {
        archivo.close();
        reporte.close();

        return
            "Error: no se pudo acceder al bitmap de inodos.";
    }


    /*
        Leer todos los registros del bitmap.

        El enunciado solicita mostrar
        20 registros por línea.
    */

    for (int i = 0;
         i < sb.s_inodes_count;
         i++)
    {
        char estado;

        archivo.read(
            &estado,
            sizeof(char)
        );

        if (!archivo)
        {
            archivo.close();
            reporte.close();

            return
                "Error: no se pudo leer el bitmap de inodos.";
        }


        reporte << estado;


        /*
            Separar los registros de
            una misma línea.
        */

        if (
            (i + 1) % 20 == 0
        )
        {
            reporte << "\n";
        }
        else
        {
            reporte << " ";
        }
    }


    /*
        Si la última línea no tenía
        exactamente 20 registros,
        agregar salto de línea.
    */

    if (
        sb.s_inodes_count % 20 != 0
    )
    {
        reporte << "\n";
    }


    archivo.close();
    reporte.close();


    return
        "Reporte BM_INODE generado correctamente: "
        + params.path;
}

/*
    ==================================================
              REPORTE BITMAP DE BLOQUES
    ==================================================
*/

static string generarReporteBmBlock(
    const RepParams& params,
    const MountedPartition& mounted)
{
    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    int inicioParticion =
        mounted.partition.part_start;

    SuperBlock sb{};

    archivo.seekg(
        inicioParticion,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    /*
        Validar EXT2.
    */

    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Crear carpeta del reporte.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        Crear archivo TXT.
    */

    ofstream reporte(
        params.path
    );

    if (!reporte.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear el reporte.";
    }


    /*
        Ir al inicio del bitmap
        de bloques.
    */

    archivo.seekg(
        sb.s_bm_block_start,
        ios::beg
    );

    if (!archivo)
    {
        archivo.close();
        reporte.close();

        return
            "Error: no se pudo acceder al bitmap de bloques.";
    }


    /*
        Leer todos los bloques.

        Se escriben 20 registros
        por línea.
    */

    for (
        int i = 0;
        i < sb.s_blocks_count;
        i++
    )
    {
        char estado;

        archivo.read(
            &estado,
            sizeof(char)
        );

        if (!archivo)
        {
            archivo.close();
            reporte.close();

            return
                "Error: no se pudo leer el bitmap de bloques.";
        }


        reporte << estado;


        if (
            (i + 1) % 20 == 0
        )
        {
            reporte << "\n";
        }
        else
        {
            reporte << " ";
        }
    }


    /*
        Salto de línea final si
        no terminó exactamente
        en un grupo de 20.
    */

    if (
        sb.s_blocks_count % 20 != 0
    )
    {
        reporte << "\n";
    }


    archivo.close();
    reporte.close();


    return
        "Reporte BM_BLOCK generado correctamente: "
        + params.path;
}


/*
    ==================================================
                  REPORTE DE BLOQUES
    ==================================================
*/

static string generarReporteBlock(
    const RepParams& params,
    const MountedPartition& mounted)
{
    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    SuperBlock sb{};

    archivo.seekg(
        mounted.partition.part_start,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Crear carpeta de salida.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        DOT temporal.
    */

    string dotPath =
        params.path + ".dot";

    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear el archivo DOT.";
    }


    dot << "digraph G {\n";
    dot << "rankdir=LR;\n";
    dot << "node [shape=plaintext];\n\n";


    /*
        Marcamos los tipos de bloque.

        0 = desconocido
        1 = FolderBlock
        2 = FileBlock
        3 = PointerBlock
    */

    vector<int> tipoBloque(
        sb.s_blocks_count,
        0
    );


    /*
        Recorrer inodos ocupados para determinar
        qué tipo de bloque utiliza cada uno.
    */

    for (
        int i = 0;
        i < sb.s_inodes_count;
        i++
    )
    {
        char estado;

        archivo.seekg(
            sb.s_bm_inode_start + i,
            ios::beg
        );

        archivo.read(
            &estado,
            sizeof(char)
        );

        if (!archivo)
        {
            dot.close();
            archivo.close();

            remove(
                dotPath.c_str()
            );

            return
                "Error: no se pudo leer el bitmap de inodos.";
        }


        if (estado != '1')
        {
            continue;
        }


        Inode inode{};

        archivo.seekg(
            sb.s_inode_start
            +
            static_cast<long long>(i)
            *
            sizeof(Inode),
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&inode),
            sizeof(Inode)
        );

        if (!archivo)
        {
            dot.close();
            archivo.close();

            remove(
                dotPath.c_str()
            );

            return
                "Error: no se pudo leer un inodo.";
        }


        /*
            Apuntadores directos.
        */

        for (
            int j = 0;
            j < 12;
            j++
        )
        {
            int bloque =
                inode.i_block[j];

            if (
                bloque < 0
                ||
                bloque >= sb.s_blocks_count
            )
            {
                continue;
            }


            if (inode.i_type == '0')
            {
                tipoBloque[bloque] = 1;
            }
            else
            {
                tipoBloque[bloque] = 2;
            }
        }


        /*
            Apuntadores indirectos.

            i_block[12] = simple
            i_block[13] = doble
            i_block[14] = triple
        */

        for (
            int j = 12;
            j < 15;
            j++
        )
        {
            int bloque =
                inode.i_block[j];

            if (
                bloque >= 0
                &&
                bloque < sb.s_blocks_count
            )
            {
                tipoBloque[bloque] = 3;
            }
        }
    }


    /*
        Recorrer bitmap de bloques.
    */

    int bloquesOcupados = 0;

    for (
        int i = 0;
        i < sb.s_blocks_count;
        i++
    )
    {
        char estado;

        archivo.seekg(
            sb.s_bm_block_start + i,
            ios::beg
        );

        archivo.read(
            &estado,
            sizeof(char)
        );

        if (!archivo)
        {
            dot.close();
            archivo.close();

            remove(
                dotPath.c_str()
            );

            return
                "Error: no se pudo leer el bitmap de bloques.";
        }


        if (estado != '1')
        {
            continue;
        }


        bloquesOcupados++;


        long long posicionBloque =
            static_cast<long long>(
                sb.s_block_start
            )
            +
            static_cast<long long>(i)
            *
            sizeof(FileBlock);


        /*
            ===================================
                    FOLDER BLOCK
            ===================================
        */

        if (tipoBloque[i] == 1)
        {
            FolderBlock bloque{};

            archivo.seekg(
                posicionBloque,
                ios::beg
            );

            archivo.read(
                reinterpret_cast<char*>(&bloque),
                sizeof(FolderBlock)
            );


            dot
                << "block"
                << i
                << " [label=<\n";

            dot
                << "<TABLE BORDER=\"1\" "
                << "CELLBORDER=\"1\" "
                << "CELLSPACING=\"0\">\n";

            dot
                << "<TR>"
                << "<TD COLSPAN=\"2\">"
                << "<B>FOLDER BLOCK "
                << i
                << "</B>"
                << "</TD>"
                << "</TR>\n";


            for (
                int j = 0;
                j < 4;
                j++
            )
            {
                string nombre(
                    bloque.b_content[j].b_name,
                    strnlen(
                        bloque.b_content[j].b_name,
                        12
                    )
                );


                dot
                    << "<TR>"
                    << "<TD>"
                    << nombre
                    << "</TD>"
                    << "<TD>"
                    << bloque.b_content[j].b_inodo
                    << "</TD>"
                    << "</TR>\n";
            }


            dot
                << "</TABLE>\n"
                << ">];\n\n";
        }


        /*
            ===================================
                     FILE BLOCK
            ===================================
        */

        else if (tipoBloque[i] == 2)
        {
            FileBlock bloque{};

            archivo.seekg(
                posicionBloque,
                ios::beg
            );

            archivo.read(
                reinterpret_cast<char*>(&bloque),
                sizeof(FileBlock)
            );


            string contenido;

            for (
                int j = 0;
                j < 64;
                j++
            )
            {
                char c =
                    bloque.b_content[j];

                if (c == '\0')
                {
                    break;
                }

                /*
                    Escapar caracteres especiales
                    para tabla HTML de Graphviz.
                */

                if (c == '<')
                {
                    contenido += "&lt;";
                }
                else if (c == '>')
                {
                    contenido += "&gt;";
                }
                else if (c == '&')
                {
                    contenido += "&amp;";
                }
                else if (c == '"')
                {
                    contenido += "&quot;";
                }
                else if (c == '\n')
                {
                    contenido += "<BR/>";
                }
                else
                {
                    contenido += c;
                }
            }


            dot
                << "block"
                << i
                << " [label=<\n";

            dot
                << "<TABLE BORDER=\"1\" "
                << "CELLBORDER=\"1\" "
                << "CELLSPACING=\"0\">\n";

            dot
                << "<TR>"
                << "<TD>"
                << "<B>FILE BLOCK "
                << i
                << "</B>"
                << "</TD>"
                << "</TR>\n";

            dot
                << "<TR>"
                << "<TD>"
                << contenido
                << "</TD>"
                << "</TR>\n";

            dot
                << "</TABLE>\n"
                << ">];\n\n";
        }


        /*
            ===================================
                   POINTER BLOCK
            ===================================
        */

        else if (tipoBloque[i] == 3)
        {
            PointerBlock bloque{};

            archivo.seekg(
                posicionBloque,
                ios::beg
            );

            archivo.read(
                reinterpret_cast<char*>(&bloque),
                sizeof(PointerBlock)
            );


            dot
                << "block"
                << i
                << " [label=<\n";

            dot
                << "<TABLE BORDER=\"1\" "
                << "CELLBORDER=\"1\" "
                << "CELLSPACING=\"0\">\n";

            dot
                << "<TR>"
                << "<TD COLSPAN=\"2\">"
                << "<B>POINTER BLOCK "
                << i
                << "</B>"
                << "</TD>"
                << "</TR>\n";


            for (
                int j = 0;
                j < 16;
                j++
            )
            {
                dot
                    << "<TR>"
                    << "<TD>b_pointers["
                    << j
                    << "]</TD>"
                    << "<TD>"
                    << bloque.b_pointers[j]
                    << "</TD>"
                    << "</TR>\n";
            }


            dot
                << "</TABLE>\n"
                << ">];\n\n";
        }


        /*
            Si el bloque está marcado como ocupado
            pero no pudimos determinar su tipo.
        */

        else
        {
            dot
                << "block"
                << i
                << " [label=<\n";

            dot
                << "<TABLE BORDER=\"1\" "
                << "CELLBORDER=\"1\" "
                << "CELLSPACING=\"0\">\n";

            dot
                << "<TR>"
                << "<TD>"
                << "<B>BLOQUE "
                << i
                << "</B>"
                << "</TD>"
                << "</TR>\n";

            dot
                << "<TR>"
                << "<TD>Tipo no determinado</TD>"
                << "</TR>\n";

            dot
                << "</TABLE>\n"
                << ">];\n\n";
        }
    }


    if (bloquesOcupados == 0)
    {
        dot
            << "vacio "
            << "[label=\"No hay bloques ocupados\"];\n";
    }


    dot << "}\n";

    dot.close();
    archivo.close();


    /*
        Obtener formato.
    */

    string extension =
        rutaSalida.extension().string();

    if (
        !extension.empty()
        &&
        extension[0] == '.'
    )
    {
        extension.erase(
            0,
            1
        );
    }


    if (extension == "jpeg")
    {
        extension = "jpg";
    }


    if (
        extension != "png"
        &&
        extension != "jpg"
        &&
        extension != "pdf"
        &&
        extension != "svg"
    )
    {
        remove(
            dotPath.c_str()
        );

        return
            "Error: formato no soportado para el reporte block.";
    }


    string comando =
        "dot -T"
        + extension
        + " "
        + escaparRutaShell(dotPath)
        + " -o "
        + escaparRutaShell(params.path);


    int resultado =
        system(
            comando.c_str()
        );


    if (resultado != 0)
    {
        return
            "Error: Graphviz no pudo generar el reporte block.";
    }


    remove(
        dotPath.c_str()
    );


    return
        "Reporte BLOCK generado correctamente: "
        + params.path;
}

/*
    ==================================================
              BUSCAR ENTRADA EN DIRECTORIO
    ==================================================
*/

static int buscarEntradaRep(
    ifstream& archivo,
    const SuperBlock& sb,
    int inodeIndex,
    const string& nombre)
{
    Inode inode{};

    archivo.clear();

    archivo.seekg(
        sb.s_inode_start
        + static_cast<long long>(inodeIndex)
        * sizeof(Inode),
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&inode),
        sizeof(Inode)
    );

    if (!archivo || inode.i_type != '0')
    {
        return -1;
    }

    for (int i = 0; i < 12; i++)
    {
        int blockIndex =
            inode.i_block[i];

        if (
            blockIndex < 0
            ||
            blockIndex >= sb.s_blocks_count
        )
        {
            continue;
        }

        FolderBlock bloque{};

        archivo.clear();

        archivo.seekg(
            sb.s_block_start
            + static_cast<long long>(blockIndex)
            * sizeof(FileBlock),
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&bloque),
            sizeof(FolderBlock)
        );

        if (!archivo)
        {
            return -1;
        }

        for (int j = 0; j < 4; j++)
        {
            if (
                bloque.b_content[j].b_inodo < 0
            )
            {
                continue;
            }

            string nombreActual(
                bloque.b_content[j].b_name,
                strnlen(
                    bloque.b_content[j].b_name,
                    12
                )
            );

            if (nombreActual == nombre)
            {
                return
                    bloque.b_content[j].b_inodo;
            }
        }
    }

    return -1;
}


/*
    ==================================================
                BUSCAR INODO POR RUTA
    ==================================================
*/

static int buscarInodoPorRutaRep(
    ifstream& archivo,
    const SuperBlock& sb,
    const string& ruta)
{
    if (
        ruta.empty()
        ||
        ruta[0] != '/'
    )
    {
        return -1;
    }

    if (ruta == "/")
    {
        return 0;
    }

    stringstream ss(ruta);
    string parte;

    int inodeActual = 0;

    while (
        getline(
            ss,
            parte,
            '/'
        )
    )
    {
        if (parte.empty())
        {
            continue;
        }

        inodeActual =
            buscarEntradaRep(
                archivo,
                sb,
                inodeActual,
                parte
            );

        if (inodeActual < 0)
        {
            return -1;
        }
    }

    return inodeActual;
}


/*
    ==================================================
             LEER BLOQUES DE UN ARCHIVO
    ==================================================
*/

static bool leerBloquesArchivoRep(
    ifstream& archivo,
    const SuperBlock& sb,
    int blockIndex,
    int nivel,
    string& contenido,
    long long& restantes)
{
    if (
        blockIndex < 0
        ||
        blockIndex >= sb.s_blocks_count
        ||
        restantes <= 0
    )
    {
        return true;
    }

    long long posicion =
        static_cast<long long>(
            sb.s_block_start
        )
        +
        static_cast<long long>(
            blockIndex
        )
        * sizeof(FileBlock);


    /*
        nivel 0:
        bloque real de contenido.
    */

    if (nivel == 0)
    {
        FileBlock bloque{};

        archivo.clear();

        archivo.seekg(
            posicion,
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&bloque),
            sizeof(FileBlock)
        );

        if (!archivo)
        {
            return false;
        }

        long long cantidad =
            min<long long>(
                restantes,
                sizeof(bloque.b_content)
            );

        contenido.append(
            bloque.b_content,
            static_cast<size_t>(
                cantidad
            )
        );

        restantes -= cantidad;

        return true;
    }


    /*
        nivel > 0:
        bloque de apuntadores.
    */

    PointerBlock bloque{};

    archivo.clear();

    archivo.seekg(
        posicion,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&bloque),
        sizeof(PointerBlock)
    );

    if (!archivo)
    {
        return false;
    }

    for (int i = 0; i < 16; i++)
    {
        if (restantes <= 0)
        {
            break;
        }

        if (bloque.b_pointers[i] < 0)
        {
            continue;
        }

        if (
            !leerBloquesArchivoRep(
                archivo,
                sb,
                bloque.b_pointers[i],
                nivel - 1,
                contenido,
                restantes
            )
        )
        {
            return false;
        }
    }

    return true;
}


/*
    ==================================================
             LEER CONTENIDO COMPLETO
    ==================================================
*/

static bool leerContenidoArchivoRep(
    ifstream& archivo,
    const SuperBlock& sb,
    const Inode& inode,
    string& contenido)
{
    contenido.clear();

    long long restantes =
        inode.i_s;


    /*
        12 apuntadores directos.
    */

    for (
        int i = 0;
        i < 12 && restantes > 0;
        i++
    )
    {
        if (inode.i_block[i] < 0)
        {
            continue;
        }

        if (
            !leerBloquesArchivoRep(
                archivo,
                sb,
                inode.i_block[i],
                0,
                contenido,
                restantes
            )
        )
        {
            return false;
        }
    }


    /*
        Indirecto simple.
    */

    if (
        restantes > 0
        &&
        inode.i_block[12] >= 0
    )
    {
        if (
            !leerBloquesArchivoRep(
                archivo,
                sb,
                inode.i_block[12],
                1,
                contenido,
                restantes
            )
        )
        {
            return false;
        }
    }


    /*
        Indirecto doble.
    */

    if (
        restantes > 0
        &&
        inode.i_block[13] >= 0
    )
    {
        if (
            !leerBloquesArchivoRep(
                archivo,
                sb,
                inode.i_block[13],
                2,
                contenido,
                restantes
            )
        )
        {
            return false;
        }
    }


    /*
        Indirecto triple.
    */

    if (
        restantes > 0
        &&
        inode.i_block[14] >= 0
    )
    {
        if (
            !leerBloquesArchivoRep(
                archivo,
                sb,
                inode.i_block[14],
                3,
                contenido,
                restantes
            )
        )
        {
            return false;
        }
    }

    return true;
}


/*
    ==================================================
               ESCAPAR HTML GRAPHVIZ
    ==================================================
*/

static string escaparHtmlRep(
    const string& texto)
{
    string resultado;

    for (char c : texto)
    {
        switch (c)
        {
            case '&':
                resultado += "&amp;";
                break;

            case '<':
                resultado += "&lt;";
                break;

            case '>':
                resultado += "&gt;";
                break;

            case '"':
                resultado += "&quot;";
                break;

            case '\n':
                resultado +=
                    "<BR ALIGN=\"LEFT\"/>";
                break;

            case '\r':
                break;

            default:
                resultado += c;
                break;
        }
    }

    return resultado;
}

/*
    ==================================================
                    REPORTE FILE
    ==================================================
*/

static string generarReporteFile(
    const RepParams& params,
    const MountedPartition& mounted)
{
    if (params.pathFileLs.empty())
    {
        return
            "Error: el reporte file requiere -path_file_ls.";
    }


    /*
        Abrir disco.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    SuperBlock sb{};

    archivo.seekg(
        mounted.partition.part_start,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Buscar el inodo correspondiente
        a -path_file_ls.
    */

    int inodeIndex =
        buscarInodoPorRutaRep(
            archivo,
            sb,
            params.pathFileLs
        );

    if (inodeIndex < 0)
    {
        archivo.close();

        return
            "Error: no existe el archivo "
            + params.pathFileLs
            + ".";
    }


    /*
        Leer inodo.
    */

    Inode inode{};

    archivo.clear();

    archivo.seekg(
        sb.s_inode_start
        +
        static_cast<long long>(
            inodeIndex
        )
        * sizeof(Inode),
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&inode),
        sizeof(Inode)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el inodo del archivo.";
    }


    /*
        FILE solamente acepta archivos.
    */

    if (inode.i_type != '1')
    {
        archivo.close();

        return
            "Error: "
            + params.pathFileLs
            + " no es un archivo.";
    }


    /*
        Leer contenido completo.
    */

    string contenido;

    if (
        !leerContenidoArchivoRep(
            archivo,
            sb,
            inode,
            contenido
        )
    )
    {
        archivo.close();

        return
            "Error: no se pudo leer el contenido del archivo.";
    }

    archivo.close();


    /*
        Obtener nombre.
    */

    fs::path rutaInterna(
        params.pathFileLs
    );

    string nombreArchivo =
        rutaInterna.filename().string();


    /*
        Crear carpeta de salida.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        Obtener formato utilizando el helper
        que YA tienes en Rep.cpp.
    */

    string formato =
        obtenerFormatoRep(
            params.path
        );

    if (formato.empty())
    {
        return
            "Error: extensión de reporte no soportada. "
            "Utilice .png, .jpg, .jpeg, .pdf o .svg.";
    }


    /*
        DOT temporal.
    */

    string dotPath =
        params.path + ".dot";

    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        return
            "Error: no se pudo crear el archivo DOT.";
    }


    /*
        Generar Graphviz.
    */

    dot << "digraph G {\n";

    dot << "    node [shape=plain];\n\n";

    dot << "    archivo [label=<\n";

    dot
        << "    <table border=\"1\" "
        << "cellborder=\"1\" "
        << "cellspacing=\"0\" "
        << "cellpadding=\"6\">\n";


    dot
        << "        <tr>"
        << "<td colspan=\"2\" bgcolor=\"lightblue\">"
        << "<b>REPORTE FILE</b>"
        << "</td>"
        << "</tr>\n";


    dot
        << "        <tr>"
        << "<td><b>Nombre</b></td>"
        << "<td>"
        << escaparHtmlRep(
            nombreArchivo
        )
        << "</td>"
        << "</tr>\n";


    dot
        << "        <tr>"
        << "<td><b>Contenido</b></td>"
        << "<td align=\"left\">"
        << escaparHtmlRep(
            contenido
        )
        << "</td>"
        << "</tr>\n";


    dot << "    </table>\n";

    dot << "    >];\n";

    dot << "}\n";

    dot.close();


    /*
        Usamos ejecutarGraphvizRep()
        que YA existe en tu archivo.
    */

    if (
        !ejecutarGraphvizRep(
            dotPath,
            params.path,
            formato
        )
    )
    {
        return
            "Error: Graphviz no pudo generar el reporte file.";
    }


    remove(
        dotPath.c_str()
    );


    return
        "Reporte FILE generado correctamente: "
        + params.path;
}

/*
    ==================================================
             CONVERTIR PERMISOS A TEXTO
    ==================================================
*/

static string permisosTextoRep(
    const Inode& inode)
{
    string resultado;

    for (int i = 0; i < 3; i++)
    {
        int valor =
            inode.i_perm[i] - '0';

        resultado +=
            (valor & 4) ? 'r' : '-';

        resultado +=
            (valor & 2) ? 'w' : '-';

        resultado +=
            (valor & 1) ? 'x' : '-';
    }

    return resultado;
}


/*
    ==================================================
                 DIVIDIR CSV
    ==================================================
*/

static vector<string> dividirCsvRep(
    const string& linea)
{
    vector<string> partes;

    stringstream ss(linea);
    string parte;

    while (
        getline(
            ss,
            parte,
            ','
        )
    )
    {
        partes.push_back(parte);
    }

    return partes;
}


/*
    ==================================================
            CARGAR USUARIOS Y GRUPOS
    ==================================================
*/

static bool cargarUsuariosGruposRep(
    ifstream& archivo,
    const SuperBlock& sb,
    map<int, string>& usuarios,
    map<int, string>& grupos)
{
    Inode usersInode{};

    archivo.clear();

    archivo.seekg(
        sb.s_inode_start
        +
        static_cast<long long>(1)
        * sizeof(Inode),
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(
            &usersInode
        ),
        sizeof(Inode)
    );

    if (!archivo)
    {
        return false;
    }


    string contenido;

    if (
        !leerContenidoArchivoRep(
            archivo,
            sb,
            usersInode,
            contenido
        )
    )
    {
        return false;
    }


    stringstream ss(contenido);
    string linea;

    while (getline(ss, linea))
    {
        if (linea.empty())
        {
            continue;
        }

        vector<string> campos =
            dividirCsvRep(linea);

        if (campos.size() < 3)
        {
            continue;
        }

        int id = 0;

        try
        {
            id = stoi(
                campos[0]
            );
        }
        catch (...)
        {
            continue;
        }

        /*
            ID 0 = eliminado.
        */

        if (id == 0)
        {
            continue;
        }

        /*
            Grupo:
            ID,G,nombre
        */

        if (
            campos[1] == "G"
            &&
            campos.size() >= 3
        )
        {
            grupos[id] =
                campos[2];
        }

        /*
            Usuario:
            ID,U,grupo,usuario,password
        */

        else if (
            campos[1] == "U"
            &&
            campos.size() >= 5
        )
        {
            usuarios[id] =
                campos[3];
        }
    }

    return true;
}

/*
    ==================================================
                     REPORTE LS
    ==================================================
*/

static string generarReporteLs(
    const RepParams& params,
    const MountedPartition& mounted)
{
    if (params.pathFileLs.empty())
    {
        return
            "Error: el reporte ls requiere -path_file_ls.";
    }


    /*
        Abrir disco.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    SuperBlock sb{};

    archivo.seekg(
        mounted.partition.part_start,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Buscar ruta indicada.
    */

    int inodeIndex =
        buscarInodoPorRutaRep(
            archivo,
            sb,
            params.pathFileLs
        );

    if (inodeIndex < 0)
    {
        archivo.close();

        return
            "Error: no existe la ruta "
            + params.pathFileLs
            + ".";
    }


    /*
        Cargar nombres de usuarios
        y grupos.
    */

    map<int, string> usuarios;
    map<int, string> grupos;

    cargarUsuariosGruposRep(
        archivo,
        sb,
        usuarios,
        grupos
    );


    /*
        Crear directorio de salida.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    string formato =
        obtenerFormatoRep(
            params.path
        );

    if (formato.empty())
    {
        archivo.close();

        return
            "Error: extensión de reporte no soportada. "
            "Utilice .png, .jpg, .jpeg, .pdf o .svg.";
    }


    /*
        Crear DOT.
    */

    string dotPath =
        params.path + ".dot";

    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear el archivo DOT.";
    }


    dot << "digraph G {\n";
    dot << "node [shape=plain];\n\n";

    dot << "ls [label=<\n";

    dot
        << "<TABLE BORDER=\"1\" "
        << "CELLBORDER=\"1\" "
        << "CELLSPACING=\"0\" "
        << "CELLPADDING=\"5\">\n";


    /*
        Encabezado.
    */

    dot
        << "<TR>"
        << "<TD COLSPAN=\"8\" BGCOLOR=\"lightblue\">"
        << "<B>REPORTE LS - "
        << escaparHtmlRep(
            params.pathFileLs
        )
        << "</B>"
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD><B>Permisos</B></TD>"
        << "<TD><B>Propietario</B></TD>"
        << "<TD><B>Grupo</B></TD>"
        << "<TD><B>Tamaño</B></TD>"
        << "<TD><B>Fecha creación</B></TD>"
        << "<TD><B>Fecha modificación</B></TD>"
        << "<TD><B>Tipo</B></TD>"
        << "<TD><B>Nombre</B></TD>"
        << "</TR>\n";


    /*
        Helper local para escribir
        una fila.
    */

    auto escribirFila =
        [&](
            int indice,
            const string& nombre)
    {
        Inode inode{};

        archivo.clear();

        archivo.seekg(
            sb.s_inode_start
            +
            static_cast<long long>(
                indice
            )
            * sizeof(Inode),
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(
                &inode
            ),
            sizeof(Inode)
        );

        if (!archivo)
        {
            return;
        }


        string propietario =
            to_string(
                inode.i_uid
            );

        string grupo =
            to_string(
                inode.i_gid
            );


        /*
            Buscar nombre de propietario.
        */

        auto itUsuario =
            usuarios.find(
                inode.i_uid
            );

        if (
            itUsuario !=
            usuarios.end()
        )
        {
            propietario =
                itUsuario->second;
        }


        /*
            Buscar nombre del grupo.
        */

        auto itGrupo =
            grupos.find(
                inode.i_gid
            );

        if (
            itGrupo !=
            grupos.end()
        )
        {
            grupo =
                itGrupo->second;
        }


        string tipo;

        if (inode.i_type == '0')
        {
            tipo = "Carpeta";
        }
        else if (inode.i_type == '1')
        {
            tipo = "Archivo";
        }
        else
        {
            tipo = "Desconocido";
        }


        string permisos =
            permisosTextoRep(
                inode
            );


        /*
            También mostramos el valor UGO,
            por ejemplo:
            rw-rw-r-- (664)
        */

        string permisosNumericos;

        permisosNumericos +=
            inode.i_perm[0];

        permisosNumericos +=
            inode.i_perm[1];

        permisosNumericos +=
            inode.i_perm[2];


        dot
            << "<TR>";


        dot
            << "<TD>"
            << permisos
            << " ("
            << permisosNumericos
            << ")"
            << "</TD>";


        dot
            << "<TD>"
            << escaparHtmlRep(
                propietario
            )
            << "</TD>";


        dot
            << "<TD>"
            << escaparHtmlRep(
                grupo
            )
            << "</TD>";


        dot
            << "<TD>"
            << inode.i_s
            << "</TD>";


        dot
            << "<TD>"
            << formatearFechaRep(
                inode.i_ctime
            )
            << "</TD>";


        dot
            << "<TD>"
            << formatearFechaRep(
                inode.i_mtime
            )
            << "</TD>";


        dot
            << "<TD>"
            << tipo
            << "</TD>";


        dot
            << "<TD>"
            << escaparHtmlRep(
                nombre
            )
            << "</TD>";


        dot
            << "</TR>\n";
    };


    /*
        Leer inodo solicitado.
    */

    Inode inodeObjetivo{};

    archivo.clear();

    archivo.seekg(
        sb.s_inode_start
        +
        static_cast<long long>(
            inodeIndex
        )
        * sizeof(Inode),
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(
            &inodeObjetivo
        ),
        sizeof(Inode)
    );

    if (!archivo)
    {
        dot.close();
        archivo.close();

        remove(
            dotPath.c_str()
        );

        return
            "Error: no se pudo leer el inodo solicitado.";
    }


    /*
        Si es archivo:
        mostrar únicamente ese archivo.
    */

    if (inodeObjetivo.i_type == '1')
    {
        fs::path interna(
            params.pathFileLs
        );

        string nombre =
            interna.filename().string();

        escribirFila(
            inodeIndex,
            nombre
        );
    }


    /*
        Si es carpeta:
        listar sus entradas.
    */

    else if (inodeObjetivo.i_type == '0')
    {
        for (int i = 0; i < 12; i++)
        {
            int blockIndex =
                inodeObjetivo.i_block[i];

            if (
                blockIndex < 0
                ||
                blockIndex >= sb.s_blocks_count
            )
            {
                continue;
            }


            FolderBlock bloque{};

            archivo.clear();

            archivo.seekg(
                sb.s_block_start
                +
                static_cast<long long>(
                    blockIndex
                )
                * sizeof(FileBlock),
                ios::beg
            );

            archivo.read(
                reinterpret_cast<char*>(
                    &bloque
                ),
                sizeof(FolderBlock)
            );

            if (!archivo)
            {
                dot.close();
                archivo.close();

                remove(
                    dotPath.c_str()
                );

                return
                    "Error: no se pudo leer un bloque de carpeta.";
            }


            for (int j = 0; j < 4; j++)
            {
                int indice =
                    bloque.b_content[j].b_inodo;

                if (indice < 0)
                {
                    continue;
                }


                string nombre(
                    bloque.b_content[j].b_name,
                    strnlen(
                        bloque.b_content[j].b_name,
                        12
                    )
                );


                /*
                    No mostrar . ni ..
                */

                if (
                    nombre == "."
                    ||
                    nombre == ".."
                )
                {
                    continue;
                }


                escribirFila(
                    indice,
                    nombre
                );
            }
        }
    }


    dot << "</TABLE>\n";
    dot << ">];\n";
    dot << "}\n";


    dot.close();
    archivo.close();


    /*
        Generar imagen.
    */

    if (
        !ejecutarGraphvizRep(
            dotPath,
            params.path,
            formato
        )
    )
    {
        return
            "Error: Graphviz no pudo generar el reporte ls.";
    }


    remove(
        dotPath.c_str()
    );


    return
        "Reporte LS generado correctamente: "
        + params.path;
}

/*
    ==================================================
              LEER INODO PARA TREE
    ==================================================
*/

static bool leerInodoTreeRep(
    ifstream& archivo,
    const SuperBlock& sb,
    int indice,
    Inode& inode)
{
    if (
        indice < 0
        ||
        indice >= sb.s_inodes_count
    )
    {
        return false;
    }

    archivo.clear();

    archivo.seekg(
        sb.s_inode_start
        +
        static_cast<long long>(indice)
        * sizeof(Inode),
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&inode),
        sizeof(Inode)
    );

    return static_cast<bool>(archivo);
}


/*
    ==================================================
           DECLARACIÓN RECURSIVA TREE
    ==================================================
*/

static void generarInodoTreeRep(
    ifstream& archivo,
    const SuperBlock& sb,
    int inodeIndex,
    ofstream& dot,
    vector<bool>& inodosVisitados,
    vector<bool>& bloquesVisitados);


/*
    ==================================================
             GENERAR BLOQUE PARA TREE
    ==================================================
*/

static void generarBloqueTreeRep(
    ifstream& archivo,
    const SuperBlock& sb,
    int blockIndex,
    char tipoInodo,
    int nivel,
    ofstream& dot,
    vector<bool>& inodosVisitados,
    vector<bool>& bloquesVisitados)
{
    if (
        blockIndex < 0
        ||
        blockIndex >= sb.s_blocks_count
    )
    {
        return;
    }


    /*
        Evitar dibujar dos veces
        el mismo bloque.
    */

    if (bloquesVisitados[blockIndex])
    {
        return;
    }

    bloquesVisitados[blockIndex] = true;


    long long posicion =
        static_cast<long long>(
            sb.s_block_start
        )
        +
        static_cast<long long>(
            blockIndex
        )
        * sizeof(FileBlock);


    /*
        ==================================================
                  BLOQUE DE APUNTADORES
        ==================================================
    */

    if (nivel > 0)
    {
        PointerBlock bloque{};

        archivo.clear();

        archivo.seekg(
            posicion,
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&bloque),
            sizeof(PointerBlock)
        );

        if (!archivo)
        {
            archivo.clear();
            return;
        }


        dot
            << "block"
            << blockIndex
            << " [label=<\n";

        dot
            << "<TABLE BORDER=\"1\" "
            << "CELLBORDER=\"1\" "
            << "CELLSPACING=\"0\" "
            << "CELLPADDING=\"5\">\n";

        dot
            << "<TR>"
            << "<TD COLSPAN=\"2\" BGCOLOR=\"khaki\">"
            << "<B>POINTER BLOCK "
            << blockIndex
            << "</B>"
            << "</TD>"
            << "</TR>\n";


        for (int i = 0; i < 16; i++)
        {
            dot
                << "<TR>"
                << "<TD>pointer["
                << i
                << "]</TD>"
                << "<TD PORT=\"p"
                << i
                << "\">"
                << bloque.b_pointers[i]
                << "</TD>"
                << "</TR>\n";
        }


        dot << "</TABLE>\n";
        dot << ">];\n\n";


        /*
            Recorrer apuntadores.
        */

        for (int i = 0; i < 16; i++)
        {
            int siguiente =
                bloque.b_pointers[i];

            if (
                siguiente < 0
                ||
                siguiente >= sb.s_blocks_count
            )
            {
                continue;
            }


            dot
                << "block"
                << blockIndex
                << ":p"
                << i
                << " -> block"
                << siguiente
                << ";\n";


            generarBloqueTreeRep(
                archivo,
                sb,
                siguiente,
                tipoInodo,
                nivel - 1,
                dot,
                inodosVisitados,
                bloquesVisitados
            );
        }

        return;
    }


    /*
        ==================================================
                   FOLDER BLOCK
        ==================================================
    */

    if (tipoInodo == '0')
    {
        FolderBlock bloque{};

        archivo.clear();

        archivo.seekg(
            posicion,
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&bloque),
            sizeof(FolderBlock)
        );

        if (!archivo)
        {
            archivo.clear();
            return;
        }


        dot
            << "block"
            << blockIndex
            << " [label=<\n";

        dot
            << "<TABLE BORDER=\"1\" "
            << "CELLBORDER=\"1\" "
            << "CELLSPACING=\"0\" "
            << "CELLPADDING=\"5\">\n";

        dot
            << "<TR>"
            << "<TD COLSPAN=\"2\" BGCOLOR=\"lightgreen\">"
            << "<B>FOLDER BLOCK "
            << blockIndex
            << "</B>"
            << "</TD>"
            << "</TR>\n";


        for (int i = 0; i < 4; i++)
        {
            string nombre(
                bloque.b_content[i].b_name,
                strnlen(
                    bloque.b_content[i].b_name,
                    12
                )
            );


            dot
                << "<TR>"
                << "<TD>"
                << escaparHtmlRep(nombre)
                << "</TD>"
                << "<TD PORT=\"c"
                << i
                << "\">"
                << bloque.b_content[i].b_inodo
                << "</TD>"
                << "</TR>\n";
        }


        dot << "</TABLE>\n";
        dot << ">];\n\n";


        /*
            Conectar carpeta con sus inodos hijos.

            No seguimos "." ni ".." porque
            crearían ciclos.
        */

        for (int i = 0; i < 4; i++)
        {
            int hijo =
                bloque.b_content[i].b_inodo;

            if (
                hijo < 0
                ||
                hijo >= sb.s_inodes_count
            )
            {
                continue;
            }


            string nombre(
                bloque.b_content[i].b_name,
                strnlen(
                    bloque.b_content[i].b_name,
                    12
                )
            );


            if (
                nombre == "."
                ||
                nombre == ".."
                ||
                nombre.empty()
            )
            {
                continue;
            }


            dot
                << "block"
                << blockIndex
                << ":c"
                << i
                << " -> inode"
                << hijo
                << ";\n";


            generarInodoTreeRep(
                archivo,
                sb,
                hijo,
                dot,
                inodosVisitados,
                bloquesVisitados
            );
        }

        return;
    }


    /*
        ==================================================
                     FILE BLOCK
        ==================================================
    */

    if (tipoInodo == '1')
    {
        FileBlock bloque{};

        archivo.clear();

        archivo.seekg(
            posicion,
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(&bloque),
            sizeof(FileBlock)
        );

        if (!archivo)
        {
            archivo.clear();
            return;
        }


        string contenido;

        for (int i = 0; i < 64; i++)
        {
            char c =
                bloque.b_content[i];

            if (c == '\0')
            {
                break;
            }

            contenido += c;
        }


        dot
            << "block"
            << blockIndex
            << " [label=<\n";

        dot
            << "<TABLE BORDER=\"1\" "
            << "CELLBORDER=\"1\" "
            << "CELLSPACING=\"0\" "
            << "CELLPADDING=\"5\">\n";

        dot
            << "<TR>"
            << "<TD BGCOLOR=\"lightyellow\">"
            << "<B>FILE BLOCK "
            << blockIndex
            << "</B>"
            << "</TD>"
            << "</TR>\n";

        dot
            << "<TR>"
            << "<TD ALIGN=\"LEFT\">"
            << escaparHtmlRep(contenido)
            << "</TD>"
            << "</TR>\n";

        dot << "</TABLE>\n";
        dot << ">];\n\n";

        return;
    }
}


/*
    ==================================================
                 GENERAR INODO TREE
    ==================================================
*/

static void generarInodoTreeRep(
    ifstream& archivo,
    const SuperBlock& sb,
    int inodeIndex,
    ofstream& dot,
    vector<bool>& inodosVisitados,
    vector<bool>& bloquesVisitados)
{
    if (
        inodeIndex < 0
        ||
        inodeIndex >= sb.s_inodes_count
    )
    {
        return;
    }


    /*
        Evitar ciclos.
    */

    if (inodosVisitados[inodeIndex])
    {
        return;
    }

    inodosVisitados[inodeIndex] = true;


    /*
        Verificar bitmap.
    */

    char estado = '0';

    archivo.clear();

    archivo.seekg(
        sb.s_bm_inode_start
        + inodeIndex,
        ios::beg
    );

    archivo.read(
        &estado,
        sizeof(char)
    );

    if (
        !archivo
        ||
        estado != '1'
    )
    {
        archivo.clear();
        return;
    }


    /*
        Leer inodo.
    */

    Inode inode{};

    if (
        !leerInodoTreeRep(
            archivo,
            sb,
            inodeIndex,
            inode
        )
    )
    {
        return;
    }


    string permisos;

    permisos += inode.i_perm[0];
    permisos += inode.i_perm[1];
    permisos += inode.i_perm[2];


    string tipo;

    if (inode.i_type == '0')
    {
        tipo = "Carpeta";
    }
    else if (inode.i_type == '1')
    {
        tipo = "Archivo";
    }
    else
    {
        tipo = "Desconocido";
    }


    /*
        Dibujar inodo.
    */

    dot
        << "inode"
        << inodeIndex
        << " [label=<\n";

    dot
        << "<TABLE BORDER=\"1\" "
        << "CELLBORDER=\"1\" "
        << "CELLSPACING=\"0\" "
        << "CELLPADDING=\"5\">\n";


    dot
        << "<TR>"
        << "<TD COLSPAN=\"2\" BGCOLOR=\"lightblue\">"
        << "<B>INODO "
        << inodeIndex
        << "</B>"
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_uid</TD>"
        << "<TD>"
        << inode.i_uid
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_gid</TD>"
        << "<TD>"
        << inode.i_gid
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_s</TD>"
        << "<TD>"
        << inode.i_s
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_atime</TD>"
        << "<TD>"
        << formatearFechaRep(
            inode.i_atime
        )
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_ctime</TD>"
        << "<TD>"
        << formatearFechaRep(
            inode.i_ctime
        )
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_mtime</TD>"
        << "<TD>"
        << formatearFechaRep(
            inode.i_mtime
        )
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_type</TD>"
        << "<TD>"
        << tipo
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD>i_perm</TD>"
        << "<TD>"
        << permisos
        << "</TD>"
        << "</TR>\n";


    /*
        Mostrar los 15 apuntadores.
    */

    for (int i = 0; i < 15; i++)
    {
        dot
            << "<TR>"
            << "<TD>i_block["
            << i
            << "]</TD>"
            << "<TD PORT=\"b"
            << i
            << "\">"
            << inode.i_block[i]
            << "</TD>"
            << "</TR>\n";
    }


    dot << "</TABLE>\n";
    dot << ">];\n\n";


    /*
        ==================================================
                APUNTADORES DIRECTOS
        ==================================================
    */

    for (int i = 0; i < 12; i++)
    {
        int bloque =
            inode.i_block[i];

        if (
            bloque < 0
            ||
            bloque >= sb.s_blocks_count
        )
        {
            continue;
        }


        dot
            << "inode"
            << inodeIndex
            << ":b"
            << i
            << " -> block"
            << bloque
            << ";\n";


        generarBloqueTreeRep(
            archivo,
            sb,
            bloque,
            inode.i_type,
            0,
            dot,
            inodosVisitados,
            bloquesVisitados
        );
    }


    /*
        ==================================================
             APUNTADOR INDIRECTO SIMPLE
        ==================================================
    */

    if (
        inode.i_block[12] >= 0
        &&
        inode.i_block[12] < sb.s_blocks_count
    )
    {
        dot
            << "inode"
            << inodeIndex
            << ":b12 -> block"
            << inode.i_block[12]
            << ";\n";

        generarBloqueTreeRep(
            archivo,
            sb,
            inode.i_block[12],
            inode.i_type,
            1,
            dot,
            inodosVisitados,
            bloquesVisitados
        );
    }


    /*
        ==================================================
              APUNTADOR INDIRECTO DOBLE
        ==================================================
    */

    if (
        inode.i_block[13] >= 0
        &&
        inode.i_block[13] < sb.s_blocks_count
    )
    {
        dot
            << "inode"
            << inodeIndex
            << ":b13 -> block"
            << inode.i_block[13]
            << ";\n";

        generarBloqueTreeRep(
            archivo,
            sb,
            inode.i_block[13],
            inode.i_type,
            2,
            dot,
            inodosVisitados,
            bloquesVisitados
        );
    }


    /*
        ==================================================
              APUNTADOR INDIRECTO TRIPLE
        ==================================================
    */

    if (
        inode.i_block[14] >= 0
        &&
        inode.i_block[14] < sb.s_blocks_count
    )
    {
        dot
            << "inode"
            << inodeIndex
            << ":b14 -> block"
            << inode.i_block[14]
            << ";\n";

        generarBloqueTreeRep(
            archivo,
            sb,
            inode.i_block[14],
            inode.i_type,
            3,
            dot,
            inodosVisitados,
            bloquesVisitados
        );
    }
}


/*
    ==================================================
                     REPORTE TREE
    ==================================================
*/

static string generarReporteTree(
    const RepParams& params,
    const MountedPartition& mounted)
{
    /*
        Abrir disco.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer SuperBlock.
    */

    SuperBlock sb{};

    archivo.seekg(
        mounted.partition.part_start,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(&sb),
        sizeof(SuperBlock)
    );

    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el SuperBlock.";
    }


    if (sb.s_magic != 0xEF53)
    {
        archivo.close();

        return
            "Error: la partición no contiene un sistema EXT2 válido.";
    }


    /*
        Crear carpeta destino.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear la carpeta del reporte.";
        }
    }


    /*
        Formato.
    */

    string formato =
        obtenerFormatoRep(
            params.path
        );

    if (formato.empty())
    {
        archivo.close();

        return
            "Error: extensión de reporte no soportada. "
            "Utilice .png, .jpg, .jpeg, .pdf o .svg.";
    }


    /*
        DOT temporal.
    */

    string dotPath =
        params.path + ".dot";

    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear el archivo DOT.";
    }


    /*
        Inicio Graphviz.
    */

    dot << "digraph G {\n";
    dot << "rankdir=LR;\n";
    dot << "node [shape=plain];\n";
    dot << "graph [nodesep=\"0.5\", ranksep=\"1\"];\n\n";


    /*
        Controlar ciclos.
    */

    vector<bool> inodosVisitados(
        sb.s_inodes_count,
        false
    );

    vector<bool> bloquesVisitados(
        sb.s_blocks_count,
        false
    );


    /*
        EXT2 comienza desde el
        inodo raíz = 0.
    */

    generarInodoTreeRep(
        archivo,
        sb,
        0,
        dot,
        inodosVisitados,
        bloquesVisitados
    );


    dot << "}\n";

    dot.close();
    archivo.close();


    /*
        Ejecutar Graphviz.
    */

    if (
        !ejecutarGraphvizRep(
            dotPath,
            params.path,
            formato
        )
    )
    {
        return
            "Error: Graphviz no pudo generar el reporte tree.";
    }


    /*
        Eliminar DOT.
    */

    remove(
        dotPath.c_str()
    );


    return
        "Reporte TREE generado correctamente: "
        + params.path;
}

/*
    ==================================================
                    REPORTE MBR
    ==================================================
*/

static string generarReporteMbr(
    const RepParams& params,
    const MountedPartition& mounted)
{
    /*
        Abrir el disco completo.

        Para MBR no usamos part_start porque
        el MBR siempre está en el byte 0.
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );

    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        Leer MBR.
    */

    MBR mbr{};

    archivo.seekg(
        0,
        ios::beg
    );

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


    /*
        Crear carpeta destino.
    */

    fs::path rutaSalida(
        params.path
    );

    if (rutaSalida.has_parent_path())
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear "
                "la carpeta del reporte.";
        }
    }


    /*
        Obtener formato de salida.
    */

    string formato =
        obtenerFormatoRep(
            params.path
        );

    if (formato.empty())
    {
        archivo.close();

        return
            "Error: extensión de reporte no soportada. "
            "Utilice .png, .jpg, .jpeg, .pdf o .svg.";
    }


    /*
        Archivo DOT temporal.
    */

    string dotPath =
        params.path + ".dot";

    ofstream dot(
        dotPath
    );

    if (!dot.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear "
            "el archivo DOT.";
    }


    /*
        ==================================================
                        GRAPHVIZ
        ==================================================
    */

    dot << "digraph G {\n";

    dot
        << "node [shape=plain];\n";

    dot
        << "graph ["
        << "rankdir=TB, "
        << "nodesep=\"0.5\", "
        << "ranksep=\"0.7\""
        << "];\n\n";


    /*
        ==================================================
                         TABLA MBR
        ==================================================
    */

    dot
        << "mbr [label=<\n";

    dot
        << "<TABLE BORDER=\"1\" "
        << "CELLBORDER=\"1\" "
        << "CELLSPACING=\"0\" "
        << "CELLPADDING=\"6\">\n";


    dot
        << "<TR>"
        << "<TD COLSPAN=\"2\" "
        << "BGCOLOR=\"lightblue\">"
        << "<B>MBR</B>"
        << "</TD>"
        << "</TR>\n";


    /*
        Información general.
    */

    dot
        << "<TR>"
        << "<TD><B>mbr_tamano</B></TD>"
        << "<TD>"
        << mbr.mbr_tamano
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD><B>mbr_fecha_creacion</B></TD>"
        << "<TD>"
        << formatearFechaRep(
            mbr.mbr_fecha_creacion
        )
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD><B>mbr_dsk_signature</B></TD>"
        << "<TD>"
        << mbr.mbr_dsk_signature
        << "</TD>"
        << "</TR>\n";


    dot
        << "<TR>"
        << "<TD><B>dsk_fit</B></TD>"
        << "<TD>"
        << mbr.dsk_fit
        << "</TD>"
        << "</TR>\n";


    /*
        ==================================================
                LAS CUATRO PARTICIONES DEL MBR
        ==================================================
    */

    for (int i = 0; i < 4; i++)
    {
        const Partition& p =
            mbr.mbr_partitions[i];


        string numero =
            to_string(i + 1);


        /*
            Separador visual.
        */

        dot
            << "<TR>"
            << "<TD COLSPAN=\"2\" "
            << "BGCOLOR=\"lightgray\">"
            << "<B>PARTICION "
            << numero
            << "</B>"
            << "</TD>"
            << "</TR>\n";


        dot
            << "<TR>"
            << "<TD>part_status</TD>"
            << "<TD>"
            << p.part_status
            << "</TD>"
            << "</TR>\n";


        dot
            << "<TR>"
            << "<TD>part_type</TD>"
            << "<TD>"
            << p.part_type
            << "</TD>"
            << "</TR>\n";


        dot
            << "<TR>"
            << "<TD>part_fit</TD>"
            << "<TD>"
            << p.part_fit
            << "</TD>"
            << "</TR>\n";


        dot
            << "<TR>"
            << "<TD>part_start</TD>"
            << "<TD>"
            << p.part_start
            << "</TD>"
            << "</TR>\n";


        dot
            << "<TR>"
            << "<TD>part_s</TD>"
            << "<TD>"
            << p.part_s
            << "</TD>"
            << "</TR>\n";


        /*
            Nombre seguro porque es char[16].
        */

        string nombreParticion(
            p.part_name,
            strnlen(
                p.part_name,
                sizeof(p.part_name)
            )
        );


        dot
            << "<TR>"
            << "<TD>part_name</TD>"
            << "<TD>"
            << escaparHtmlRep(
                nombreParticion
            )
            << "</TD>"
            << "</TR>\n";


        dot
            << "<TR>"
            << "<TD>part_correlative</TD>"
            << "<TD>"
            << p.part_correlative
            << "</TD>"
            << "</TR>\n";


        /*
            part_id puede no terminar en \0,
            así que usamos strnlen.
        */

        string idParticion(
            p.part_id,
            strnlen(
                p.part_id,
                sizeof(p.part_id)
            )
        );


        dot
            << "<TR>"
            << "<TD>part_id</TD>"
            << "<TD>"
            << escaparHtmlRep(
                idParticion
            )
            << "</TD>"
            << "</TR>\n";
    }


    dot << "</TABLE>\n";
    dot << ">];\n\n";


    /*
        ==================================================
                  BUSCAR PARTICIÓN EXTENDIDA
        ==================================================

        Tu implementación permite solamente una
        partición extendida, pero recorremos las cuatro
        entradas por seguridad.
    */

    int contadorEbr = 0;


    for (int i = 0; i < 4; i++)
    {
        const Partition& extendida =
            mbr.mbr_partitions[i];


        if (
            extendida.part_status == '0'
            ||
            extendida.part_type != 'E'
            ||
            extendida.part_start < 0
            ||
            extendida.part_s <= 0
        )
        {
            continue;
        }


        int inicioExtendida =
            extendida.part_start;

        int finExtendida =
            extendida.part_start
            +
            extendida.part_s;


        int posicion =
            inicioExtendida;


        /*
            Para evitar cadenas dañadas o ciclos.
        */

        int seguridad = 0;


        while (
            posicion >= inicioExtendida
            &&
            posicion
                + static_cast<int>(
                    sizeof(EBR)
                )
                <= finExtendida
            &&
            seguridad < 1000
        )
        {
            seguridad++;


            EBR ebr{};

            archivo.clear();

            archivo.seekg(
                posicion,
                ios::beg
            );

            archivo.read(
                reinterpret_cast<char*>(
                    &ebr
                ),
                sizeof(EBR)
            );


            if (!archivo)
            {
                archivo.clear();
                break;
            }


            /*
                Generamos también el EBR vacío inicial
                de una extendida si todavía no contiene
                particiones lógicas.

                Eso permite visualizar realmente lo que
                existe en disco.
            */

            string nombreEbr(
                ebr.part_name,
                strnlen(
                    ebr.part_name,
                    sizeof(ebr.part_name)
                )
            );


            string nombreNodo =
                "ebr"
                +
                to_string(
                    contadorEbr
                );


            dot
                << nombreNodo
                << " [label=<\n";


            dot
                << "<TABLE BORDER=\"1\" "
                << "CELLBORDER=\"1\" "
                << "CELLSPACING=\"0\" "
                << "CELLPADDING=\"6\">\n";


            dot
                << "<TR>"
                << "<TD COLSPAN=\"2\" "
                << "BGCOLOR=\"lightyellow\">"
                << "<B>EBR "
                << contadorEbr + 1
                << "</B>"
                << "</TD>"
                << "</TR>\n";


            dot
                << "<TR>"
                << "<TD><B>part_mount</B></TD>"
                << "<TD>"
                << ebr.part_mount
                << "</TD>"
                << "</TR>\n";


            dot
                << "<TR>"
                << "<TD><B>part_fit</B></TD>"
                << "<TD>"
                << ebr.part_fit
                << "</TD>"
                << "</TR>\n";


            dot
                << "<TR>"
                << "<TD><B>part_start</B></TD>"
                << "<TD>"
                << ebr.part_start
                << "</TD>"
                << "</TR>\n";


            dot
                << "<TR>"
                << "<TD><B>part_s</B></TD>"
                << "<TD>"
                << ebr.part_s
                << "</TD>"
                << "</TR>\n";


            dot
                << "<TR>"
                << "<TD><B>part_next</B></TD>"
                << "<TD>"
                << ebr.part_next
                << "</TD>"
                << "</TR>\n";


            dot
                << "<TR>"
                << "<TD><B>part_name</B></TD>"
                << "<TD>"
                << escaparHtmlRep(
                    nombreEbr
                )
                << "</TD>"
                << "</TR>\n";


            dot
                << "</TABLE>\n";

            dot
                << ">];\n\n";


            /*
                Primer EBR conectado al MBR.
            */

            if (contadorEbr == 0)
            {
                dot
                    << "mbr -> "
                    << nombreNodo
                    << ";\n";
            }


            /*
                Conectar EBR anterior con actual.
            */

            if (contadorEbr > 0)
            {
                dot
                    << "ebr"
                    << contadorEbr - 1
                    << " -> "
                    << nombreNodo
                    << ";\n";
            }


            contadorEbr++;


            /*
                Fin de la lista enlazada.
            */

            if (ebr.part_next == -1)
            {
                break;
            }


            /*
                Evitar EBR corrupto o ciclo.
            */

            if (
                ebr.part_next <= posicion
                ||
                ebr.part_next
                    + static_cast<int>(
                        sizeof(EBR)
                    )
                    > finExtendida
            )
            {
                break;
            }


            posicion =
                ebr.part_next;
        }
    }


    dot << "}\n";

    dot.close();
    archivo.close();


    /*
        ==================================================
                    EJECUTAR GRAPHVIZ
        ==================================================
    */

    if (
        !ejecutarGraphvizRep(
            dotPath,
            params.path,
            formato
        )
    )
    {
        return
            "Error: Graphviz no pudo generar "
            "el reporte MBR.";
    }


    remove(
        dotPath.c_str()
    );


    return
        "Reporte MBR generado correctamente: "
        + params.path;
}

/*
    ==================================================
             PORCENTAJE PARA REPORTE DISK
    ==================================================
*/

static double porcentajeDiskRep(
    long long tamanio,
    long long tamanioDisco)
{
    if (tamanioDisco <= 0)
    {
        return 0.0;
    }

    return
        (
            static_cast<double>(tamanio)
            * 100.0
        )
        /
        static_cast<double>(tamanioDisco);
}


/*
    ==================================================
          FORMATEAR PORCENTAJE DISK
    ==================================================
*/

static string porcentajeTextoDiskRep(
    long long tamanio,
    long long tamanioDisco)
{
    stringstream ss;

    ss
        << fixed
        << setprecision(2)
        << porcentajeDiskRep(
            tamanio,
            tamanioDisco
        )
        << "%";

    return ss.str();
}


/*
    ==================================================
      CONTENIDO DE PARTICIÓN EXTENDIDA PARA DISK
    ==================================================
*/

static string generarContenidoExtendidaDiskRep(
    ifstream& archivo,
    const Partition& extendida,
    long long tamanioDisco)
{
    stringstream html;

    int inicioExtendida =
        extendida.part_start;

    int finExtendida =
        extendida.part_start
        +
        extendida.part_s;


    html
        << "<TABLE BORDER=\"0\" "
        << "CELLBORDER=\"1\" "
        << "CELLSPACING=\"0\" "
        << "CELLPADDING=\"5\">";


    html
        << "<TR>"
        << "<TD COLSPAN=\"50\">"
        << "<B>EXTENDIDA</B><BR/>"
        << escaparHtmlRep(
            string(
                extendida.part_name,
                strnlen(
                    extendida.part_name,
                    sizeof(
                        extendida.part_name
                    )
                )
            )
        )
        << "<BR/>"
        << porcentajeTextoDiskRep(
            extendida.part_s,
            tamanioDisco
        )
        << "</TD>"
        << "</TR>";


    html << "<TR>";


    /*
        Posición desde donde vamos calculando
        los espacios ocupados/libres.
    */

    int posicion =
        inicioExtendida;

    int seguridad = 0;


    while (
        posicion >= inicioExtendida
        &&
        posicion
            + static_cast<int>(
                sizeof(EBR)
            )
            <= finExtendida
        &&
        seguridad < 1000
    )
    {
        seguridad++;


        EBR ebr{};

        archivo.clear();

        archivo.seekg(
            posicion,
            ios::beg
        );

        archivo.read(
            reinterpret_cast<char*>(
                &ebr
            ),
            sizeof(EBR)
        );


        if (!archivo)
        {
            archivo.clear();
            break;
        }


        /*
            ==========================================
                    EBR VACÍO
            ==========================================

            Cuando se crea la extendida inicialmente,
            existe un EBR con part_s = 0.
        */

        if (ebr.part_s <= 0)
        {
            html
                << "<TD>"
                << "<B>EBR</B><BR/>"
                << porcentajeTextoDiskRep(
                    sizeof(EBR),
                    tamanioDisco
                )
                << "</TD>";


            int inicioLibre =
                posicion
                +
                static_cast<int>(
                    sizeof(EBR)
                );


            int tamanioLibre =
                finExtendida
                -
                inicioLibre;


            if (tamanioLibre > 0)
            {
                html
                    << "<TD>"
                    << "<B>LIBRE</B><BR/>"
                    << porcentajeTextoDiskRep(
                        tamanioLibre,
                        tamanioDisco
                    )
                    << "</TD>";
            }

            break;
        }


        /*
            ==========================================
                      EBR
            ==========================================
        */

        html
            << "<TD>"
            << "<B>EBR</B><BR/>"
            << porcentajeTextoDiskRep(
                sizeof(EBR),
                tamanioDisco
            )
            << "</TD>";


        /*
            ==========================================
                 PARTICIÓN LÓGICA
            ==========================================
        */

        string nombreLogica(
            ebr.part_name,
            strnlen(
                ebr.part_name,
                sizeof(
                    ebr.part_name
                )
            )
        );


        html
            << "<TD>"
            << "<B>LOGICA</B><BR/>"
            << escaparHtmlRep(
                nombreLogica
            )
            << "<BR/>"
            << porcentajeTextoDiskRep(
                ebr.part_s,
                tamanioDisco
            )
            << "</TD>";


        int finLogica =
            ebr.part_start
            +
            static_cast<int>(
                sizeof(EBR)
            )
            +
            ebr.part_s;


        /*
            ==========================================
                  ÚLTIMO EBR
            ==========================================
        */

        if (ebr.part_next == -1)
        {
            if (finLogica < finExtendida)
            {
                int libre =
                    finExtendida
                    -
                    finLogica;


                html
                    << "<TD>"
                    << "<B>LIBRE</B><BR/>"
                    << porcentajeTextoDiskRep(
                        libre,
                        tamanioDisco
                    )
                    << "</TD>";
            }

            break;
        }


        /*
            ==========================================
               ESPACIO LIBRE ENTRE LÓGICAS
            ==========================================
        */

        if (
            ebr.part_next
            >
            finLogica
        )
        {
            int libre =
                ebr.part_next
                -
                finLogica;


            html
                << "<TD>"
                << "<B>LIBRE</B><BR/>"
                << porcentajeTextoDiskRep(
                    libre,
                    tamanioDisco
                )
                << "</TD>";
        }


        /*
            Validar cadena EBR.
        */

        if (
            ebr.part_next
            <= posicion
            ||
            ebr.part_next
                + static_cast<int>(
                    sizeof(EBR)
                )
                > finExtendida
        )
        {
            break;
        }


        posicion =
            ebr.part_next;
    }


    html << "</TR>";
    html << "</TABLE>";


    return html.str();
}


/*
    ==================================================
                    REPORTE DISK
    ==================================================
*/

static string generarReporteDisk(
    const RepParams& params,
    const MountedPartition& mounted)
{
    /*
        ==================================================
                       ABRIR DISCO
        ==================================================
    */

    ifstream archivo(
        mounted.path,
        ios::binary
    );


    if (!archivo.is_open())
    {
        return
            "Error: no se pudo abrir el disco.";
    }


    /*
        ==================================================
                        LEER MBR
        ==================================================
    */

    MBR mbr{};

    archivo.seekg(
        0,
        ios::beg
    );

    archivo.read(
        reinterpret_cast<char*>(
            &mbr
        ),
        sizeof(MBR)
    );


    if (!archivo)
    {
        archivo.close();

        return
            "Error: no se pudo leer el MBR.";
    }


    if (mbr.mbr_tamano <= 0)
    {
        archivo.close();

        return
            "Error: tamaño de disco inválido.";
    }


    /*
        ==================================================
                   OBTENER PARTICIONES
        ==================================================
    */

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
            particiones.push_back(
                p
            );
        }
    }


    /*
        Ordenarlas por posición física.
    */

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


    /*
        ==================================================
                    CREAR DIRECTORIO
        ==================================================
    */

    fs::path rutaSalida(
        params.path
    );


    if (
        rutaSalida.has_parent_path()
    )
    {
        try
        {
            fs::create_directories(
                rutaSalida.parent_path()
            );
        }
        catch (...)
        {
            archivo.close();

            return
                "Error: no se pudo crear "
                "la carpeta del reporte.";
        }
    }


    /*
        ==================================================
                        FORMATO
        ==================================================
    */

    string formato =
        obtenerFormatoRep(
            params.path
        );


    if (formato.empty())
    {
        archivo.close();

        return
            "Error: extensión de reporte no soportada. "
            "Utilice .png, .jpg, .jpeg, .pdf o .svg.";
    }


    /*
        ==================================================
                      ARCHIVO DOT
        ==================================================
    */

    string dotPath =
        params.path
        +
        ".dot";


    ofstream dot(
        dotPath
    );


    if (!dot.is_open())
    {
        archivo.close();

        return
            "Error: no se pudo crear "
            "el archivo DOT.";
    }


    /*
        ==================================================
                       GRAPHVIZ
        ==================================================
    */

    dot
        << "digraph G {\n";


    dot
        << "node [shape=plain];\n";


    dot
        << "disk [label=<\n";


    dot
        << "<TABLE BORDER=\"1\" "
        << "CELLBORDER=\"1\" "
        << "CELLSPACING=\"0\" "
        << "CELLPADDING=\"8\">\n";


    /*
        ==================================================
                    ENCABEZADO
        ==================================================
    */

    dot
        << "<TR>"
        << "<TD COLSPAN=\"50\" "
        << "BGCOLOR=\"lightblue\">"
        << "<B>REPORTE DISK</B>"
        << "<BR/>"
        << escaparHtmlRep(
            mounted.path
        )
        << "</TD>"
        << "</TR>\n";


    /*
        ==================================================
                 FILA PRINCIPAL DEL DISCO
        ==================================================
    */

    dot << "<TR>\n";


    /*
        --------------------------------------------------
                           MBR
        --------------------------------------------------
    */

    long long tamanioMbr =
        sizeof(MBR);


    dot
        << "<TD>"
        << "<B>MBR</B>"
        << "<BR/>"
        << porcentajeTextoDiskRep(
            tamanioMbr,
            mbr.mbr_tamano
        )
        << "</TD>\n";


    /*
        Posición actual dentro del disco.
    */

    long long posicionActual =
        sizeof(MBR);


    /*
        ==================================================
               PARTICIONES + ESPACIOS LIBRES
        ==================================================
    */

    for (
        const Partition& p :
        particiones
    )
    {
        /*
            ----------------------------------------------
                  LIBRE ANTES DE PARTICIÓN
            ----------------------------------------------
        */

        if (
            p.part_start
            >
            posicionActual
        )
        {
            long long libre =
                static_cast<long long>(
                    p.part_start
                )
                -
                posicionActual;


            dot
                << "<TD>"
                << "<B>LIBRE</B>"
                << "<BR/>"
                << porcentajeTextoDiskRep(
                    libre,
                    mbr.mbr_tamano
                )
                << "</TD>\n";
        }


        /*
            Nombre.
        */

        string nombre(
            p.part_name,
            strnlen(
                p.part_name,
                sizeof(
                    p.part_name
                )
            )
        );


        /*
            ----------------------------------------------
                    PARTICIÓN PRIMARIA
            ----------------------------------------------
        */

        if (p.part_type == 'P')
        {
            dot
                << "<TD>"
                << "<B>PRIMARIA</B>"
                << "<BR/>"
                << escaparHtmlRep(
                    nombre
                )
                << "<BR/>"
                << porcentajeTextoDiskRep(
                    p.part_s,
                    mbr.mbr_tamano
                )
                << "</TD>\n";
        }


        /*
            ----------------------------------------------
                    PARTICIÓN EXTENDIDA
            ----------------------------------------------
        */

        else if (
            p.part_type == 'E'
        )
        {
            string contenido =
                generarContenidoExtendidaDiskRep(
                    archivo,
                    p,
                    mbr.mbr_tamano
                );


            /*
                El <TD> externo representa toda
                la extendida.

                La tabla interna solo describe
                cómo está dividida internamente.
            */

            dot
                << "<TD>"
                << contenido
                << "</TD>\n";
        }


        /*
            Tipo desconocido, por seguridad.
        */

        else
        {
            dot
                << "<TD>"
                << "<B>PARTICION</B>"
                << "<BR/>"
                << escaparHtmlRep(
                    nombre
                )
                << "<BR/>"
                << porcentajeTextoDiskRep(
                    p.part_s,
                    mbr.mbr_tamano
                )
                << "</TD>\n";
        }


        posicionActual =
            static_cast<long long>(
                p.part_start
            )
            +
            p.part_s;
    }


    /*
        ==================================================
               ESPACIO LIBRE AL FINAL
        ==================================================
    */

    if (
        posicionActual
        <
        mbr.mbr_tamano
    )
    {
        long long libre =
            static_cast<long long>(
                mbr.mbr_tamano
            )
            -
            posicionActual;


        dot
            << "<TD>"
            << "<B>LIBRE</B>"
            << "<BR/>"
            << porcentajeTextoDiskRep(
                libre,
                mbr.mbr_tamano
            )
            << "</TD>\n";
    }


    dot << "</TR>\n";


    /*
        ==================================================
                   INFORMACIÓN DEL DISCO
        ==================================================
    */

    dot
        << "<TR>"
        << "<TD COLSPAN=\"50\">"
        << "<B>Tamaño total:</B> "
        << mbr.mbr_tamano
        << " bytes"
        << "</TD>"
        << "</TR>\n";


    dot
        << "</TABLE>\n";


    dot
        << ">];\n";


    dot
        << "}\n";


    dot.close();
    archivo.close();


    /*
        ==================================================
                    GENERAR REPORTE
        ==================================================
    */

    if (
        !ejecutarGraphvizRep(
            dotPath,
            params.path,
            formato
        )
    )
    {
        return
            "Error: Graphviz no pudo generar "
            "el reporte DISK.";
    }


    remove(
        dotPath.c_str()
    );


    return
        "Reporte DISK generado correctamente: "
        + params.path;
}

/*
    ==================================================
                     EJECUTAR REP
    ==================================================
*/

string ejecutarRep(
    const RepParams& params)
{
    /*
        Parámetros obligatorios.
    */

    if (params.name.empty())
    {
        return
            "Error: debe especificar -name.";
    }


    if (params.path.empty())
    {
        return
            "Error: debe especificar -path.";
    }


    if (params.id.empty())
    {
        return
            "Error: debe especificar -id.";
    }


    /*
        Validar nombre.
    */

    if (!nombreReporteValido(
            params.name))
    {
        return
            "Error: nombre de reporte no válido: "
            + params.name
            + ".";
    }


    /*
        file y ls necesitan path_file_ls.
    */

    if (
        (
            params.name == "file" ||
            params.name == "ls"
        ) &&
        params.pathFileLs.empty()
    )
    {
        return
            "Error: el reporte "
            + params.name
            + " requiere -path_file_ls.";
    }


    /*
        Buscar partición montada mediante
        el ID proporcionado.
    */

    MountedPartition mounted;

    if (!buscarParticionMontada(
            params.id,
            mounted))
    {
        return
            "Error: no existe una partición montada con el ID "
            + params.id
            + ".";
    }


    if (params.name == "mbr")
    {
        return generarReporteMbr(
            params,
            mounted
        );
    }

    if (params.name == "sb")
    {
        return generarReporteSb(
            params,
            mounted
        );
    }

    if (params.name == "bm_inode")
    {
        return generarReporteBmInode(
            params,
            mounted
        );
    }

    

    if (params.name == "bm_block")
    {
        return generarReporteBmBlock(
            params,
            mounted
        );
    }

    if (params.name == "inode")
    {
        return generarReporteInode(
            params,
            mounted
        );
    }

    if (params.name == "block")
    {
        return generarReporteBlock(
            params,
            mounted
        );
    }

    if (params.name == "file")
    {
        return generarReporteFile(
            params,
            mounted
        );
    }

    if (params.name == "ls")
    {
        return generarReporteLs(
            params,
            mounted
        );
    }

    if (params.name == "tree")
    {
        return generarReporteTree(
            params,
            mounted
        );
    }

    if (params.name == "disk")
    {
        return generarReporteDisk(
            params,
            mounted
        );
    }

    return
        "Error: el reporte "
        + params.name
        + " aún no está implementado.";
}