#ifndef EXT2_UTILS_H
#define EXT2_UTILS_H

#include "../Estructuras/Ext2Structures.h"

// Calcula la cantidad de inodos que caben
// dentro de una partición EXT2
int calcularNumeroInodos(int tamanioParticion);

// Cantidad de bloques = 3 * cantidad de inodos
int calcularNumeroBloques(int numeroInodos);

// Crea e inicializa el SuperBlock
SuperBlock crearSuperBlock(
    int inicioParticion,
    int tamanioParticion
);

// Crea un inodo vacío
Inode crearInodoVacio();

// Crea un bloque de carpeta vacío
FolderBlock crearFolderBlockVacio();

// Crea un bloque de apuntadores vacío
PointerBlock crearPointerBlockVacio();

#endif