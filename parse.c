void outParse(char *message, int sockfd, char *channel)
{
    if(message[0]=='/')
    {
        parseCommand(message,sockfd, channel);
    }
    else
        chanMsg(message,channel,sockfd);
}

void inParse(char *message, int sockfd)
{
    char buffer[SIZE];
    sscanf(message,"%s",buffer);
    if(strcmp(buffer,"PING")==0)
    {
        printf("Pinged by irc\n");
        timecheck=1;
        OUT(sockfd,"PONG\r\n\0");
    }
    else if(message[0]==':')
    {
        messParse(message,sockfd);        
    }
    else
        printf("%s",message);
}

void messParse(char *message, int sockfd)
{
    char buf[SIZE],buffer[SIZE],user[SIZE],channame[SIZE],mess[SIZE];
    message++;
    sscanf(message,"%s %s %s",buf,buf,channame);
    char *p=strchr(message,':');
    if(p)
        p++;
    char *a=strchr(message,'?');
    char *q=strchr(message,'!');
    if(q)
    {
        q=' ';
        exUser(message,user);
    }
    else
        sscanf(message,"%s",user);    
    if(strcmp(buf,"PRIVMSG")==0)
    {
        if(p[0]==icommand)
            botCommands(user,channame,p,sockfd);
        if(a)
            questionParse(user,channame,p,sockfd);
        if(q)
        {
            if(channame[0]=='#')         
                printf("%s(%s): %s",user,channame,p);
            else
            {
  //              char pbuf[SIZE];
//                sscanf(p,"%s",pbuf);
  //              if(strcmp(pbuf,"help"))
    //            {
      //              help(channame,sockfd,1);
        //        }
                printf("<%s> %s",user,p);
            }
        }
        logToFile(p,user,channame);
        int ch=checkAway(user);
        if(ch)
            relayAway(channame,sockfd,user);
        randomSpeach(p,sockfd,channame);
    }
    else if(strcmp(buf,"MODE")==0)
    {
        p=strchr(message,'+');
        printf("%s(%s) Sets mode: %s",user,channame,p);

    }
    else if(strcmp(buf,"ACTION")==0)
    {


    }
    else if(strcmp(buf,"JOIN")==0)
    {
        joinScript(user,channame,sockfd);
    }
    else if(strcmp(buf,"KICK")==0)
    {
        JoinChan(channame,sockfd);
    }
    else
        printf("%s",message);
}
void exUser(char *in, char *out)
{
    char buf[SIZE];
    int x=0;
    while(in[x]!='!')
    {
        buf[x]=in[x];
        x++;
    }
    buf[x]='\0'; 
    strcpy(out,buf);
}

void parseCommand(char *message,int sockfd, char *channel)
{
    char buffer[SIZE],buf[SIZE],empty[SIZE];
    sscanf(message,"%s",buffer);
    if(strcmp(buffer,"/join")==0)
    {
        sscanf(message,"%s %s",empty,buffer);
//        sprintf(buf,"JOIN %s\r\n\0",buffer);
//        send(sockfd,buf,strlen(buf),0);
        JoinChan(buffer,sockfd);
    }
    else if(strcmp(buffer,"/chan")==0)
    {
        sscanf(message, "%s %s",empty,buffer);
//        strcpy(channel,buffer);
        chanVoice(buffer,channel);
    }
    else if(strcmp(buffer,"/exit")==0)
    {
        exit(0);
    }
    else if(strcmp(buffer,"/kick")==0)
    {
        Kick(message,sockfd,channel);
    }
    else if(strcmp(buffer,"/mode")==0)
    {
        giveMode(message,sockfd,channel);
    }
    else if(strcmp(buffer,"/msg")==0)
    {
        privMsg(message,sockfd);
    }
}


void toLower(char *arr,char *out)
{
    int A=65;
    int Z=90;
    int a=97;
    int z=122;
    int l=strlen(arr);
    int i=0;
    while(i<l)
    {
        if(arr[i]>='A' && arr[i]<='Z')
        {
            out[i]=arr[i]+32;
        }
        else
            out[i]=arr[i];
        i++;
    }
    out[i]='\0';
    i++;
    out[i]='\0';
}
