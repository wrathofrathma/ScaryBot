void logToFile(char *message, char *user, char *channame)
{
    printf("Logging: %s\n",message);
    FILE *pfile;
    time_t rawtime;
//    struct tm * timeinfo;
    time ( &rawtime );
    char *buftime=ctime(&rawtime);
    char buffer[4096];
    sprintf(buffer,"%s :CHANNEL %s :TIMEINFO %s!MESSAGE %s",user,channame,buftime,message);
    char *p=strchr(buffer,'\n');
    if(p)
        *p=' ';
//    char *q=strchr(buffer,'\n'); 
//    if(q)
//        *q='\0';
    if(pfile=fopen(logfile,"a+"))
    {
        fprintf(pfile,"%s",buffer);
    }
    fclose(pfile);
    printf("Out of logging\n");
}

int searchLogs(char *message,char *usr, char *chn, char *tim, char *msg)
{
    FILE *pfile;
    char command[256];
    char buffer[4096],buf[1024];
    char in[1024],out[1024];
    toLower(message,in);
    sprintf(command,"tac %s > %s",logfile,rlog);
    system(command);
    pfile=fopen(rlog,"r");
    if(pfile)
    {
        fgets(buffer,4095,pfile);
        while(!feof(pfile))
        {
            sscanf(buffer,"%s",buf);
            toLower(buf,out);
            if(strcmp(out,in)==0)
            {
                parseLogs(buffer, usr,chn,tim,msg);
                return 0;
            } 
            fgets(buffer,4095,pfile);
        }    
        return 1;
    }
    else
    {
        printf("ERRORS@searchlogs\n");
        return 1;
    }

}

int parseLogs(char *message,char *usr, char *chn, char *tim, char *msg)
{
    char user[256],chan[256],ttime[256],buffer[4095],empty[12024],buf[512],test[512];;
    sscanf(message,"%s %s %s",user,empty,chan);
    char *p=strchr(message,' ');
    if(p)
        p++;
    else
    {
        printf("Um wth o.o\n");
        return 1;
    }
    int x=1,y=0,n=0;
    while(x)
    {
        sscanf(p,"%s",empty);
        if(strcmp(empty,":TIMEINFO")==0)
            break;
        *p=strchr(p,' ');
         if(p)
         {
            p++;
         }
        else
            return 1;
    } 
    char *q=strchr(p,' ');
    q++;
    *p=strchr(q,' ');
    p++;
    *p=strchr(q,' ');
   p++;
   p++;
   p++;
   p++;
   p++;
   p++;
   p++;
   p++;
   p++;
   p++;

    while(x)
    {
        ttime[n]=p[n];
        n++;
        if(p[n]=='!')
            x=0;
    }
    char *d=strchr(p,'!');
    char *f=strchr(d,' ');
    f++;
    strcpy(buffer,f);
    strcpy(usr,user);
    strcpy(chn,chan);
    strcpy(tim,ttime);
    strcpy(msg,buffer);
}
