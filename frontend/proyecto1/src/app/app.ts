import {
  Component,
  signal,
  computed,
  ViewChild,
  ElementRef,
  AfterViewChecked,
  OnInit
} from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { HttpClient, HttpHeaders } from '@angular/common/http';

export interface LineaConsola {
  tipo: 'cmd' | 'salida' | 'error' | 'info';
  texto: string;
  timestamp: string;
}

export interface ComandoCatalogo {
  nombre: string;
  categoria: string;
  sintaxis: string;
  descripcion: string;
  ejemplo: string;
  parametros: { nombre: string; requerido: boolean; desc: string }[];
}

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [CommonModule, FormsModule],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App implements OnInit, AfterViewChecked {
  @ViewChild('scrollConsola') private scrollConsolaRef?: ElementRef<HTMLDivElement>;
  @ViewChild('inputComando') private inputComandoRef?: ElementRef<HTMLInputElement>;

  // Estado reactivo
  readonly apiUrl = signal('http://localhost:8080/comando');
  readonly backendOnline = signal<boolean | null>(null);
  readonly modoActivo = signal<'terminal' | 'script' | 'catalogo' | 'reportes'>('terminal');

  readonly comandoActual = signal('');
  readonly ejecutando = signal(false);
  readonly autoScroll = signal(true);

  readonly historial = signal<string[]>([]);
  readonly indiceHistorial = signal<number>(-1);

  readonly scriptContenido = signal(
    `# ================================================\n` +
    `# Script de prueba - MIA 2S 2026\n` +
    `# ================================================\n` +
    `mkdisk -size=20 -unit=M -fit=BF -path=/home/disco1.dsk\n` +
    `fdisk -size=5 -unit=M -path=/home/disco1.dsk -type=P -fit=FF -name=Particion1\n` +
    `mount -path=/home/disco1.dsk -name=Particion1\n` +
    `mkfs -id=261disco1 -type=full\n`
  );
  readonly progresoScript = signal<{ actual: number; total: number } | null>(null);
  private cancelacionScript = false;

  readonly consola = signal<LineaConsola[]>([
    {
      tipo: 'info',
      texto: 'MIA Studio v1.0 — Simulador de Discos y Sistemas de Archivos (USAC MIA 2S 2026)\nListo para recibir comandos. Escribe un comando o ejecuta un script por lotes.',
      timestamp: this.obtenerHoraActual()
    }
  ]);

  // Catálogo completo de comandos de MIA
  readonly catalogoComandos: ComandoCatalogo[] = [
    {
      nombre: 'MKDISK',
      categoria: 'Discos',
      sintaxis: 'mkdisk -size=INT -fit=[BF|FF|WF] -unit=[K|M] -path=RUTA',
      descripcion: 'Crea un archivo binario que simula un disco duro con su Master Boot Record (MBR).',
      ejemplo: 'mkdisk -size=10 -unit=M -fit=FF -path=/home/disco.dsk',
      parametros: [
        { nombre: '-size', requerido: true, desc: 'Tamaño del disco (entero positivo)' },
        { nombre: '-fit', requerido: false, desc: 'Ajuste inicial: BF (Best), FF (First), WF (Worst)' },
        { nombre: '-unit', requerido: false, desc: 'Unidad: K (Kilobytes) o M (Megabytes, por defecto)' },
        { nombre: '-path', requerido: true, desc: 'Ruta absoluta donde se guardará el archivo' }
      ]
    },
    {
      nombre: 'RMDISK',
      categoria: 'Discos',
      sintaxis: 'rmdisk -path=RUTA',
      descripcion: 'Elimina el archivo binario del disco virtual en la ruta indicada.',
      ejemplo: 'rmdisk -path=/home/disco.dsk',
      parametros: [
        { nombre: '-path', requerido: true, desc: 'Ruta absoluta del archivo de disco a borrar' }
      ]
    },
    {
      nombre: 'FDISK',
      categoria: 'Particiones',
      sintaxis: 'fdisk -size=INT -unit=[B|K|M] -path=RUTA -type=[P|E|L] -fit=[BF|FF|WF] -name=ID',
      descripcion: 'Administra particiones primarias (P), extendidas (E) y lógicas (L) sobre un disco.',
      ejemplo: 'fdisk -size=5 -unit=M -path=/home/disco.dsk -type=P -name=Part1',
      parametros: [
        { nombre: '-size', requerido: true, desc: 'Tamaño de la partición (entero positivo)' },
        { nombre: '-path', requerido: true, desc: 'Ruta absoluta del disco donde se particiona' },
        { nombre: '-name', requerido: true, desc: 'Nombre identificador único de la partición' },
        { nombre: '-unit', requerido: false, desc: 'Unidad: B (Bytes), K (Kilo), M (Mega, defecto)' },
        { nombre: '-type', requerido: false, desc: 'Tipo: P (Primaria, defecto), E (Extendida), L (Lógica)' },
        { nombre: '-fit', requerido: false, desc: 'Ajuste: BF (Best), FF (First), WF (Worst)' }
      ]
    },
    {
      nombre: 'MOUNT',
      categoria: 'Particiones',
      sintaxis: 'mount -path=RUTA -name=NOMBRE',
      descripcion: 'Monta una partición del disco en memoria y genera su identificador alfanumérico.',
      ejemplo: 'mount -path=/home/disco.dsk -name=Part1',
      parametros: [
        { nombre: '-path', requerido: true, desc: 'Ruta del disco' },
        { nombre: '-name', requerido: true, desc: 'Nombre de la partición a montar' }
      ]
    },
    {
      nombre: 'MKFS',
      categoria: 'Sistema de Archivos',
      sintaxis: 'mkfs -id=ID -type=full',
      descripcion: 'Formatea la partición montada con el sistema de archivos EXT2/EXT3.',
      ejemplo: 'mkfs -id=261disco -type=full',
      parametros: [
        { nombre: '-id', requerido: true, desc: 'Identificador asignado al montar la partición' },
        { nombre: '-type', requerido: false, desc: 'Formateo full (por defecto)' }
      ]
    },
    {
      nombre: 'MKUSR',
      categoria: 'Usuarios',
      sintaxis: 'mkusr -user=USER -pass=PASSWORD -grp=GRUPO',
      descripcion: 'Crea un nuevo usuario en el sistema de archivos de la partición activa.',
      ejemplo: 'mkusr -user=usuario1 -pass=pass123 -grp=usuarios',
      parametros: [
        { nombre: '-user', requerido: true, desc: 'Nombre del usuario (máx. 10 caracteres)' },
        { nombre: '-pass', requerido: true, desc: 'Contraseña (máx. 10 caracteres)' },
        { nombre: '-grp', requerido: true, desc: 'Grupo al que pertenece' }
      ]
    },
    {
      nombre: 'RMUSR',
      categoria: 'Usuarios',
      sintaxis: 'rmusr -user=USER',
      descripcion: 'Elimina de forma lógica a un usuario registrado.',
      ejemplo: 'rmusr -user=usuario1',
      parametros: [
        { nombre: '-user', requerido: true, desc: 'Nombre del usuario a eliminar' }
      ]
    },
    {
      nombre: 'MKFILE',
      categoria: 'Archivos',
      sintaxis: 'mkfile -path=RUTA -r -size=INT -count',
      descripcion: 'Crea un archivo de texto en la ruta del sistema de archivos con contenido opcional.',
      ejemplo: 'mkfile -path=/documentos/nota.txt -r -size=64',
      parametros: [
        { nombre: '-path', requerido: true, desc: 'Ruta virtual del archivo' },
        { nombre: '-r', requerido: false, desc: 'Crear carpetas padre recursivamente si no existen' },
        { nombre: '-size', requerido: false, desc: 'Tamaño en bytes (rellena con números 0-9)' },
        { nombre: '-count', requerido: false, desc: 'Parámetro auxiliar' }
      ]
    }
  ];

  // Reportes predefinidos para la pestaña de visualización
  readonly reportesCatalogo = [
    { tipo: 'mbr', nombre: 'Reporte MBR', desc: 'Estructura general del disco y sus particiones primarias/extendidas.' },
    { tipo: 'disk', nombre: 'Reporte Disco', desc: 'Mapa gráfico del espacio utilizado y disponible en el disco.' },
    { tipo: 'inode', nombre: 'Reporte Inodos', desc: 'Tabla de inodos asignados en el sistema de archivos.' },
    { tipo: 'block', nombre: 'Reporte Bloques', desc: 'Detalle de bloques de carpetas, archivos y apuntadores.' },
    { tipo: 'tree', nombre: 'Reporte Árbol', desc: 'Árbol completo de directorios y archivos con inodos y bloques.' }
  ];

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    this.verificarConexion();
  }

  ngAfterViewChecked(): void {
    if (this.autoScroll()) {
      this.hacerScrollAbajo();
    }
  }

  // Comprueba si el servidor C++ está activo
  verificarConexion(): void {
    this.backendOnline.set(null);
    this.http.post(this.apiUrl(), '', {
      headers: new HttpHeaders({ 'Content-Type': 'text/plain' }),
      responseType: 'text'
    }).subscribe({
      next: () => this.backendOnline.set(true),
      error: (err) => {
        // Un error de red o de parseo vacío indica respuesta o no disponibilidad
        if (err.status === 200 || err.status === 400 || err.status === 0) {
          this.backendOnline.set(err.status !== 0);
        } else {
          this.backendOnline.set(true);
        }
      }
    });
  }

  // Ejecuta un comando único
  ejecutarComando(cmdManual?: string): void {
    const comando = (cmdManual ?? this.comandoActual()).trim();
    if (!comando || this.ejecutando()) return;

    // Agregar al historial
    this.historial.update((prev) => [comando, ...prev.filter((c) => c !== comando)]);
    this.indiceHistorial.set(-1);

    // Agregar comando a la salida
    this.agregarLineaConsola('cmd', `> ${comando}`);
    this.ejecutando.set(true);

    if (!cmdManual) {
      this.comandoActual.set('');
    }

    const headers = new HttpHeaders({ 'Content-Type': 'text/plain' });

    this.http.post(this.apiUrl(), comando, { headers, responseType: 'text' }).subscribe({
      next: (respuesta) => {
        this.backendOnline.set(true);
        this.agregarLineaConsola('salida', respuesta.trim() || 'Comando ejecutado sin salida.');
        this.ejecutando.set(false);
        this.enfocarInput();
      },
      error: (err) => {
        this.backendOnline.set(false);
        const mensaje = err.status === 0
          ? 'Error de conexión: No se pudo comunicar con el servidor C++ en ' + this.apiUrl() +
            '. Asegúrate de que ./servidor esté corriendo.'
          : `Error HTTP (${err.status}): ${err.message || 'Error desconocido'}`;
        this.agregarLineaConsola('error', mensaje);
        this.ejecutando.set(false);
        this.enfocarInput();
      }
    });
  }

  // Ejecuta un script completo línea por línea
  async ejecutarScript(): Promise<void> {
    const lineas = this.scriptContenido()
      .split('\n')
      .map((l) => l.trim())
      .filter((l) => l.length > 0 && !l.startsWith('#'));

    if (lineas.length === 0 || this.ejecutando()) return;

    this.ejecutando.set(true);
    this.cancelacionScript = false;
    this.progresoScript.set({ actual: 0, total: lineas.length });

    this.agregarLineaConsola(
      'info',
      `--- Iniciando ejecución de script (${lineas.length} comandos) ---`
    );

    for (let i = 0; i < lineas.length; i++) {
      if (this.cancelacionScript) {
        this.agregarLineaConsola('error', 'Ejecución de script cancelada por el usuario.');
        break;
      }

      const cmd = lineas[i];
      this.progresoScript.set({ actual: i + 1, total: lineas.length });
      this.agregarLineaConsola('cmd', `[${i + 1}/${lineas.length}] > ${cmd}`);

      try {
        const respuesta = await this.enviarComandoPromesa(cmd);
        this.backendOnline.set(true);
        this.agregarLineaConsola('salida', respuesta.trim() || 'OK');
      } catch (err: any) {
        this.backendOnline.set(false);
        this.agregarLineaConsola('error', `Error al ejecutar: ${err.message || 'Fallo de red'}`);
      }

      // Pequeña pausa entre comandos para permitir renderizado
      await new Promise((resolve) => setTimeout(resolve, 150));
    }

    this.progresoScript.set(null);
    this.ejecutando.set(false);
    this.agregarLineaConsola('info', '--- Fin de la ejecución del script ---');
  }

  detenerScript(): void {
    this.cancelacionScript = true;
  }

  private enviarComandoPromesa(comando: string): Promise<string> {
    return new Promise((resolve, reject) => {
      const headers = new HttpHeaders({ 'Content-Type': 'text/plain' });
      this.http.post(this.apiUrl(), comando, { headers, responseType: 'text' }).subscribe({
        next: (resp) => resolve(resp),
        error: (err) => reject(err)
      });
    });
  }

  // Carga de archivo .mia o .txt
  cargarArchivoScript(event: Event): void {
    const input = event.target as HTMLInputElement;
    if (!input.files || input.files.length === 0) return;

    const file = input.files[0];
    const reader = new FileReader();

    reader.onload = (e) => {
      const contenido = e.target?.result as string;
      if (contenido) {
        this.scriptContenido.set(contenido);
        this.agregarLineaConsola(
          'info',
          `Archivo "${file.name}" cargado exitosamente (${contenido.split('\n').length} líneas).`
        );
      }
    };

    reader.readAsText(file);
    input.value = '';
  }

  // Navegación de historial con flechas
  navegarHistorial(direccion: 'arriba' | 'abajo', event: Event): void {
    event.preventDefault();
    const h = this.historial();
    if (h.length === 0) return;

    let nuevoIndice = this.indiceHistorial();

    if (direccion === 'arriba') {
      if (nuevoIndice < h.length - 1) {
        nuevoIndice++;
      }
    } else {
      if (nuevoIndice > 0) {
        nuevoIndice--;
      } else {
        nuevoIndice = -1;
        this.comandoActual.set('');
        this.indiceHistorial.set(-1);
        return;
      }
    }

    this.indiceHistorial.set(nuevoIndice);
    this.comandoActual.set(h[nuevoIndice]);
  }

  insertarComando(plantilla: string): void {
    this.comandoActual.set(plantilla);
    this.modoActivo.set('terminal');
    this.enfocarInput();
  }

  limpiarConsola(): void {
    this.consola.set([]);
  }

  copiarConsola(): void {
    const texto = this.consola()
      .map((l) => `[${l.timestamp}] ${l.texto}`)
      .join('\n');
    navigator.clipboard.writeText(texto).then(() => {
      this.agregarLineaConsola('info', 'Consola copiada al portapapeles.');
    });
  }

  descargarLog(): void {
    const texto = this.consola()
      .map((l) => `[${l.timestamp}] ${l.texto}`)
      .join('\n');
    const blob = new Blob([texto], { type: 'text/plain;charset=utf-8' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `mia_log_${new Date().toISOString().slice(0, 10)}.txt`;
    a.click();
    URL.revokeObjectURL(url);
  }

  private agregarLineaConsola(tipo: 'cmd' | 'salida' | 'error' | 'info', texto: string): void {
    this.consola.update((prev) => [
      ...prev,
      { tipo, texto, timestamp: this.obtenerHoraActual() }
    ]);
  }

  private obtenerHoraActual(): string {
    const now = new Date();
    return now.toTimeString().split(' ')[0];
  }

  private hacerScrollAbajo(): void {
    if (this.scrollConsolaRef) {
      const el = this.scrollConsolaRef.nativeElement;
      el.scrollTop = el.scrollHeight;
    }
  }

  private enfocarInput(): void {
    setTimeout(() => {
      this.inputComandoRef?.nativeElement?.focus();
    }, 50);
  }
}
