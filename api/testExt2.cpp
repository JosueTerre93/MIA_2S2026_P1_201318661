#include <iostream>
#include "Estructuras/Ext2Structures.h"

using namespace std;

int main()
{
    cout << "========== TAMAÑOS EXT2 ==========" << endl;

    cout << "SuperBlock: "
         << sizeof(SuperBlock)
         << " bytes"
         << endl;

    cout << "Inode: "
         << sizeof(Inode)
         << " bytes"
         << endl;

    cout << "Content: "
         << sizeof(Content)
         << " bytes"
         << endl;

    cout << "FolderBlock: "
         << sizeof(FolderBlock)
         << " bytes"
         << endl;

    cout << "FileBlock: "
         << sizeof(FileBlock)
         << " bytes"
         << endl;

    cout << "PointerBlock: "
         << sizeof(PointerBlock)
         << " bytes"
         << endl;

    return 0;
}