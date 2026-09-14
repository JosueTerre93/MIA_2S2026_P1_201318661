#include "Rep.h"

#include "Mount.h"

#include "../Estructuras/Ext2Structures.h"

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


    /*
        Por ahora implementamos SB.
    */

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

    return
        "Error: el reporte "
        + params.name
        + " aún no está implementado.";
}