#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/tcp.h>
#include<fcntl.h>

int main(int argc,char *argv[])
{
  struct sockaddr_in server;
  struct stat obj;
  int sock;
  int c=0;
  int choice,view,flag=1;
  char buf[100], command[5], filename[20], *f;
  int k, size, status,pass;
  int filehandle;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
    {
      printf("socket creation failed");
      exit(1);
    }
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[1]));
  server.sin_addr.s_addr = inet_addr("192.168.43.23");
  k = connect(sock,(struct sockaddr*)&server, sizeof(server));
  if(k == -1)
    {
      printf("Connect Error");
      exit(1);
    }
  int i = 1;
  int wc,y=0;
  system("vlc intro1.mp4 -f --fullscreen");
  system("clear");

   printf("\t\t Welcome to HYPE Broadcast TV.\n");
  printf("Enter your Position  \n\t 1.Manager \t 2.Customer \n\t Enter choice \n");
  scanf("%d",&view);
  if(view==1)
  {
    printf("Please Enter Password \n");
    scanf("%d",&pass);
    if(pass==12345)
    {
        printf("Login Successful !!");
    }
    else
    {
        printf("Don't prank !! We know you are a Customer \n");
        sleep(2);
        printf("Opening Customer group \n");
        sleep(1);
        view=2;
    }
  }
  while(1)
    {
        if(y!=0){

      printf("\nPress 1 to continue 0 to quit\n");
      scanf("%d",&wc);
      if(wc==0)
        choice=0;

      }
    if(wc!=0){
      //printf("\nEnter a choice:\n1- get\n2- put\n3- pwd\n4- ls\n5- cd\n6- LIVETV\n 7-Quit\n");
        if(view==2)
        {
                system("clear");
                printf("Please Select an option \n\t 1.List all the channels \n \t 2.Start LIVE TV: \n\t 0.SwitchOFF!!! \n");
            }
        else  {
        system("clear");
        printf("Please Select an option \n\t 1.List all the channels \n \t 2.Start LIVE TV: \n\t 3.Add a channel \n\t 0.SwitchOFF!!! \n");
        }
      scanf("%d", &choice);
      }
      switch(choice)
	{

	case 1:y=1;
	char buf1[100];

	  strcpy(buf1, "ls");
          send(sock, buf1, 100, 0);
	  recv(sock, &size, sizeof(int), 0);
          f = malloc(size);
          recv(sock, f, size, 0);
	  filehandle = creat("temp.txt", O_WRONLY);
	  write(filehandle, f, size, 0);
	  close(filehandle);
      system("clear");
          printf("The channel listings are: \n");
          if(c==0)
          {
          c=1;
          system("cat temp.txt>file1.txt");
            system("cat temp.txt");
          }
	  else system("cat file1.txt");

	  break;


	case 2:y=1;system("clear");
	  printf("Enter Channel name:");
	  scanf("%s", filename);
	  strcpy(buf, "get ");
	  strcat(buf, filename);

	  send(sock, buf, 100, 0);
	  recv(sock, &size, sizeof(int), 0);
//	  printf("==%d==",size);
	  if(!size)
	    {
	      printf("No such file on the remote directory\n\n");
	    break;
	    }
	  f = malloc(size);
	  recv(sock, f, size, 0);
	  while(1)
	    {
	      filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
	      if(filehandle == -1)
		{
		  sprintf(filename + strlen(filename), "%d", i);//needed only if same directory is used for both server and client
		}
	      else break;
	    }
	  write(filehandle, f, size, 0);
	  close(filehandle);
	  strcpy(buf, "cat ");
	  strcat(buf, filename);
	  printf("Connecting to the station...\n");
	  sleep(1);
	  printf("Finiding the required Channel...\n");
	  sleep(1);
	  printf("Starting the Channel...\n");
	  sleep(1);
	  printf("Here it goes...\n");
	  sleep(1);
	  char t1[100];
        strcpy(t1,"vlc ");
    strcat(t1,filename);
    strcat(t1," -f --fullscreen");
	  system(t1);
	  //system(buf);
	  break;



case 3:y=1;system("clear");
	  printf("Enter Channel to put on the server: ");
          scanf("%s", filename);
	  filehandle = open(filename, O_RDONLY);
          if(filehandle == -1)
            {
              printf("No such file on the local directory\n\n");
              break;
            }

          strcpy(buf, "put ");
	  strcat(buf, filename);
	  send(sock, buf, 100, 0);
	  stat(filename, &obj);
	  size = obj.st_size;
	  send(sock, &size, sizeof(int), 0);
	  sendfile(sock, filehandle, NULL, size);
	  recv(sock, &status, sizeof(int), 0);
	  if(status){
	    printf("Channel Successfully Added \n");
	    }
	  else
	    printf("Due to some internal conditions request was not serviced \n");
	  break;


	case 0:y=1;system("clear");
	  strcpy(buf, "quit");
          send(sock, buf, 100, 0);
          recv(sock, &status, 100, 0);
	  if(status)
	    {
        printf("Terminating the connection...\n");
        sleep(1);
	      printf("\t\t Have a Good day :) \n");
	      sleep(1);
	      exit(0);
	    }
	    printf("Server failed to close connection\n");


	}
    }
}
