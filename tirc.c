#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "something.h"
//#include <libnsqrand.h>
//Server settings!
#define serv "irc.freenode.net"
#define port 6667
//Size
#define SIZE 2048
int exit_flag=0;
//Thread init
pthread_t read_thread;
pthread_mutex_t mutex;
//pthread_t ping_thread;
int timecheck=0;
//File includes
#include "conf.c"
#include "IO.c"
#include "ChanCommands.c"
#include "logs.c"
#include "parse.c"
//Line/string based operations.
void zap_newline (char *str)
{
     char *p = strchr (str, '\n');
     if (p)
          *p = '0';
}
void zap_all(char *str)
{
    char *p = strchr ( str, '\n');
    if (p)
        *p = ' ';
}
  
int clearstr(char *array)
{
    int test=0;
    int a=strlen(array);
    while(test<a)
    {
        array[test]='\0';
        test++;
    }
    return 0;

}


int main()
{
    char buffer[512]; 
    char identify[512];
    //Initializing lnsqrand
    //Connecting to the server stuff, don't touch, ever.
    int sockfd,portno,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    portno=(port);
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if ( sockfd <0)
        error("ERROR opening socket");
    server=gethostbyname(serv);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port =htons(portno);
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
        printf("ERROR connecting");
    printf("Connected\n");
//Identifying yourself
    sprintf(buffer, "USER %s . . :%s\r\nNICK %s\r\n\0",nick, BOT_VERSION, nick);
    printf("Sending ident\n");
    OUT(sockfd,buffer);
//Threading
    pthread_mutex_init(&mutex, NULL);
    pthread_create (&read_thread, NULL, Recieve, (void *) sockfd);
//    pthread_create (&ping_thread, NULL, PINGING, (void *) sockfd);
    Send(sockfd);
    pthread_exit (NULL);
}
//What to run on immediate startup before doing anything else.
void mainOnjoin(int sockfd)
{
    char buffer[1024];
    char identify[1024];
    sleep(15);
    sprintf(identify,"PRIVMSG nickserv :identify %s\r\n\0",password);
    OUT(sockfd,identify);
    sprintf(buffer, "JOIN %s\r\n\0", defchan);
    OUT(sockfd, buffer);
}
