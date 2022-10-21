//Stefania Hurtado Gonzalez y Juan Jose Marin 
// Entrega 3 -- Sistemas operativos
// HILOS (CREAR, LISTAR, PAUSAR Y BORRAR CONTENEDORES DOCKER)

#include<stdio.h> // libreria basica
#include<string.h>	//string
#include <stdlib.h> //Libreria basica
#include<sys/socket.h> //contenedores
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include <pthread.h> // manejo de Hilo
#include<stdbool.h> // manejo de booleanos
#define NOMBRE_FICHERO "Contenedores.txt"
FILE *fichero;
FILE *fichero2;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // inicializador de mutex (solo de 1 hilo)
//FUNCIONES PROYECTO
// -Crear contenedor Docker
//- Listar Ccontenedor Docker
//- Detener Contenedor Docker
//- Borrar contenedores Docker

void *crear( void *message );
void *listas( );
void *pausa( void *message );
void *borrar( void *message );
void *buscar(void *message);
char corriendo[20] = " corriendo";
char detenido[20] = " detenido";
bool  flag = false;
char *token;
char buffer[20];
char delimitador[] = " ";
char palabras[20];
char nombreContenedor[20];
char buscando[30];
bool delete = false;
int linea;
size_t len = 0;
int del = 0;


// creacion del file 
int main() {
	int containerServ, containerClient, c, read_size;
	struct sockaddr_in server, client;  //estructura cliente,y container administrador
	char mensajeCliente[2000]; //espacio de memoria reservado

//-------------------------------------------------------------------------

	// Crear contenedor

	containerServ = socket(AF_INET, SOCK_STREAM, 0); // AF_INET - direcciones para la comunicacion del contenedor (ipv4 ipv6)
                                                   // SOCK_STREAM - TCP confiable -conexion 
	if (containerServ == -1) {
		printf("No se pudo crear el contenedor");
	}
	puts("Contenedor creado"); //-printf
	
	//Estructura del contenedor
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(1299);
	

	if( bind(containerServ, (struct sockaddr *)&server, sizeof(server)) < 0) { //enlaza la direccion con nuestro puerto especificado
	
		perror("ERROR, La vinculacion falló."); // Imprime mensaje de error o invalidacion.
		return 1;
	}
	puts("Vinculación aceptada");//-printf
	
   
	listen(containerServ , 3);  // El contenedor espera la solicitud del usuario o cliente.
                              // Se define maximo de conexiones.
                              // longitud maxima establecida para realizar una conexion con el servidor.
                              // Cuando se tienen varios requerimientos, el cliente puede recibir una falla en la conexion del servidor, 
                              // sin embargo la ejecucion del programa sigue sin ninguna dificiltad (errores de ciclos por esperar solicitud del usuario).
                              // indicacion de estado de conexion (aceptada - fallida).
	
	//Conexion exitosa, esperando siguiente conexion o solicitud del usuario.
	puts("Esperando conexiones..."); 
	c = sizeof(struct sockaddr_in);
	
	while(1) {

		//conexion aceptada de usuario
		containerClient = accept(containerServ, (struct sockaddr *)&client, (socklen_t*)&c);
		if(containerClient < 0) {
			perror("La aceptacion falló");
			return 1;
		}
		puts("Conexion aceptada");
        memset(mensajeCliente, 0, 2000); //memoria reservada 
        recv(containerClient, mensajeCliente, 2000, 0);
        char *simb = strtok( mensajeCliente, " \n" );
        char cmd[ 3 ][ 200 ];
        int i = 0;
        while( simb != NULL )
        {
           strcpy( cmd[ i ], simb );
           i = i + 1;
           simb = strtok( NULL, " \n" ); // strtok (particion -cmd -name -image)
        }
      FILE *fichero = fopen(NOMBRE_FICHERO, "a");
      fclose(fichero);
      
        
// CREACION DE HILOS Y PROCESOS 
   
	pthread_t hilo;
    	    if( strcmp( cmd[ 0 ], "crear" ) == 0 )   // comando para usuario
        {
           pthread_create( &hilo, NULL, crear, ( void* ) cmd[ 1 ] );
           pthread_join( hilo, NULL );
        }
        else if( strcmp( cmd[ 0 ], "listar" ) == 0 ) // comando para usuario
        {
           pthread_create( &hilo, NULL, listas, NULL );
           pthread_join( hilo, NULL );
        }
        else if( strcmp( cmd[ 0 ], "detener" ) == 0 ) // comando para usuario
        {
           pthread_create( &hilo, NULL, pausa, ( void* ) cmd[ 1 ] );
           pthread_join( hilo, NULL );
        }
        else if( strcmp( cmd[ 0 ], "borre" ) == 0 ) // comando para usuario
        {
           pthread_create( &hilo, NULL, borrar, ( void* ) cmd[ 1 ] );
           pthread_join( hilo, NULL );
        }
    }
   
      


	exit( 0 );
}


void *pausa( void *message ){
   pthread_mutex_lock(&mutex1);
   char *name = (char *) message;
   FILE *fichero = fopen(NOMBRE_FICHERO, "r+");
   while (fgets(buffer, 20, fichero)){ //Recorrido de txt 
        // Aquí, justo ahora, tenemos ya la línea. Le vamos a remover el salto
        buffer[ strcspn( buffer, "\n" ) ] = 0;

        // La imprimimos, pero realmente podríamos hacer cualquier otra cosa
        strcpy(palabras,buffer);
        
        
       token = strtok(palabras, delimitador); //Me entrega solo los elementos sin salts de linea en token 

        if(token != ""){
         while(token != NULL){
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            if(strcmp(token,name)== 0){
               flag = true;
            
            }
            token = strtok(NULL, delimitador);
            printf("Token: %s\n", token);

         }

         }
        printf("La línea es: '%s'\n", buffer);
        printf("La línea contendor: '%s'\n", nombreContenedor);
    }
    if(flag == true){
      pthread_mutex_lock( &mutex1 );
      char * insert;
      //printf("name: %s\n", name);

      /*
      fputs(name,fichero);
      fputs(" ",fichero);
      fputs(detenido,fichero);
      */

      //fputs("\n", fichero);
      
      //fputs(insert, fichero);
      fclose(fichero);
      printf("nombre %s\n", name);
      size_t var2 = 20;
      char *buffer2 = NULL; // buffer2 creado para no conseguir errores del bffer utilizado para la bsuqeuda de del nombre del contendor
		fichero = fopen("Contenedores.txt", "r");
      linea = 0;
      del = 0;
		while (getline(&buffer2, &var2, fichero) ){
			token = strtok(buffer2, " ");
         printf("token %s", token);
			if(strcmp(token, name) == 0){
				break;
			}
			del++;
		}
		fclose(fichero);
      char * buffer3 = NULL;
		fichero = fopen("Contenedores.txt", "r");
		fichero2 = fopen("temp.txt", "w");

		while((fgets(buffer, 20, fichero)) != NULL){
			if(linea != del){ //Tefa no hagas el proceso adentro del while solo hacelo tipo bandera como lo hizimos en crear
            fputs(buffer, fichero2);
            //fputs("\n", fichero2);
			}
			linea++;
		}

      fputs(name,fichero2);
      fputs(" ",fichero2);
      fputs(detenido,fichero2);
      fputs("\n",fichero2); /// contendor detenido al final

		fclose(fichero2);
		fclose(fichero);

		remove("Contenedores.txt");
		rename("temp.txt", "Contenedores.txt");
		pthread_mutex_unlock( &mutex1 );
      
    }
    else{
      fclose(fichero);
      puts("no exite");
    }
      //fputs(fichero,"%s %s\n",name,corriendo);
      

   /*FILE* tmp2_file = fopen("archivo2.txt", "r+");
					FILE* new_file = fopen("containers.txt", "a+");
					if(!new_file || !tmp2_file)
						perror("Error fopen");

					char buffer2[100];
					char *ref2 = fgets(buffer2, 100, tmp2_file);
					
					while(ref2){
						char *token1 = strtok(buffer2, "\n");
						char *tmp2 = token1;
						char *token = strtok(buffer2, " "); 
							char *token2 = strtok(NULL, " ");
							fwrite(tmp2, 1, strlen(tmp2), new_file);
							printf("%s\n", token2);
							fwrite(" ", 1, 1, new_file);
							fwrite(token2, 1, 4, new_file);
							fwrite("\n", 1, 1, new_file);
							//printf("Done Writing!");
						
						ref2 = fgets(buffer2, 100, tmp2_file);

					}*/

   pid_t pid = fork(); //creo el proceso pausa

   if( pid == 0 ){
   execlp("docker", "docker", "stop", name, NULL);
         }
   else{
      printf( "Contenedor detenido \n" );
   }
         pthread_exit( 0 );
   }

void *crear( void *message ){
   //abrir txt -- recorre 
   char *name = (char *) message;
   pthread_mutex_lock(&mutex1);
   char palabras[20];
   char buffer[20];
   int i=0;
   char delimitador[] = " ";
   char nombreContenedor[20];
   FILE *fichero = fopen(NOMBRE_FICHERO, "a+"); // Modo lectura
    buscar(name);
    if(flag == true){
      fclose(fichero);
      puts("existe");
    }
    else{
      //fputs(fichero,"%s %s\n",name,corriendo);
      fputs(name,fichero);
      fputs(" ",fichero);
      fputs(corriendo,fichero);
      fputs("\n", fichero);
      fclose(fichero);

      pid_t pid = fork(); //creo el proceso crear
      if( pid == 0 ){
         execlp("docker", "docker", "run", "-di", "--name", name,"ubuntu:latest", "/bin/bash", NULL); //creamos el container docker 
         }
      else{
         printf( "Contenedor creado \n" );
      }
    }

   
   pthread_mutex_unlock(&mutex1);
   pthread_exit( 0 );
   return 0;
   }

void *borrar( void *message ){
   char *name = (char *) message;
   bool  flagDelete = false;
   pthread_mutex_lock(&mutex1);
   FILE *fichero = fopen(NOMBRE_FICHERO, "r+");
   while (fgets(buffer, 20, fichero)){ //Recorrido de txt 
        // Aquí, justo ahora, tenemos ya la línea. Le vamos a remover el salto
        buffer[ strcspn( buffer, "\n" ) ] = 0;

        // La imprimimos, pero realmente podríamos hacer cualquier otra cosa
        strcpy(palabras,buffer);
        
        
       token = strtok(palabras, delimitador); //Me entrega solo los elementos sin salts de linea en token 

        if(token != ""){
         while(token != NULL){
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            if(strcmp(token,name)== 0){
               flagDelete = true;

            }
            token = strtok(NULL, delimitador);
            printf("Token: %s\n", token);

         }

         }
        printf("La línea es: '%s'\n", buffer);
        printf("La línea contendor: '%s'\n", nombreContenedor);
    }
    printf("leyendo");
   fclose(fichero);
   pthread_mutex_unlock(&mutex1);
   size_t var = 20;
   del = 0;
   linea = 0;
   if(flagDelete==true){
      printf("nombre %s\n", name);
      pthread_mutex_lock( &mutex1 );
      char *buffer2 = NULL; // buffer2 creado para no conseguir errores del bffer utilizado para la bsuqeuda de del nombre del contendor
		fichero = fopen("Contenedores.txt", "r");
		while (getline(&buffer2, &var, fichero) != -1){
			token = strtok(buffer2, " ");
         printf("token %s", token);
			if(strcmp(token, name) == 0){
            printf("olawa\n");
				break;
			}
			del++;
		}
		fclose(fichero);

		fichero = fopen("Contenedores.txt", "r");
		fichero2 = fopen("temp.txt", "w");

		while((fgets(buffer, 20, fichero)) != NULL){
			if(linea != del){ //Tefa no hagas el proceso adentro del while solo hacelo tipo bandera como lo hizimos en crear
				fputs(buffer, fichero2);
			}
			linea++;
		}

		fclose(fichero2);
		fclose(fichero);

		remove("Contenedores.txt");
		rename("temp.txt", "Contenedores.txt");
		pthread_mutex_unlock( &mutex1 );
	}
   else{
      printf("no existe");
   }
   if(flagDelete == true){
      pid_t pid = fork(); //creo el proceso borrar
      if( pid == 0 ){
      execlp("docker", "docker", "rm", name, NULL);
      }
      else{
      printf( "Contenedor eliminado \n" );
      }
      pthread_exit( 0 );
   }
   
}

void *listas( ){
   //validacion para listar
   pthread_mutex_lock(&mutex1);
   FILE *fichero = fopen(NOMBRE_FICHERO,"r+");
   char bufer[20];
   printf("Aqui se imprimen los contenedores manejados");
   if(fichero != NULL){
      while (fgets(bufer,20,fichero)){
            strtok(bufer, "\n");
            printf("%s/n",bufer);
      }
      fclose(fichero);
      pid_t pid = fork(); //creo el proceso listar
      if( pid == 0 ){
      execlp("docker", "docker", "ps", NULL); //me lista los container para visualizar las imagenes, nombre, id, fecha de creacion y peso.
      }
      pthread_mutex_unlock(&mutex1);
      pthread_exit( 0 );
   }
}

void *buscar(void *message){
   char *name = (char *) message;
   FILE *fichero = fopen(NOMBRE_FICHERO, "r+");
   while (fgets(buffer, 20, fichero)){ //Recorrido de txt 
        // Aquí, justo ahora, tenemos ya la línea. Le vamos a remover el salto
        buffer[ strcspn( buffer, "\n" ) ] = 0;

        // La imprimimos, pero realmente podríamos hacer cualquier otra cosa
        strcpy(palabras,buffer);
        
        
       token = strtok(palabras, delimitador); //Me entrega solo los elementos sin salts de linea en token 

        if(token != ""){
         while(token != NULL){
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            if(strcmp(token,name)== 0){
               flag = true;
            }
            token = strtok(NULL, delimitador);
            printf("Token: %s\n", token);

         }

         }
        printf("La línea es: '%s'\n", buffer);
        printf("La línea contendor: '%s'\n", nombreContenedor);
    }
   fclose(fichero);
}