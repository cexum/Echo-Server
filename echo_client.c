/* Developer: Charles DeGrapharee Exum
 * Date: 2018.08.08
 * Contact: charlesexumdev@gmail.com
 * */

#include <stdio.h> 
#include <string.h>    
#include <sys/socket.h>    
#include <arpa/inet.h> 
#include <unistd.h>

const unsigned short int BUFFER_SIZE = 2048; // TODO: make this a macro
unsigned short int REMAIN_CONNECTED = 1;
//socket 
 
int main(int argc , char *argv[]) {
	
    int connection_file_descriptor;   
    unsigned short int temp_ui16; //temp 16-bit buffer
    struct sockaddr_in server_address; 
    char message[BUFFER_SIZE] , server_reply[BUFFER_SIZE]; // why is reply 2x?
	char* temp_byte; //temp 8-bit buffer	
	int i;
    size_t size = BUFFER_SIZE;   
    //Create socket
    connection_file_descriptor = socket(AF_INET , SOCK_STREAM , 0); //calls to sys/socket.h
    if (connection_file_descriptor == -1) {
        printf("Could not create socket"); // this condition most likely means a reference issue /w <sys/socket.h>
		}
    puts("Socket created");
     
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // local "home"
    server_address.sin_family = AF_INET; // protocol family? Specifically INET as opposed to something else
    server_address.sin_port = htons( 8888 ); // port #
 
	// error handle 
	
    if (connect(connection_file_descriptor , (struct sockaddr *)&server_address , sizeof(server_address)) < 0) {
        perror("connect failed. Error");
        return 1;
		}
    
    // tell the user connection happened
    puts("Connected");
     
    //keep communicating with server
	
    while(REMAIN_CONNECTED) {
		//puts ("Start of loop");
		// clear out buffers in case lingering data is present from previous iteration
		for (int i = 0; i < sizeof(BUFFER_SIZE); i++) {
			server_reply[i] = 0;
			message[i] = 0;
			}
				
        printf("Enter message : ");       
        fgets(message, sizeof(message), stdin); // read text input                      
        temp_byte = &message[0]; // read first byte of message
        temp_ui16 = *(temp_byte); // cast as ui16
        //puts (temp_byte);
        printf("%d\n", temp_ui16);
        
        //check for exit condition
        if (temp_ui16 == 48) { // make this better
			puts("you want to close this socket");
			REMAIN_CONNECTED = !REMAIN_CONNECTED;//exit condition
			}
        if( send(connection_file_descriptor , message , BUFFER_SIZE, 0) < 0) {
            puts("Send failed");
            //memset(message, 0, sizeof(message)); // clear buffer
            return 1;
			}
        
        printf("Sent server: %s\n", message);
        
        // if recieved string.length < 0, error condition break loop
        if( recv(connection_file_descriptor , server_reply , BUFFER_SIZE , 0) < 0) {
            puts("recv failed");
            break;
			}			
        
        printf("Server replied: %s\n", server_reply);
       
        //puts ("end of loop");
		}     
    close(connection_file_descriptor);
    puts("closed socket");
    return 0;
	}
