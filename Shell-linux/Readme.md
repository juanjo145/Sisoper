# Stefania Hurtado -- Juan Jose Marín  
# Periodo 2022-2 
# PROYECTO 1 -- Sistemas Operativos 
# SHELL -- LINUX 

## Copilacion y ejecucion
La siguiente guia es una guia de las diferentes funciones y variables de nuestro 
programa nuestro programa utiliza diferentes metodos que le permiten desarrollar
un shell a partir de instrucciones de c nuestro programa corre en ciclo leyendo
las diferentes instrucciones que el usuario digita y de acuerdo a estas instrucciones
se hacen acciones que pueden editar o observar diferentes partes de nuestro shell.

### Estas son las varibles de tope

LECT_MAX 100  lectura maxima hasta 100 caracteres por linea
ARG_MAX 60    60 argumentos
HISTORY_PATH ".history"  

### Definimos cuales son las funciones permitidas en el shell para asi saber que se puede conocer que se va a ejecutar
-cat (lista salida cat nombrearchivo.c)
 -ls (ver contenido)
- pwd (ruta de donde nos encontramos en el momento)
- cp (nombre del archivo que se va a copiar y la ruta donde se va a guardar la
 copia)
- mkdir (crear un nuevo directorio)
- rmdir(eliminar directorio que este vacio)
- rm eliminar archivo
- touch (crear nuevo archivo)
- find (buscar archivo dentro el directorio donde me encuentro)
- grep (buscar palabra en archivo txt, se mostrara donde esta ubicada la
 palabra)
- sudo (actualizaciones que requieran permiso)
- df (uso del espacio del sistema si ponemos df-m es el espacio en megabytes)
- du (cuando espacio ocupa un archivo en el sistem du nombre archivo (valor
 exact))
- head(primeras lineas de el archivo txt seleccionado)
-  > < (recibe funciones indicando archivos especificos)
 ls > out.txt
- sort (ordena lista de archivos)
### Las variables globales y sus tipos
int pEspera;
int ent, out;
int ofi;
int entradaOfi, salidaOfi;
int pipe_ind;
int entrada_Archivo, salida_Archivo;

## Lo siguiente es el apartado de funciones :

### Funcion ejecutar
Verifica si la instruccion que se va a llamar se encuentra en el comando

#### Funcion entrada del shell -- espera de comando para el usuario.
Esta funcion verifica lo ingresado y espera a que el usuario lo haga de la manera correcta para dar un correcto estado


#### Funcion condicioness
Esto lo que hace es verificar que los conectores logicos esten despues de nuestro comando ingresado por usuario.

### Funcion historial:
#### Agrega una nueva instruccion al historial
Utilizamos condiciones para hacer una lectura de historial optima y sin errores.


## Funcion Main
La funcion que copila todo nuestro programa hace el Control shell y llamado de funciones para copilar se realiza el llamado de cada una de las funciones.
utilizando metodos para que tenga sus verificaciones pertinentes a la hora de utilizar variables globales
y el llamado de funciones con variables propias del main.
