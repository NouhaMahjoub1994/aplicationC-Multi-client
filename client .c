#include "stdlib.h"
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>
#include <gtk/gtk.h>

void reverse(char str[], int length) 
{ 
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
        char temp = *(str+start);
        *(str+start) = *(str+end); 
        *(str+end) = temp;
        start++; 
        end--; 
    } 
} 
 
 
 // Implementation of itoa() 
char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    char isNegative = 0; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = 1; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
  
    return str; 
} 
  
  
  
  GtkBuilder      *builder; 
    GtkWidget       *window;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void fermer()
{
    gtk_main_quit();
}

int main(int argc, char *argv[]){
   gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "client.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "principale"));
    gtk_builder_connect_signals(builder, NULL);
 
 //   g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}
void envoyer(){  
GtkWidget *port= GTK_WIDGET(gtk_builder_get_object(builder, "numPort"));
GtkWidget *adrServeur= GTK_WIDGET(gtk_builder_get_object(builder, "adrServer"));
GtkWidget *zoneAffichage= GTK_WIDGET(gtk_builder_get_object(builder, "zoneAffichage"));
GtkTextBuffer *buffer=gtk_text_view_get_buffer (GTK_TEXT_VIEW (zoneAffichage));
GtkWidget *fichier= GTK_WIDGET(gtk_builder_get_object(builder, "nomFichier"));
GtkTextIter end;
gtk_text_buffer_get_end_iter(buffer, &end);
     FILE *f;
     char c;
    int connected_socket, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer1[512];
 
    portno = atoi(gtk_entry_get_text(GTK_ENTRY(port)));
    connected_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connected_socket < 0) 
        printf("ERROR opening socket");
    server = gethostbyname(gtk_entry_get_text(GTK_ENTRY(adrServeur)));
    if (server == NULL) {
        printf("ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(connected_socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting\n");
        bzero(buffer1,512);
        
  printf("enter le nom du fichier. n'oubliez pas l'extension SVP!\n");
char filename[100];
  strcpy(filename,gtk_entry_get_text(GTK_ENTRY(fichier)));
  printf("fichier : #%s#\n",filename);
  f=fopen(filename,"r");

     do
      {
	c=fgetc(f);
	write(connected_socket,&c,1);
      }
	while(c!=EOF);
	printf("\nThe file was sent successfully\n");
    fclose(f);
//reception of results from server
int received_number_of_lines=0;
int return_status1=0;
return_status1 = read(connected_socket, &received_number_of_lines,sizeof(received_number_of_lines));
if (return_status1 > 0) {
      char dst[50];
   printf("Received number of lines = %d\n", ntohl(received_number_of_lines));
   gtk_text_buffer_insert(buffer, &end, "Received number of lines = ", -1);
   gtk_text_buffer_insert(buffer, &end, itoa(ntohl(received_number_of_lines),dst,10), -1);
   gtk_text_buffer_insert(buffer, &end, "\n", -1);
}
int received_number_of_words=0;
int return_status3=0;
return_status3 = read(connected_socket, &received_number_of_words,sizeof(received_number_of_words));
if (return_status3 > 0) {
    char dst[50];
   printf("Received number of words = %d\n", ntohl(received_number_of_words));
   gtk_text_buffer_insert(buffer, &end, "Received number of words = ", -1);
   gtk_text_buffer_insert(buffer, &end, itoa(ntohl(received_number_of_words),dst,10), -1);
   gtk_text_buffer_insert(buffer, &end, "\n", -1);
}
int received_number_of_chars=0;
int return_status2=0;
return_status2 = read(connected_socket, &received_number_of_chars,sizeof(received_number_of_chars));
if (return_status2 > 0) {
   char dst[50];
   printf("Received number of chars = %d\n", ntohl(received_number_of_chars));
   gtk_text_buffer_insert(buffer, &end, "Received number of chars = ", -1);
   gtk_text_buffer_insert(buffer, &end, itoa(ntohl(received_number_of_chars),dst,10), -1);
   gtk_text_buffer_insert(buffer, &end, "\n", -1);
}

 


    close(connected_socket);
    return;
}
