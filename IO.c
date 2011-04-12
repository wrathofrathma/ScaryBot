void chanMsg(char *msg,char *channel,int sockfd)
{
    char buffer[SIZE+SIZE];
    sprintf(buffer,"PRIVMSG %s :%s",channel,msg);
    OUT(sockfd,buffer);
}

void OUT(int sockfd,char *message)
{
    if(timecheck)
    {
        send(sockfd,message,strlen(message),0);
        timecheck=0;
    }
    send(sockfd,message,strlen(message),0);
}

int Send(int sockfd)
{
    char bufz[SIZE];
    char buf[SIZE];
    char channel[SIZE];
    strcpy(channel,defchan);
    int buffer[SIZE];
//    sprintf(bufz, "JOIN %s\r\n\0", defchan);
//    send(sockfd, bufz, strlen(bufz), 0);
//    JoinChan(defchan,sockfd);
    mainOnjoin(sockfd);
    int test=1;
    int n;
    while(test==1)
    {
        fgets(buf,SIZE,stdin);
        zap_all (buf);
        pthread_mutex_lock(&mutex);
        sprintf(buffer,"%s \r\n\0",buf);
        outParse(buffer,sockfd,channel);
//        chanMsg(buffer,defchan,sockfd);
        pthread_mutex_unlock(&mutex);
        if(n<0)
            error("ERROR writing to socket");
    }
    exit_flag=1;
    return 1;
}

void *Recieve(void *sock)
{
    char buf[SIZE];
    int test=1;
    int n;
    int sockfd = (int) sock;
    while(test==1)
    {
        if (exit_flag)
        {
             exit(0);
             break;
        }
        n=read(sockfd,buf,1024);
        if(n<0)
        {
            printf("ERROR reading from socket\n");
            exit(1);
        }
        else if (n > 0) {
            buf[n] = '\0';
            inParse(buf,sockfd);
            clearstr(buf);
//            printf("%s", buf);
        }
    }
    pthread_exit (NULL);
}

void PINGING(int sockfd)
{
    while(1)
    {
        sleep(1);
        printf("%d\n",timecheck);
        timecheck++;
    }
}
