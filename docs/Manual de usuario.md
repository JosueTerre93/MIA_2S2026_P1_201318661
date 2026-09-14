# Manual de Usuario

## 1. Introducción

El presente manual explica el funcionamiento de la aplicación web desarrollada para administrar discos virtuales y sistemas de archivos EXT2.

La aplicación permite ejecutar comandos para crear discos, administrar particiones, crear usuarios, administrar archivos y carpetas y generar diferentes reportes.

---

# 2. Interfaz Principal

La interfaz principal contiene dos áreas:

* **Área de entrada:** permite escribir los comandos que serán ejecutados.
* **Área de salida:** muestra las respuestas generadas por el servidor.

También se dispone de opciones para ejecutar comandos y cargar archivos de script.

> **Captura 1:** Interfaz principal de la aplicación.

---

# 3. Creación de un Disco

Para crear un disco virtual se utiliza el comando `MKDISK`.

Ejemplo:

```text
mkdisk -size=10 -unit=M -fit=FF -path=/home/usuario/Discos/Disco1.mia
```

Después de ejecutar el comando, el sistema mostrará un mensaje indicando que el disco fue creado.

> **Captura 2:** Creación de un disco virtual.

---

# 4. Creación de Particiones

Para crear una partición se utiliza `FDISK`.

Ejemplo:

```text
fdisk -size=5 -unit=M -path=/home/usuario/Discos/Disco1.mia -type=P -fit=FF -name=Part1
```

> **Captura 3:** Creación de una partición primaria.

---

# 5. Montaje de una Partición

Para montar una partición:

```text
mount -path=/home/usuario/Discos/Disco1.mia -name=Part1
```

El sistema asignará un identificador a la partición.

Para observar las particiones montadas:

```text
mounted
```

> **Captura 4:** Partición montada y su identificador.

---

# 6. Formateo de la Partición

Después de montar la partición puede formatearse utilizando `MKFS`.

Ejemplo:

```text
mkfs -id=611A -type=full
```

Esto genera las estructuras correspondientes al sistema de archivos EXT2.

> **Captura 5:** Formateo de una partición.

---

# 7. Inicio de Sesión

Para iniciar sesión:

```text
login -user=root -pass=123 -id=611A
```

El usuario `root` se crea automáticamente durante el formateo.

> **Captura 6:** Inicio de sesión como root.

---

# 8. Administración de Grupos

Para crear un grupo:

```text
mkgrp -name=usuarios
```

Para eliminarlo:

```text
rmgrp -name=usuarios
```

> **Captura 7:** Administración de grupos.

---

# 9. Administración de Usuarios

Para crear un usuario:

```text
mkusr -user=user1 -pass=1234 -grp=usuarios
```

Para cambiar su grupo:

```text
chgrp -user=user1 -grp=root
```

Para eliminarlo:

```text
rmusr -user=user1
```

El contenido de `users.txt` puede comprobarse utilizando:

```text
cat -file1=/users.txt
```

> **Captura 8:** Administración de usuarios.

---

# 10. Creación de Carpetas

Para crear una carpeta:

```text
mkdir -path=/home/documentos
```

Para crear también las carpetas padre inexistentes:

```text
mkdir -p -path=/home/usuario/documentos
```

> **Captura 9:** Creación de carpetas.

---

# 11. Creación de Archivos

Para crear un archivo vacío:

```text
mkfile -path=/home/documentos/archivo.txt
```

También puede indicarse su tamaño:

```text
mkfile -path=/home/documentos/archivo.txt -size=100
```

Para crear automáticamente las carpetas padre:

```text
mkfile -r -path=/home/usuario/documentos/archivo.txt -size=100
```

También es posible cargar el contenido desde un archivo existente mediante `-cont`.

> **Captura 10:** Creación de un archivo.

---

# 12. Lectura de Archivos

El comando `CAT` permite mostrar el contenido de los archivos.

Ejemplo:

```text
cat -file1=/users.txt
```

También pueden solicitarse varios archivos:

```text
cat -file1=/archivo1.txt -file2=/archivo2.txt
```

> **Captura 11:** Contenido mostrado mediante CAT.

---

# 13. Generación de Reportes

El comando `REP` permite visualizar diferentes estructuras del sistema.

Ejemplo:

```text
rep -name=tree -path=/home/usuario/Reportes/tree.jpg -id=611A
```

Los reportes disponibles son:

* `mbr`
* `disk`
* `inode`
* `block`
* `bm_inode`
* `bm_block`
* `tree`
* `sb`
* `file`
* `ls`

> **Captura 12:** Reporte TREE generado por la aplicación.

Se recomienda incluir también capturas de los reportes MBR, DISK y SuperBlock.

---

# 14. Carga de Scripts

La aplicación permite cargar archivos con extensión `.smia`.

El usuario deberá:

1. Seleccionar el archivo.
2. Cargar su contenido en el área de comandos.
3. Ejecutar los comandos.
4. Revisar las respuestas en el área de salida.

> **Captura 13:** Carga de un archivo `.smia`.

---

# 15. Cierre de Sesión

Para finalizar la sesión actual:

```text
logout
```

> **Captura 14:** Cierre de sesión.

---

# 16. Manejo de Errores

La aplicación muestra mensajes cuando una operación no puede realizarse correctamente.

Entre los errores que pueden presentarse se encuentran:

* Disco inexistente.
* Partición inexistente.
* Usuario inexistente.
* Contraseña incorrecta.
* Grupo inexistente.
* Archivo inexistente.
* Falta de sesión activa.
* Falta de permisos.
* Parámetros inválidos.

> **Captura 15:** Ejemplo de mensaje de error mostrado por la aplicación.

---

# 17. Flujo General de Uso

El flujo recomendado para utilizar el sistema es:

1. Crear un disco con `MKDISK`.
2. Crear una partición con `FDISK`.
3. Montar la partición con `MOUNT`.
4. Obtener su identificador con `MOUNTED`.
5. Formatear con `MKFS`.
6. Iniciar sesión con `LOGIN`.
7. Crear grupos y usuarios.
8. Crear carpetas y archivos.
9. Consultar archivos mediante `CAT`.
10. Generar reportes mediante `REP`.
11. Finalizar la sesión mediante `LOGOUT`.

---

# 18. Conclusiones

La aplicación proporciona una interfaz para interactuar con la simulación de un sistema de archivos EXT2 mediante comandos.

El usuario puede administrar discos, particiones, usuarios, grupos, archivos y carpetas, además de generar reportes que permiten visualizar las estructuras internas creadas durante la ejecución.
