#include <iostream>

#include "Utils/Ext2Utils.h"

using namespace std;

int main()
{
    int inicioParticion = 1024;
    int tamanioParticion = 1024 * 1024;

    int inodos =
        calcularNumeroInodos(
            tamanioParticion
        );

    int bloques =
        calcularNumeroBloques(
            inodos
        );

    SuperBlock sb =
        crearSuperBlock(
            inicioParticion,
            tamanioParticion
        );

    cout << "===== EXT2 TEST =====" << endl;

    cout
        << "Inodos: "
        << inodos
        << endl;

    cout
        << "Bloques: "
        << bloques
        << endl;

    cout
        << "Filesystem type: "
        << sb.s_filesystem_type
        << endl;

    cout
        << "Magic: "
        << std::hex
        << sb.s_magic
        << std::dec
        << endl;

    cout
        << "Bitmap inodos inicia: "
        << sb.s_bm_inode_start
        << endl;

    cout
        << "Bitmap bloques inicia: "
        << sb.s_bm_block_start
        << endl;

    cout
        << "Tabla inodos inicia: "
        << sb.s_inode_start
        << endl;

    cout
        << "Bloques inician: "
        << sb.s_block_start
        << endl;

    return 0;
}