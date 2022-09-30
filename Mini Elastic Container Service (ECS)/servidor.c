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

//FUNCIONES PROYECTO
// -Crear contenedor Docker
//- Listar Ccontenedor Docker
//- Detener Contenedor Docker
//- Borrar contenedores Docker

void *crear( void *message );
void *listas( );
void *pausa( void *message );
void *borrar( void *message );

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
           simb = strtok( NULL, " \n" );
        }
        
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
   char *name = (char *) message;
   pid_t pid = fork(); //creo el proceso pausa
   if( pid == 0 )
   {
      execlp("docker", "docker", "stop", name, NULL);
   }
   else{
      printf( "Contenedor detenido \n" );
   }
   pthread_exit( 0 );
}

void *crear( void *message ){
   char *name = (char *) message;
   pid_t pid = fork(); //creo el proceso crear
   if( pid == 0 )
   {
      execlp("docker", "docker", "run", "-di", "--name", name, "ubuntu:latest", "/bin/bash", NULL); //creamos el container docker 
   }
   else{
      printf( "Contenedor creado \n" );
   }
   pthread_exit( 0 );
}

void *borrar( void *message ){
   char *name = (char *) message;
   pid_t pid = fork(); //creo el proceso borrar
   if( pid == 0 )
   {
      execlp("docker", "docker", "rm", name, NULL);
   }
   else{
      printf( "Contenedor eliminado \n" );
   }
   pthread_exit( 0 );
}

void *listas( ){
   pid_t pid = fork(); //creo el proceso listar
   if( pid == 0 )
   {
      execlp("docker", "docker", "ps", NULL); //me lista los container para visualizar las imagenes, nombre, id, fecha de creacion y peso.
   }
   pthread_exit( 0 );
}