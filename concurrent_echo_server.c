/*
 * concurrentechoserver.c
 * 
 * Copyright 2018 graf <g@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


//IMPORT
//**********************************************************************
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>   
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
//#include <pthread.h>
#include "common_utils.h"
//**********************************************************************

//DECLARATIONS
//**********************************************************************
const unsigned short int BUFFER_SIZE = 2048;
const unsigned short int MAXIMUM_NUMBER_OF_CLIENTS = 10;
const unsigned short int MAXIMUM_QUEUED = 8;
const unsigned long int PORT_NUMBER = 8888;
static unsigned short int SERVER_IS_AVAILABLE = 0;
static unsigned long int CONNECTION_COUNTER = 0;
static unsigned long int CONNECTION_NUMBER = 0;
//**********************************************************************

//MEMBERS
//**********************************************************************
void *connection_handler(void *socket_descriptor) {
	
	}
//**********************************************************************
 
int main(int argc , char *argv[]) {		
	//DECLARATIONS
	//******************************************************************	
	char *traffic_log = "concurrentechoserver.connection.log";
	char *message_log = "concurrentechoserver.message.log";
	char log_buffer[BUFFER_SIZE];
	pid_t spawned_process_id;
	socklen_t client_length;
    int listen_file_descriptor;
    int connection_file_descriptor;    
    int read_size; 
    struct sockaddr_in server_address , client_address; //32 bit IPV4 network byte order address 
    char *ip_string;  
    char client_message[BUFFER_SIZE];     
    //******************************************************************    
    
    //SET UP SOCKET
    //******************************************************************     
    listen_file_descriptor = socket(AF_INET , SOCK_STREAM , 0); // assign socket descriptor
    
    if (listen_file_descriptor == -1) {
        perror("Could not create socket. Calling exit(2)."); 
        exit(2); 
		}
    puts("Socket created");
     
    //assign sockaddr_in structure
    server_address.sin_family = AF_INET; // internet protocol
    server_address.sin_addr.s_addr = INADDR_ANY; // let kernel choose ip and port
    server_address.sin_port = htons( 8888 ); //convert to big endien order
     
    // bind assigns a local protocol address to a socket
    // for inet this is: an ip address and tcp port number
    if (bind(listen_file_descriptor,(struct sockaddr *) &server_address ,sizeof(server_address)) < 0) {   
        perror("bind failed. Error");
        return 1;
		}
		
	SERVER_IS_AVAILABLE = !SERVER_IS_AVAILABLE; // set flag to true
    puts("bind successful");     
    
    listen(listen_file_descriptor , MAXIMUM_QUEUED);
    //******************************************************************    
    //OUTER LISTENING LOOP
    //WARNING: THIS IS A WHILE(TRUE) LOOP THAT NEEDS TO BE FINISHED. ADD
    //A TOGGLE SUCH THAT A USER MAY RESTART THE SERVER AND OR EXIT PROGRAM.
    //******************************************************************
    printf("MAXIMUM_NUMBER_OF_CLIENTS = %d\n", MAXIMUM_NUMBER_OF_CLIENTS);	
    while (SERVER_IS_AVAILABLE) {	
		puts("Waiting for incoming connections...");	
		connection_file_descriptor = accept(listen_file_descriptor, (struct sockaddr *) &client_address, &client_length);	
		CONNECTION_COUNTER++;
		CONNECTION_NUMBER++;
		
		inet_ntop(AF_INET, &client_address->sin_addr, ip_string, sizeof ip_string); 

		
		//sprintf(log_buffer, "Connection number: %d\tip: %s", CONNECTION_NUMBER, *(client_address.sa_data));
		log_it(log_buffer, traffic_log);
		sprintf(log_buffer, "Connection count: %d", CONNECTION_COUNTER);
		log_it(log_buffer, traffic_log);			
		printf("Connection number: %d\n", CONNECTION_NUMBER);
		printf("Connection count: %d\n", CONNECTION_COUNTER);
		//char temp[32];
		
		if (connection_file_descriptor < 0) {
			perror("accept failed");
			return 1;
			}
				
		//handle connections by spawning another process
		if ((spawned_process_id = fork()) == 0) {
			printf("spawned process\n");
			close(listen_file_descriptor); // stop listen on spawned process
				
			//send/receive strings from the client
			while((read_size = recv(connection_file_descriptor , client_message , BUFFER_SIZE , 0)) > 0 ) {				
				//log clients messages
				sprintf(log_buffer, "%s", client_message);
				log_it(log_buffer, message_log);
				printf("Read: %s from connection_file_descriptor\n", client_message);
				write(connection_file_descriptor, client_message, BUFFER_SIZE);
				printf("Wrote: %s to connection_file_descriptor\n", client_message);
			
				// chance to make decision, 
				//strncpy(temp_buffer, client_message); // store incoming string into temp buffer			
			
				// clear buffer					
				for (int i = 0; i < sizeof(BUFFER_SIZE); i++) {
					client_message[i] = 0;
					}			
				}		
		
			if(read_size == 0) {
				puts("Client disconnected");
				CONNECTION_COUNTER--;
				fflush(stdout);			
				}
			else if(read_size == -1) {
				perror("recv failed");
				}				
			printf("closing spawned process\n");
			exit(0);
			}	
		close(connection_file_descriptor);						
		}		
	return 0;
	}

