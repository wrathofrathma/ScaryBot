void botCommands(char *user,char *channame,char *msg,int sockfd)
{
    char buffer[SIZE],buf[SIZE],tmp[4096],p2[SIZE],tbuf[SIZE];
    char *p=strchr(msg,icommand);
//    if(p)
//        *p=' ';
    msg++;
    int ret;
    sscanf(msg,"%s %s",tbuf,p2);
    toLower(tbuf,buffer);
    if(strcmp(buffer,"seen")==0)
    {
        seen(p2,tmp);
        chanMsg(tmp,channame,sockfd);
    }
    else if(strcmp(buffer,"kick")==0)
    {
        kick(channame,sockfd,p2);        
    }
    else if(strcmp(buffer,"remember")==0)
    {
        remember(p2,msg);
    }
    else if(strcmp(buffer,"greet")==0)
    {
        Greet(p2,tmp);
        chanMsg(tmp,channame,sockfd);
    }
    else if(strcmp(buffer,"userstats")==0)
    {
        userStats(p2,tmp);
        chanMsg(tmp,channame,sockfd);
    }
    else if(strcmp(buffer,"forget")==0)
    {
        forget(p2);
    }
    else if(strcmp(buffer,"forgetq")==0)
    {
        forgetQ(msg);
    }
    else if(strcmp(buffer,"question")==0)
    {
        storeQ(msg);
    }
    else if(strcmp(buffer,"away")==0)
    {
        storeAway(msg);
    }
    else if(strcmp(buffer,"help")==0)
    {
        help(channame,sockfd,0);
    }
    else if(strcmp(buffer,"weather")==0)
    {
        weather(channame,sockfd,p2);
    }
    else
    {
        ret=remembered(buffer,tmp);
        if(!ret)
            chanMsg(tmp,channame,sockfd);
    }
}

int weather(char *channel, int sockfd, char *location)
{


}


void help(char *channel,int sockfd, int pm)
{
    if(pm==0)
    {
        chanMsg("###### ScaryBot v1.1 ######\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("Command indicator: ~\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("Seen <username> - reports the last sighting of a specified user with their last message\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("remember <word> <reply> - Set an automated response for when someone types ~<word>\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("greet <username> - Send a greeting to a fellow user\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("userstats <username> - Find out how many messages someone has said(SPAMMERS)\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("forget <word> - Forget a word set with remember command\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("question <question>? <response> - Set a question response\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("forgetq <question>? - forget a question\r\n\0",channel,sockfd);
        sleep(0.1);
        chanMsg("away <user> <message> - Set an away message for a user, it'll be played to them when they return\r\n\0",channel,sockfd);
      }
    else
    {
        privM("###### ScaryBot v1.1 ######\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("Command indicator: ~\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("Seen <username> - reports the last sighting of a specified user with their last message\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("remember <word> <reply> - Set an automated response for when someone types ~<word>\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("greet <username> - Send a greeting to a fellow user\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("userstats <username> - Find out how many messages someone has said(SPAMMERS)\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("forget <word> - Forget a word set with remember command\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("question <question>? <response> - Set a question response\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("forgetq <question>? - forget a question\r\n\0",channel,sockfd);
        sleep(0.1);
        privM("away <user> <message> - Set an away message for a user, it'll be played to them when they return\r\n\0",channel,sockfd);
    }
}
void privM(char *message,char *user,int sockfd)
{
    char buffer[SIZE];
    sprintf(buffer,"PRIVMSG %s :%s\r\n\0",user,message);
    OUT(sockfd,buffer);
}

int randomSpeach(char *msg, int sockfd, char *channel)
{
    FILE *pfile=fopen(random,"r");
    if(!pfile)
    {
        printf("Doesn't exist, random\n");
        return 0;
    }
    char buffer[SIZE],buf[SIZE],b1[SIZE];
    char hmm[SIZE],b2[SIZE];
    sscanf(msg,"%s",b2);
    toLower(b2,hmm);
    fgets(buffer,1020,pfile);
    while(!feof(pfile))
    {
        sscanf(buffer,"%s",b1);
        toLower(b1,buf);
        if(strcmp(buf,hmm)==0)
        {
            char *talk=strchr(buffer,' ');
            talk++;
            chanMsg(talk,channel,sockfd);
            return 1;
        }    
        clearstr(buf);
        clearstr(b1);
        clearstr(buffer);
        fgets(buffer,1020,pfile);
    }
}
void removeLine(char *file,char *file2, int line1, int line2)
{
    char buffer[SIZE];
    sprintf(buffer,"sed '%d,%dd' %s > %s && mv %s %s",line1,line2,file,file2,file2,file);
    system(buffer);
}

int forget(char *user)
{
    char buffer[SIZE],buf[SIZE],tmp[4096],command[SIZE];
    FILE *pfile;
    int x=0;
    pfile=fopen(remfile,"r");
    if(!pfile)
        return 1;
    fgets(tmp,SIZE,pfile);
    x++;
    while(!feof(pfile))
    {
        sscanf(tmp,"%s",buf);
        if(strcmp(buf,user)==0)
            break;
        fgets(tmp,SIZE,pfile);
        x++;
    }
    if(!strcmp(buf,user)==0)
        return 1;
    removeLine(remfile,tmpfile,x,x);
    return 0;
}


int userStats(char *user,char *outmsg)
{
    FILE *pfile;
    char buffer[2048],buf[SIZE];
    int x=0;
    pfile=fopen(logfile,"r");
    if(!pfile)
        return 1;
    fgets(buffer,SIZE,pfile);
    while(!feof(pfile))
    {
        sscanf(buffer,"%s",buf);
        if(strcmp(buf,user)==0)
        {
            x++;
        }
        fgets(buffer,SIZE,pfile);
    }
    sprintf(outmsg,"%s has said a total of %d messages\r\n\0",user,x);
    fclose(pfile);
    return x;
}

void Greet(char *user,char *outsrc)
{
    char buffer[SIZE];
    sprintf(outsrc,"Greetings %s!\r\n\0",user);
}

void remember(char *item, char *message)
{
    forget(item);
    char *litem[strlen(item)*4];
    toLower(item,litem);
    char *p=strchr(message,' ');
    p++;
    char *q=strchr(p,' ');
    q++;
    //Now q==message to log, and item is the keyword.
    FILE *pfile=fopen(remfile,"a+");
    printf("Remembered: %s as : %s\n",litem,q);
    fprintf(pfile,"%s %s",litem,q);
    fclose(pfile);
}

int remembered(char *item, char *outstr)
{
    char buffer[4096],sbuf[SIZE],litem[strlen(item)*4];
    FILE *pfile=fopen(remfile,"r");
//    char *q=strchr(item,'~');
//    if(q)
//    {
//        q++;
//    }
    toLower(item,litem);
    if(!pfile)
        return 1;  
    fgets(buffer,4095,pfile);
    while(!feof(pfile))
    {
        sscanf(buffer,"%s",sbuf);
        if(strcmp(sbuf,litem)==0)
        {
            break;
        }
        fgets(buffer,4095,pfile);
    }
    if(feof(pfile))
        return 1;
    char *p=strchr(buffer,' ');
    p++;
    sprintf(outstr,"%s\r\n\0",p);
    fclose(pfile);
    return 0;
}

void kick(char *channel, int sockfd, char *person)
{
    char buffer[SIZE];
    sprintf(buffer,"KICK %s %s\r\n\0",channel,person);
    OUT(sockfd,buffer);
}
void JoinChan(char *channel,int sockfd)
{
    char buffer[SIZE];
    sprintf(buffer,"JOIN %s\r\n\0",channel);
    OUT(sockfd,buffer);
}

void Kick(char *message,int sockfd, char *channel)
{
    char buffer[SIZE],username[SIZE],msg[SIZE],empty[SIZE];
    sscanf(message,"%s %s",empty,username);
    char *p=strchr(message,' ');
    char *q=strrchr(message,' ');
    if(p!=q)
    {
        p++;
        q=strchr(p,' ');
        q++;
        sprintf(buffer,"KICK %s %s :%s\r\n\0",channel,username,q);
    }
    else
    {
        sprintf(buffer,"KICK %s %s\r\n\0",channel,username);
    }
    OUT(sockfd,buffer);
}
void chanVoice(char *channame,char *channel)
{
    strcpy(channel,channame);
}

void giveMode(char *message, int sockfd, char *channel)
{
    char buffer[SIZE],username[SIZE],empty[SIZE],msg[SIZE],modes[SIZE];
    sscanf(message,"%s %s %s",empty,modes,username);
    sprintf(buffer,"MODE %s %s %s\r\n\0",channel,modes,username);
    OUT(sockfd,buffer);
}

void privMsg(char *message, int sockfd)
{
    char buffer[SIZE],username[SIZE],empty[SIZE],msg[SIZE];
    sscanf(message,"%s %s",empty,username);
    char *p=strchr(message,' ');
    if(p)
        p++;
    char *q=strchr(p,' ');
    if(q)
        q++;
    printf("%s\n",q);
    sprintf(buffer,"PRIVMSG %s :%s\r\n\0",username,q);
    printf("%s",buffer);
    OUT(sockfd,buffer);
}

void seen(char *person, char *outstr)
{
    char usr[SIZE],chn[SIZE],tim[SIZE],msg[4096];
    int i=searchLogs(person,usr,chn,tim,msg);
    if(i)
    {   
        sprintf(outstr,"I haven't seen %s, he must be lurking, or you need to learn to type\r\n\0",person);
    }
    else
    {
        sprintf(outstr,"%s was last seen in %s on %s : Saying : %s\r\n\0",person,chn,tim,msg);
    }
}

void joinScript(char *user, char *channel, int sockfd)
{
    char buffer[SIZE];
    char buf[SIZE];
    int x=opList(user);
    char *p=strchr(channel,':');
    if(p)
        *p=' ';
    if(x)
    {
        sprintf(buffer,"MODE %s %s %s\r\n\0",channel,"+o",user);
        OUT(sockfd,buffer);
    }
    int ch=checkAway(user);
    if(ch)
        relayAway(channel,sockfd,user);

}


int opList(char *user)
{
    int x;
    char buffer[SIZE],buf[SIZE];
    FILE *pfile=fopen(opfile,"r");
    if(!pfile)
    {
        printf("OPLIST not found\n");
        return 0;
    }
    fgets(buffer,SIZE,pfile);
    while(!feof(pfile) && x==0)
    {
        sscanf(buffer,"%s",buf);
        if(strcmp(buf,user)==0)
        {
            x=1;
        }
        fgets(buffer,SIZE,pfile);
    }
    fclose(pfile);
    return x;
}


int storeQ(char *msg)
{
    forgetQ(msg);
    FILE *pfile=fopen(qfile,"a+");
    if(!pfile)
    {
        printf("Error storing/opening qfile\n");
        return 1;
    }
    char buffer[SIZE],buf[SIZE],tmp[SIZE];
    int i=0;
    printf("Before *p in storeq: %s\n",msg);
    char *p=strchr(msg,' ');
    p++;
    while(p[i]!='?')
    {
        if(i>=strlen(p))
            return 1;
        tmp[i]=p[i];
        i++;
    }
    tmp[i]='\0';
    int x=0;
    while(x<=i)
    {
        p++;
        x++;
    }
    fprintf(pfile,"%s? %s",tmp,p);
    fclose(pfile);
}

int storeAway(char *msg)
{
    FILE *pfile=fopen(awayfile,"a+");
    char buffer[SIZE],buf[SIZE],tmp[SIZE];
    int i=0;
    char user[SIZE];
    char *p=strchr(msg,' ');
    p++;
    sscanf(p,"%s",user);
    char *q=strchr(msg,' ');
    q++;
    fprintf(pfile,"%s",q);
    fclose(pfile); 
    printf("Stored %s in away file\n",q);
}

int purgeAway(char *user)
{
    printf("Purging: %s from awayfile\n",user);
    FILE *pfile=fopen(awayfile,"a+");
    int i=1;
    char buffer[SIZE],buf[SIZE];
    char user2[SIZE];
    char tmpuser[SIZE];
    toLower(user,user2);
    fgets(buffer,SIZE-1,pfile);
    while(!feof(pfile))
    {
        sscanf(buffer,"%s",buf);
        toLower(buf,tmpuser);
        if(strcmp(user2,tmpuser)==0)
        {
            fclose(pfile);
            removeLine(awayfile,tmpfile,i,i);
            i=0;
            pfile=fopen(awayfile,"r");
        }
        i++;
        clearstr(buf);
        clearstr(buffer);
        clearstr(tmpuser);
        fgets(buffer,SIZE-1,pfile);
    }
    fclose(pfile);
}

int relayAway(char *channel,int sockfd,char *user)
{
    FILE *pfile=fopen(awayfile,"r");
    char buffer[4096],buf[SIZE],tmp[SIZE];
    fgets(buffer,4000,pfile);
    char user2[SIZE];
    char tmpuser[SIZE];
    toLower(user,user2);
    while(!feof(pfile))
    {
        sscanf(buffer,"%s",buf);
        toLower(buf,tmpuser);
        if(strcmp(tmpuser,user2)==0)
            chanMsg(buffer,channel,sockfd);                
        fgets(buffer,4000,pfile);
    }
    fclose(pfile);
    purgeAway(user);
}
int checkAway(char *user)
{
    printf("Checking Away: %s\n",user);
    FILE *pfile=fopen(awayfile,"r");
    char buffer[4096],buf[SIZE];
    fgets(buffer,4000,pfile);
    while(!feof(pfile))
    {
        sscanf(buffer,"%s",buf);
        if(strcmp(buf,user)==0)
            return 1;
        fgets(buffer,4000,pfile);
    }
    printf("Leaving away 0\n");
    return 0;
}

int questionParse(char *user, char *channel, char *array, int sockfd)
{
    printf("In question parse: %s\n",user);
    char buffer[SIZE];
    char narr[strlen(array)*4];
    int i=0;
    char tarr[SIZE];
    toLower(array,tarr);
    while(array[i]!='?'&&i<strlen(array))
    {
        narr[i]=tarr[i];
        i++;
    }
    int x=0,y=0;
    narr[i]='?'; // The question
    narr[i+1]='\0';
    printf("Narr: %s\n",narr);
    //Search for the question
    FILE *pfile=fopen(qfile,"r");
    char qbuf[4096],qb2[4096];
    char qb3[4095];
    fgets(qbuf,4095,pfile);
    while(!feof(pfile))
    {
        toLower(qbuf,qb2);
        while(qb2[x]!='?')
        {
            qb3[x]=qb2[x];
            x++;
        }
        qb3[x]='?';
        if(strcmp(qb3,narr)==0)
        {
            break;
        }
        clearstr(qbuf);
        clearstr(qb2);
        clearstr(qb3);
        printf("Comparing %s & %s\n",narr,qb3);
        fgets(qbuf,4095,pfile);
        x=0;
        y=0;
    }
    fclose(pfile);
    if(!strcmp(qb3,narr)==0)
    {
        printf("Couldn't find question\n");
        return 1;
    }
    char *g=qbuf;
    while(y<=x)
    {
        g++;
        y++;
    }
    g++;
    g++;
    chanMsg(g,channel,sockfd);
    printf("Out of message parse\n");
}


void forgetQ(char *array)
{
    char *d=strchr(array,' ');
    d++;
    char buffer[SIZE];
    char narr[strlen(d)*4];
    int i=0;
    char tarr[SIZE];
    toLower(d,tarr);
    while(d[i]!='?'&&i<strlen(d))
    {
        narr[i]=tarr[i];
        i++;
    }
    int x=0,y=0;
    narr[i]='?'; // The question
    narr[i+1]='\0';
    printf("Narr: %s\n",narr);
    //Search for the question
    FILE *pfile=fopen(qfile,"r");
    char qbuf[4096],qb2[4096];
    char qb3[4095];
    fgets(qbuf,4095,pfile);
    int f=1;
    while(!feof(pfile))
    {
        toLower(qbuf,qb2);
        while(qb2[x]!='?')
        {
            qb3[x]=qb2[x];
            x++;
        }
        qb3[x]='?';
        if(strcmp(qb3,narr)==0)
        {
            removeLine(qfile,tmpfile,f,f);
            break;
        }
        f++;
        clearstr(qbuf);
        clearstr(qb2);
        clearstr(qb3);
        fgets(qbuf,4095,pfile);
        x=0;
        y=0;
    }
    fclose(pfile);
}
