#include<stdio.h>
#include<string.h>
#include<string>
#include<iostream>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

using namespace std;
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
vector<int> input_vector;
string temp_str;
     int sockfd, newsockfd, portno, clilen, num_of_procs, proc_vals;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
getline(cin, temp_str);
num_of_procs = atoi(temp_str.c_str());
input_vector.push_back(num_of_procs);
while(getline(cin,temp_str))
{
proc_vals = atoi(temp_str.c_str());
input_vector.push_back(proc_vals);
input_vector.push_back(proc_vals);
}
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
int j =0;
     while(1)
     {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
        if (newsockfd < 0)
          error("ERROR on accept");
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
		  printf("Here is the message: %s\n",buffer);
input_vector.at(j);
        int x= input_vector.at(j);
        n = write(newsockfd, &x,sizeof(int));
j++;
        if (n < 0) error("ERROR writing to socket");
        close(newsockfd);
     }
     close(sockfd);
     return 0;
}
