# Manual Técnico

## 1. Introducción

El presente manual técnico describe la arquitectura, estructuras de datos, componentes y comandos implementados en el proyecto de simulación de un sistema de archivos EXT2.

El sistema permite administrar discos virtuales mediante archivos binarios con extensión `.mia`, crear y administrar particiones, formatearlas utilizando una estructura basada en EXT2, administrar usuarios y grupos, crear archivos y carpetas y generar reportes gráficos de las diferentes estructuras almacenadas.

---

## 2. Arquitectura del Sistema

El proyecto utiliza una arquitectura cliente-servidor compuesta principalmente por un frontend y un backend.

### 2.1 Frontend

El frontend está desarrollado utilizando **Angular** y proporciona la interfaz mediante la cual el usuario puede interactuar con el sistema.

Sus principales responsabilidades son:

* Permitir el ingreso de comandos.
* Enviar los comandos al backend.
* Mostrar las respuestas generadas por el servidor.
* Permitir la carga de archivos de script `.smia`.
* Facilitar la interacción con las funcionalidades del sistema.

### 2.2 Backend

El backend está desarrollado en **C++** y contiene la lógica principal del proyecto.

Entre sus responsabilidades se encuentran:

* Administración de discos virtuales.
* Administración de particiones.
* Implementación del sistema de archivos EXT2.
* Administración de usuarios y grupos.
* Administración de archivos y carpetas.
* Manejo de sesiones.
* Generación de reportes.
* Procesamiento de comandos.

### 2.3 API REST

La comunicación entre Angular y C++ se realiza mediante una **API REST**.

El frontend envía los comandos ingresados por el usuario al servidor, donde son procesados por el parser. Posteriormente, el backend devuelve el resultado de la operación al frontend.

### 2.4 Analizador de comandos

Para interpretar los comandos se utilizan:

* Flex para el análisis léxico.
* Bison para el análisis sintáctico.

El analizador identifica comandos, parámetros y valores para posteriormente ejecutar la operación correspondiente.

### 2.5 Graphviz

Graphviz se utiliza para generar los reportes gráficos del sistema.

Los reportes permiten visualizar las estructuras internas almacenadas dentro de los discos virtuales.

---

# 3. Estructuras Utilizadas

## 3.1 MBR

El MBR contiene la información principal del disco virtual y las particiones existentes.

Almacena información como:

* Tamaño del disco.
* Fecha de creación.
* Identificador del disco.
* Tipo de ajuste.
* Tabla de particiones.

## 3.2 Partition

Representa una partición almacenada dentro del MBR.

Contiene información relacionada con:

* Estado.
* Tipo de partición.
* Ajuste.
* Posición inicial.
* Tamaño.
* Nombre.
* Correlativo.
* Identificador.

## 3.3 EBR

El EBR permite administrar las particiones lógicas existentes dentro de una partición extendida.

Los EBR forman una estructura enlazada que permite localizar las diferentes particiones lógicas.

## 3.4 SuperBlock

El SuperBlock almacena la información general del sistema de archivos EXT2.

Entre la información almacenada se encuentra:

* Cantidad de inodos.
* Cantidad de bloques.
* Inodos libres.
* Bloques libres.
* Posición del bitmap de inodos.
* Posición del bitmap de bloques.
* Inicio de la tabla de inodos.
* Inicio del área de bloques.

## 3.5 Inode

Los inodos almacenan la información correspondiente a archivos y carpetas.

Entre sus atributos se encuentran:

* UID del propietario.
* GID del grupo.
* Tamaño.
* Fechas de acceso, creación y modificación.
* Apuntadores hacia bloques.
* Tipo de inodo.
* Permisos.

## 3.6 Content

Representa una entrada dentro de un bloque de carpeta.

Cada entrada relaciona:

* Nombre del archivo o carpeta.
* Número de inodo correspondiente.

## 3.7 FolderBlock

Representa un bloque utilizado por una carpeta.

Contiene diferentes entradas de tipo `Content`, permitiendo relacionar nombres con inodos.

## 3.8 FileBlock

Representa un bloque utilizado para almacenar contenido de archivos.

Cada bloque permite almacenar hasta 64 caracteres.

## 3.9 PointerBlock

Permite implementar el direccionamiento indirecto de bloques.

Se utiliza para manejar:

* Apuntadores indirectos simples.
* Apuntadores indirectos dobles.
* Apuntadores indirectos triples.

---

# 4. Sistema de Archivos EXT2

El sistema implementa una simulación de EXT2 dentro de las particiones almacenadas en los archivos `.mia`.

La distribución incluye:

* SuperBlock.
* Bitmap de inodos.
* Bitmap de bloques.
* Tabla de inodos.
* Bloques de carpetas.
* Bloques de archivos.
* Bloques de apuntadores.

Al ejecutar `MKFS`, también se crea automáticamente el archivo:

`/users.txt`

Este archivo almacena los grupos y usuarios existentes dentro del sistema.

---

# 5. Comandos Implementados

## 5.1 Administración de discos

* `MKDISK`: crea un disco virtual.
* `RMDISK`: elimina un disco virtual.
* `FDISK`: administra las particiones.
* `MOUNT`: monta una partición.
* `MOUNTED`: muestra las particiones montadas.

## 5.2 Administración del sistema de archivos

* `MKFS`: formatea una partición utilizando EXT2.
* `CAT`: muestra el contenido de uno o varios archivos.

## 5.3 Usuarios y grupos

* `LOGIN`: inicia una sesión.
* `LOGOUT`: finaliza una sesión.
* `MKGRP`: crea un grupo.
* `RMGRP`: elimina un grupo.
* `MKUSR`: crea un usuario.
* `RMUSR`: elimina un usuario.
* `CHGRP`: cambia el grupo de un usuario.

## 5.4 Archivos y carpetas

* `MKFILE`: crea archivos.
* `MKDIR`: crea carpetas.

## 5.5 Reportes

El comando `REP` permite generar los siguientes reportes:

* MBR.
* DISK.
* INODE.
* BLOCK.
* BM_INODE.
* BM_BLOCK.
* TREE.
* SB.
* FILE.
* LS.

Los reportes gráficos son generados utilizando Graphviz.

---

# 6. Manejo de Permisos

El sistema utiliza permisos UGO:

* **U:** propietario.
* **G:** grupo.
* **O:** otros.

Los permisos utilizan la representación numérica tradicional:

* `4`: lectura.
* `2`: escritura.
* `1`: ejecución.

El usuario `root` posee privilegios especiales dentro del sistema.

---

# 7. Manejo de Sesiones

Las sesiones permiten identificar al usuario que actualmente está utilizando el sistema de archivos.

Una sesión almacena información como:

* Usuario.
* UID.
* Grupo.
* GID.
* Partición utilizada.

Después de iniciar sesión, las operaciones del sistema de archivos se realizan sobre la partición asociada a dicha sesión.

---

# 8. Generación de Reportes

Los reportes permiten inspeccionar las estructuras almacenadas dentro de los discos `.mia`.

Graphviz procesa archivos DOT generados por el backend y produce representaciones visuales de estructuras como:

* MBR.
* Distribución del disco.
* SuperBlock.
* Inodos.
* Bloques.
* Árbol del sistema de archivos.

---

# 9. Tecnologías Utilizadas

* C++.
* Flex.
* Bison.
* httplib.
* Angular.
* Graphviz.
* Git.
* GitHub.
* GNU/Linux.

---

# 10. Conclusiones

El proyecto implementa una simulación de un sistema de archivos EXT2 utilizando archivos binarios como discos virtuales.

La integración del frontend desarrollado en Angular con el backend desarrollado en C++ permite administrar las diferentes funcionalidades mediante comandos, mientras que Flex y Bison permiten analizar su sintaxis.

Finalmente, Graphviz permite visualizar las estructuras internas generadas durante las diferentes operaciones del sistema.
