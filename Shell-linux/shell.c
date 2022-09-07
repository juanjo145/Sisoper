// Stefania Hurtado -- Juan Jose Marín  //
// Periodo 2022-2 //
// PROYECTO 1 -- Sistemas Operativos //
// SHELL -- LINUX //

#include <fcntl.h> // constantes de archivo
#include <stdio.h> // entrada y salida
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // busqueda, orden y manejo de archivos
#include <sys/wait.h>  // declaraciones para espera
#include <unistd.h>    // manejo de directorios y archivos

// Definimos constantes para mejor manejo de memoria

#define LECT_MAX 100 // lectura maxima hasta 100 caracteres por linea
#define ARG_MAX 60   // 60 argumentos
#define HISTORY_PATH ".history"  //

// funciones permitidas en el shell
// -cat (lista salida cat nombrearchivo.c)
// -ls (ver contenido)
//- pwd (ruta de donde nos encontramos en el momento)
//- cp (nombre del archivo que se va a copiar y la ruta donde se va a guardar la
// copia)
//- mkdir (crear un nuevo directorio)
//- rmdir(eliminar directorio que este vacio)
//- rm eliminar archivo
//- touch (crear nuevo archivo)
//- find (buscar archivo dentro el directorio donde me encuentro)
//- grep (buscar palabra en archivo txt, se mostrara donde esta ubicada la
// palabra)
//- sudo (actualizaciones que requieran permiso)
//- df (uso del espacio del sistema si ponemos df-m es el espacio en megabytes)
//- du (cuando espacio ocupa un archivo en el sistem du nombre archivo (valor
// exact))
//- head(primeras lineas de el archivo txt seleccionado)
//- > < (recibe funciones indicando archivos especificos)
// ls > out.txt
//- sort (ordena lista de archivos)
//-

int pEspera;
int ent, out;
int ofi;
int entradaOfi, salidaOfi;
int pipe_ind;
int entrada_Archivo, salida_Archivo;

// funcion ejecutar
void llamado(char **args) {
  if (execvp(args[0], args) < 0) // llamando al sistema operativo (execvp)
  {
    printf("No se encontro el comando \n");
    exit(1);
  }
}
// funcion entrada del shell -- espera
// de comando para el usuario.
void entrada(char *comando, char **args) {  
  int numero_argu = 0, arg_inicio = -1,
      tamano_coma = strlen(comando); // La variable tamano_coma nos indica el
                                     // tamaño de carecteres en el arreglo
  for (int i = 0; i < tamano_coma; i++) {
    if (comando[i] == ' ' || comando[i] == '\t' || comando[i] == '\n') {
      if (arg_inicio != -1) {
        args[numero_argu++] = &comando[arg_inicio];
        arg_inicio = -1;
      }
      comando[i] = '\0';
    } else {
      if (comando[i] == '&') { // lo que esta en comando[i] debe corresponder a &de comando usado
        pEspera = 0;
        i = tamano_coma;
      }
      if (arg_inicio == -1)
        arg_inicio = i;
    }
  }
  args[numero_argu] = NULL; // se hace para verificar que el metodo se ejecuta de manera correcta.
}

void comandoOfi(char *comando) {
  FILE *h = fopen(HISTORY_PATH, "a+");
  fprintf(h, "%s", comando);

  rewind(h); // rewind() muestra en pantalla comando copiado
}

// funcion condicioness shell verifica que los conectores logicos esten despues de nuestro comando ingresado por usuario
void condiciones(char **args) {
  for (int i = 0; args[i] != NULL; i++) {
    if (!strcmp(args[i], ">")) {
      if (args[i + 1] == NULL)

        printf("formato de comando invalido\n");

      else
        salida_Archivo = i + 1;
    }

    if (!strcmp(args[i], "|")) {
      if (args[i + 1] == NULL)
        printf("comando invalido despues de |\n");
      else
        pipe_ind = i;
    }

    if (!strcmp(args[i], "<")) {
      if (args[i + 1] == NULL)
        printf("formato de comando invalido\n");
      else
        entrada_Archivo = i + 1;
    }
  }
}
void historial(char **argu) {

  FILE *h = fopen(HISTORY_PATH, "r");
  if (h == NULL) {
    printf("Historial vacio \n");
  } else {
    if (argu[1] == NULL) {
      char c = fgetc(h);
      while (c != EOF) {        // EFO fin del archivo
        printf("%c", c);

        c = fgetc(h);
      }
    } else if (!strcmp(argu[1], "-c")) { // comparar argu[1], -c
      ofi = 0;
      remove(HISTORY_PATH); // borar(HISTORY PATH)

    } else {
      printf("[!] sintaxis invalida ");
    }
    fclose(h);
  }
}

// control shell y llamado de funciones
int main(void) {
  char comando[LECT_MAX];
  char ultimoComando[LECT_MAX];
  char comIngresado[LECT_MAX];
  char *args[ARG_MAX];
  char *arguF1[ARG_MAX], *arguF2[ARG_MAX];
  int ejecucion = 1, history = 0;
  int alerta;
  int pipech[2];

  while (ejecucion) {
    printf("StefaniaHG-JuanJM$ ");
    fflush(stdout); // limpia el buffer
    fgets(comando, LECT_MAX,
          stdin); // comando - almacena la cadena de caracteres , Maxline  - lee
                  // 80 caracteres maximo , stdin - Identifica la secuencia
                  // desde la que se leen los caracteres.
    //  fgets - me devuelve el comando.

    //--Trabajamos con boolean (su condicion cambia respeto a lo que valga
    // nuestra variable // 1-0 )
    salida_Archivo = entrada_Archivo = pipe_ind = -1;
    ofi = 1;
    pEspera = 1;
    alerta = 0;

    strcpy(comIngresado,
           comando); // (Libreria strcpy) = se el coomand a comandoo ingresado
                     // que seria nuestro destino  - comandoo: cadena que se va
                     // a copiar  - comIngresado: destino de la copia
    entrada(comIngresado, args);

    if (args[0] == NULL || !strcmp(args[0], "\0") || !strcmp(args[0], "\n")) {
      continue;
    }
    if (!strcmp(args[0], "exit")) {
      ejecucion = 0;
      continue;
    }
    // strcmp comparar cadenas
      if (!strcmp(args[0], "!!")) {// !! arroja el ultimo comando ingresado
        if (history) {
          printf("%s", ultimoComando);
          strcpy(comando, ultimoComando);
          strcpy(comIngresado, comando);
          entrada(comIngresado, args);
        } else {
          printf("No se encontraron comandos \n");
          continue;
        }
      }

    condiciones(args);

    //verifica el estado de archivos haciendo que los archivos se puedan abrir o cerrar segun su estados
    if (entrada_Archivo != -1) {
      ent = open(args[entrada_Archivo],
                 O_RDONLY); // abrir archivo solo en modo lectura (O_RDONLY)
      if (ent < 0) {
        printf("Fallo para abrir el archivo \'%s\'\n", args[entrada_Archivo]);
        alerta = 1;
      } else {
        entradaOfi = dup(0); 
        ent = dup(0);
        close(ent);
        args[entrada_Archivo - 1] = NULL;
      }
    }

    if (salida_Archivo != -1) {
      // O_WRONLY salida con escritura activada
      // O_TRUNC vacia todo el archivo antes de escribir
      // O_CREAT obtiene un error si el archivo ya existe
      // S_IRUSR permiso de lectura del usuario (propietario maquina)
      // S_IRGRP permiso de escritura en grupo
      // S_IWGRP permiso ejecucion de busqueda
      // S_IWUSR permiso de lectura @laptopStefaniaJuanJose

      out = open(args[salida_Archivo], O_WRONLY | O_TRUNC | O_CREAT,
                 S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
      if (out < 0) {
        printf("Fallo al abrir \'%s\'\n", args[salida_Archivo]);
        alerta = 1;
      } else {
        salidaOfi = dup(1);
        dup2(out, 1);
        close(out);
        args[salida_Archivo - 1] = NULL;
      }
    }

    if (pipe_ind != -1) { // pipe sea culminado pipe = 0
      int i;
      for ( i=0; i < pipe_ind; i++)
        arguF1[i] = args[i];
      arguF1[i] = NULL;
      i++;
      for ( i=0; args[i] != NULL; i++)
        // i= 1  args[1] -1 = 1 
        // i = 1 args[0] -1 = 0 -- culminado el pipe
        arguF2[i - pipe_ind - 1] = args[i];
      arguF2[i] = NULL;
    }
    // aleta =0 ejecucion =1  && and logico (alerta y ejecucion son diferentes)
    if (!alerta && ejecucion) {
      if (!strcmp(args[0], "history"))
        historial(args);
      else {
        if (!strcmp(args[0], "stop") || !strcmp(args[0], "continue")) {
          args[2] = args[1];
          //detener, continuar y matar proceso hijo 
          //compara el comando si ingresa stop, continue, kill
          args[1] = strcmp(args[0], "stop") ? "-SIGCONT" : "-SIGSTOP";
          args[0] = "kill";
          args[3] = NULL;
        }
        // creacion de nuevos procesos
        if (fork() == 0) {
          if (pipe_ind != -1) { // culmina proceso = 0
            pipe(pipech);
            if (fork() == 0) {
              salidaOfi = dup(1);//copia los procesos 
              dup2(pipech[1], 1);
              close(pipech[0]); // cierra el proceso 0
              llamado(arguF1); // llama al proceso 1 
            } else {
              wait(NULL);
              entradaOfi = dup(0); //copia proceso 0
              dup2(pipech[0], 0);// copia el pipe cuando es valido
              close(pipech[1]); // cierra el proceso[1]
              llamado(arguF2);//llama al proceso[2] culmina
            }
          } else
            llamado(args); // espera instruccion de usuario
        } else {
          if (pEspera)
            wait(NULL); // culmina comunicacion con proceso
        }
      }
      strcpy(ultimoComando, comando);
      if (ofi)
        comandoOfi(comando);
      history = 1;
    }
    dup2(salidaOfi, 1); //proceso 1 no culmina
    dup2(entradaOfi, 0); // proceso de entrada culminado
  }
  return 0;
}