//Stefania Hurtado Gonzalez y Juan Jose Marin 
// Entrega 3 -- Sistemas operativos
// HILOS (CREAR, LISTAR, PAUSAR Y BORRAR CONTENEDORES DOCKER)

#include <stdio.h>	//libreria basica
#include <string.h>	//libreria basica
#include <sys/socket.h>	//contenedores
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>// escribe					


int main(int argc, char *argv[]) {
	int sock;
	struct  sockaddr_in server;
	char message[1000], server_reply[2000];
	
	//Create socket   aqui crea el contenedor 
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("No se pudo crear el contenedor");
	}
	puts("Contenedor creado");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(1299);

	//Connect to remote server aqui busca la conexion a internet 
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("Fallo la conexion. Error");
		return 1;
	}
	
	puts("Conectado\n");
	
	printf("Ingresa opcion: ");
	fgets(message, 1000, stdin);
	
	//Send some data  aqui manda la data y verifica que se manda o no 

	if (send(sock, message, strlen(message)+1, 0) < 0) {
		puts("Envio fallido");
		return 1;
	} else {
		puts("Envio ok");
	}
	
	
	close( sock );
	return 0;
}
