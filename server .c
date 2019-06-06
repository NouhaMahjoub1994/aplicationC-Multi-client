
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{   
     pid_t pid_fils;
     FILE *fp;
     char c;
     int nbl=0;
     int nbm=0;
     int nbc=0;
     int listening_socket, connected_socket, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     listening_socket = socket(AF_INET, SOCK_STREAM, 0);
     if (listening_socket < 0) 
        error("ERROR opening socket\n");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(listening_socket, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding\n");
     listen(listening_socket,5);
while(1){
     clilen = sizeof(cli_addr);
     connected_socket = accept(listening_socket, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (connected_socket < 0) 
         { 
          error("ERROR on accept\n");
           return -1;
         }

     pid_fils=fork();
    if(pid_fils == -1)//erreur
      {
      printf("ERROR could not create process\n");
      return -1;
      }
    if (pid_fils == 0){ //fils
       //close listening_socket
      close(listening_socket);
      fp = fopen("myfilereceived.txt","w");            
            //copy file 
      while(read(connected_socket , &c , 1))
       	   {
      
		if (c==EOF)
			break; 
	   	else{ //count number of characters, words and lines  
      fprintf(fp, "%c" , c);
        if (c=='\n'){
            nbl++;
            nbm++;}
        else if(c==' '||c=='\t'){
	        nbm++;}
        else {
	        nbc++;
             }
	   }
         }
     	printf("The file was received successfully\n");

// Put your value
int converted_number_of_lines = htonl(nbl);
int converted_number_of_words = htonl(nbm);
int converted_number_of_chars = htonl(nbc);
//Write the number to the opened socket
write(connected_socket, &converted_number_of_lines, sizeof(converted_number_of_lines));
write(connected_socket, &converted_number_of_words, sizeof(converted_number_of_words));
write(connected_socket, &converted_number_of_chars, sizeof(converted_number_of_chars));
printf("nbc=%d, nbm=%d, nbl=%d \n",nbc,nbm,nbl);
    // Close the file 
    fclose(fp); 
    return 0;
    }// Close the fils
    else
     close(connected_socket);
  }
     return 0; 
}
