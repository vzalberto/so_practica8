#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

int puerto = 7200;

void datagram_info(struct sockaddr_in * datagram_addr){
	printf("Datagrama recibido\n");
	printf("IP: %s\n", (char *)inet_ntoa(datagram_addr->sin_addr.s_addr));
	printf("Puerto: %d\n", ntohs(datagram_addr->sin_port));
	printf("\n");
}

int main(void){
	int num[2];
	int s, res, clilen;
	struct sockaddr_in server_addr, msg_to_client_addr;
	s = socket(AF_INET, SOCK_DGRAM, 0);

	printf("tamaño en bits de $puerto: %d\n", ((int) sizeof(puerto)) * 8);
	printf("$puerto: %x \t(antes de htons)\n", puerto);

	//bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(puerto);

	printf("$puerto: %x \t(despues de htons)\n", puerto);

	bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));
	clilen = sizeof(msg_to_client_addr);

	while(1){
		printf("Esperando cliente...\n");
		recvfrom(s, (char *) num, 2*sizeof(int), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		datagram_info(&msg_to_client_addr);
		res = num[0] + num[1];

		sendto(s, (char *)&res, sizeof(int), 0, (struct sockaddr *)&msg_to_client_addr, clilen);
		printf("Respuesta enviada\n");
	}
}