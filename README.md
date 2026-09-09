# Manual Técnico
## Proyecto: Administración de Discos — MIA 2S 2026
### Carné: 201318661

---

## Tabla de Contenidos

1. [Descripción General](#1-descripción-general)
2. [Tecnologías y Herramientas](#2-tecnologías-y-herramientas)
3. [Arquitectura del Sistema](#3-arquitectura-del-sistema)
4. [Estructura de Directorios](#4-estructura-de-directorios)
5. [Backend — Servidor C++](#5-backend--servidor-c)
6. [Frontend — Angular](#6-frontend--angular)
7. [Comunicación Frontend ↔ Backend](#7-comunicación-frontend--backend)
8. [Compilación y Ejecución](#8-compilación-y-ejecución)
9. [Dependencias y Requisitos](#9-dependencias-y-requisitos)

---

## 1. Descripción General

Este proyecto implementa un **simulador de administración de discos** que permite al usuario ingresar comandos de consola para realizar operaciones sobre discos virtuales, particiones, sistemas de archivos y usuarios.

El sistema se divide en dos componentes:

- **Backend:** Servidor HTTP en C++ que recibe comandos de texto, los analiza con un parser (Flex/Bison) y retorna el resultado.
- **Frontend:** Aplicación Angular que provee una interfaz tipo terminal para introducir comandos y visualizar resultados.

---

## 2. Tecnologías y Herramientas

| Herramienta     | Versión        | Rol                                     |
|-----------------|----------------|-----------------------------------------|
| C++             | C++17          | Lenguaje del backend                    |
| Flex (Lex)      | ≥ 2.6          | Generador del analizador léxico         |
| Bison (Yacc)    | ≥ 3.7.4        | Generador del analizador sintáctico     |
| cpp-httplib     | header-only    | Servidor HTTP embebido en C++           |
| Angular         | 22.1.x         | Framework del frontend                  |
| TypeScript      | ~6.0.2         | Lenguaje del frontend                   |
| Node.js / npm   | ≥ 20 / 11.19.0 | Entorno de ejecución frontend           |
| g++             | ≥ 11           | Compilador C++                          |

---

## 3. Arquitectura del Sistema

```
┌─────────────────────────────────────────────────────────┐
│                  USUARIO (Navegador)                    │
│                                                         │
│  ┌───────────────────────────────────────────────────┐  │
│  │        Frontend Angular  (localhost:4200)         │  │
│  │   Terminal visual — entrada y consola de salida   │  │
│  └──────────────────┬────────────────────────────────┘  │
│                     │  HTTP POST /comando                │
│  ┌──────────────────▼────────────────────────────────┐  │
│  │        Backend C++  (localhost:8080)               │  │
│  │  cpp-httplib → ejecutarParser() → Flex → Bison    │  │
│  └───────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
```

### Flujo de una petición

1. El usuario escribe un comando y presiona **Ejecutar**.
2. Angular hace `POST http://localhost:8080/comando` con el texto en el body.
3. El servidor C++ llama a `ejecutarParser(body)`.
4. Flex tokeniza el string → Bison valida la gramática y ejecuta acciones.
5. La salida del parser se captura desde `stderr` y se retorna como respuesta HTTP.
6. Angular renderiza el resultado en la consola visual.

---

## 4. Estructura de Directorios

```
MIA_2S2026_Practica_201318661/
│
├── api/                       ← Backend C++
│   ├── lexer.l                ← Definición del analizador léxico (Flex)
│   ├── grammar.y              ← Gramática del lenguaje (Bison)
│   ├── Scanner1.cpp / .hpp    ← Generados por Flex  (NO editar)
│   ├── Parser1.cpp  / .hpp    ← Generados por Bison (NO editar)
│   ├── parserService.hpp      ← Declaración de ejecutarParser()
│   ├── parserService.cpp      ← Implementación del servicio del parser
│   ├── api.cpp                ← Punto de entrada: servidor HTTP
│   ├── httplib.h              ← Librería cpp-httplib (header-only)
│   ├── parser                 ← Binario: solo el parser (compilado)
│   └── servidor               ← Binario: servidor completo (compilado)
│
├── frontend/
│   └── practica/              ← Aplicación Angular
│       ├── src/
│       │   ├── index.html     ← HTML raíz de Angular
│       │   ├── main.ts        ← Bootstrap de la aplicación
│       │   ├── styles.css     ← Estilos globales
│       │   └── app/
│       │       ├── app.ts        ← Componente principal (lógica)
│       │       ├── app.html      ← Template del componente
│       │       ├── app.css       ← Estilos del componente
│       │       ├── app.config.ts ← Configuración (HttpClient, Router)
│       │       └── app.routes.ts ← Definición de rutas
│       ├── package.json
│       ├── angular.json
│       └── tsconfig.json
│
└── docs/
    ├── manual_tecnico.md      ← Este documento
    └── manual_usuario.md      ← Manual de usuario
```

---

## 5. Backend — Servidor C++

### 5.1 Analizador Léxico (`lexer.l`)

El lexer está generado con Flex en modo **reentrant** (`%option reentrant`) y con soporte para Bison Bridge (`%option bison-bridge`), permitiendo múltiples instancias independientes del scanner.

#### Expresiones regulares definidas

| Nombre           | Patrón                    | Descripción                          |
|------------------|---------------------------|--------------------------------------|
| `INTEGER`        | `[0-9]+`                  | Números enteros positivos            |
| `PATH_VALUE`     | `[\/][^ \t\r\n]+`         | Rutas absolutas (inician con `/`)    |
| `ID_VALUE`       | `[a-zA-Z_][a-zA-Z0-9_]*` | Identificadores alfanuméricos        |
| `PASSWORD_VALUE` | `[a-zA-Z0-9]{1,10}`       | Contraseñas (máx. 10 caracteres)     |

#### Tokens generados

| Categoría       | Tokens                                                          |
|-----------------|-----------------------------------------------------------------|
| Comandos        | `MKDISK`, `RMDISK`, `FDISK`, `MOUNT`, `MKFS`, `MKUSR`, `RMUSR`, `MKFILE` |
| Parámetros      | `SIZE`, `FIT`, `UNIT`, `PATH`, `TYPE`, `NAME`, `ID`, `USER`, `PASSWORD`, `GROUP`, `R`, `COUNT` |
| Valores fit     | `BF`, `FF`, `WF`                                                |
| Unidades        | `B`, `K`, `M`                                                   |
| Tipos partición | `P`, `E`, `L`                                                   |
| Literales       | `FULL`                                                          |
| Operadores      | `EQUAL` (`=`), `EOL` (`\n`)                                     |
| Valores         | `INTEGER`, `PATH_VALUE`, `ID_VALUE`, `PASSWORD_VALUE`           |

> ⚠️ Los archivos `Scanner1.cpp` y `Scanner1.hpp` son generados automáticamente. No deben modificarse a mano.

---

### 5.2 Analizador Sintáctico (`grammar.y`)

Parser generado con Bison 3.7.4+ usando la API de C++ (`%language "C++"`) con variantes de valores semánticos (`%define api.value.type variant`).

#### Gramática (BNF resumido)

```
comandos → comandos comando | ε

comando  → mkdisk EOL | rmdisk EOL | fdisk EOL | mount EOL
         | mkfs EOL  | mkusr EOL  | rmusr EOL | mkfile EOL
         | EOL

mkdisk  → MKDISK ( -size=INT | -fit=[BF|FF|WF] | -unit=[K|M] | -path=PATH )+
rmdisk  → RMDISK -path=PATH
fdisk   → FDISK  ( -size=INT | -unit=[B|K|M] | -path=PATH | -type=[P|E|L] | -fit=... | -name=ID )+
mount   → MOUNT  ( -path=PATH | -name=ID )+
mkfs    → MKFS   ( -id=ID | -type=full )+
mkusr   → MKUSR  ( -user=ID | -pass=PASSWORD | -grp=ID )+
rmusr   → RMUSR  -user=ID
mkfile  → MKFILE ( -path=PATH | -r | -size=INT | -count )+
```

Los parámetros pueden aparecer **en cualquier orden** gracias a la estructura `params → params param | param`.

> ⚠️ Los archivos `Parser1.cpp` y `Parser1.hpp` son generados automáticamente. No deben modificarse a mano.

---

### 5.3 Servicio del Parser (`parserService.cpp`)

La función `ejecutarParser` encapsula el ciclo de vida completo del scanner y el parser:

```cpp
string ejecutarParser(const string& comando);
```

**Pasos internos:**

1. `yylex_init(&scanner)` — inicializa el scanner reentrant.
2. Agrega `\n` al final del string si no lo tiene (requerido para el token `EOL`).
3. `yy_scan_string(entrada.c_str(), scanner)` — crea un buffer en memoria.
4. Redirige `stderr` a un `ostringstream` para capturar los mensajes.
5. `calc::Parser parser(scanner); parser.parse()` — ejecuta el análisis.
6. Restaura `stderr` y libera recursos (`yy_delete_buffer`, `yylex_destroy`).
7. Retorna el string capturado como respuesta.

---

### 5.4 Servidor HTTP (`api.cpp`)

Se utiliza **cpp-httplib** (single-header, incluida como `httplib.h`) para el servidor HTTP sincrónico.

#### Configuración CORS

```cpp
res.set_header("Access-Control-Allow-Origin",  "http://localhost:4200");
res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
res.set_header("Access-Control-Allow-Headers", "Content-Type");
```

Las peticiones `OPTIONS` (preflight del navegador) se responden automáticamente con `200 OK`.

#### Endpoint

| Método | Ruta       | Body               | Respuesta          |
|--------|------------|--------------------|--------------------|
| POST   | `/comando` | Texto plano (cmd)  | Texto plano (resp) |

El servidor escucha en `0.0.0.0:8080`.

---

## 6. Frontend — Angular

### Componente principal (`app.ts`)

El componente `App` inyecta `HttpClient` en el constructor e implementa `ejecutarComando()`:

```typescript
ejecutarComando(): void {
  const input   = document.getElementById('comando') as HTMLInputElement;
  const consola = document.getElementById('consola') as HTMLDivElement;
  const comando = input.value.trim();

  if (comando === '') return;

  this.http.post('http://localhost:8080/comando', comando, { responseType: 'text' })
    .subscribe({
      next: (respuesta) => {
        consola.innerHTML  = `<div class="linea comando">> ${comando}</div>`;
        consola.innerHTML += `<div class="linea resultado">${respuesta}</div>`;
      },
      error: (error) => console.error('Error:', error)
    });

  input.value = '';
}
```

### Configuración de la aplicación (`app.config.ts`)

```typescript
export const appConfig: ApplicationConfig = {
  providers: [
    provideBrowserGlobalErrorListeners(),
    provideRouter(routes),
    provideHttpClient()    // Habilita HttpClient en toda la app
  ]
};
```

---

## 7. Comunicación Frontend ↔ Backend

```
Angular (4200)                          C++ Server (8080)
     │                                        │
     │  POST /comando                         │
     │  Body: "mkdisk -size=5 -path=/..."     │
     │ ──────────────────────────────────────▶│
     │                                        │ ejecutarParser(body)
     │                                        │ → Flex tokeniza
     │                                        │ → Bison valida
     │                                        │ → captura stderr
     │  200 OK                                │
     │  Body: "Comando mkdisk ejecutado."     │
     │ ◀──────────────────────────────────────│
     │                                        │
     │  Renderiza en la consola visual        │
```

---

## 8. Compilación y Ejecución

### Requisitos previos

```bash
bison --version    # ≥ 3.7.4
flex --version     # ≥ 2.6
g++ --version      # ≥ 11
node --version     # ≥ 20
npm --version      # ≥ 10
```

### Paso 1 — Generar el Scanner (Flex)

```bash
cd api/
flex lexer.l
# Genera: Scanner1.cpp, Scanner1.hpp
```

### Paso 2 — Generar el Parser (Bison)

```bash
bison grammar.y
# Genera: Parser1.cpp, Parser1.hpp
```

### Paso 3 — Compilar el servidor C++

```bash
g++ -std=c++17 -o servidor \
    api.cpp \
    parserService.cpp \
    Parser1.cpp \
    Scanner1.cpp \
    -lfl
```

### Paso 4 — Ejecutar el servidor

```bash
./servidor
# Salida: "Servidor iniciado en puerto 8080..."
```

### Paso 5 — Instalar dependencias del frontend

```bash
cd frontend/practica/
npm install
```

### Paso 6 — Ejecutar el frontend

```bash
npm start
# Disponible en: http://localhost:4200
```

> ⚠️ El servidor C++ **debe estar corriendo antes** de abrir el frontend.

---

## 9. Dependencias y Requisitos

### Backend (C++)

| Dependencia | Instalación                      |
|-------------|----------------------------------|
| `flex`      | `sudo apt install flex`          |
| `bison`     | `sudo apt install bison`         |
| `g++`       | `sudo apt install g++`           |
| `libfl`     | `sudo apt install libfl-dev`     |
| `httplib.h` | Incluida en el repositorio       |

### Frontend (Angular)

Instalación con `npm install`. Dependencias principales:

| Paquete              | Versión   |
|----------------------|-----------|
| `@angular/core`      | ^22.1.0   |
| `@angular/common`    | ^22.1.0   |
| `@angular/forms`     | ^22.1.0   |
| `rxjs`               | ~7.8.0    |
| `typescript`         | ~6.0.2    |
| `@angular/cli`       | ^22.1.3   |

---

*Manual Técnico — MIA 2S 2026 — Carné 201318661*
