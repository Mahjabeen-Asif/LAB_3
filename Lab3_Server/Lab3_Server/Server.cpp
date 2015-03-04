#include <winsock2.h>
//#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fstream>
#include<iostream>

#pragma comment(lib,"ws2_32.lib")

using namespace std;


int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    SOCKADDR_IN serv_addr;
    char sendBuff[1025];
    int numrv;

	WSADATA wsaData;
    int listenFd;

    if (WSAStartup(MAKEWORD(1,1), &wsaData) == SOCKET_ERROR) {
        printf ("Error initialising WSA.\n");
        return -1;
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(55555);

	cout<<listenfd<<endl;

    if(bind(listenfd, (LPSOCKADDR)&serv_addr,sizeof(serv_addr))==SOCKET_ERROR){
		cout<<SOCKET_ERROR<<endl;
		return false;
	}


    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }


    while(1)
    {
        connfd = accept(listenfd, (LPSOCKADDR)NULL ,NULL);

        /* Open the file that we wish to transfer */
        FILE *fp = fopen("C:/Users/mypc/AP/Lab3/src/test.txt","rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return 1;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            char * buff=new char[1024];
            int nread = fread(buff,1,500,fp);
            printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                send(connfd, buff, nread,0);
            }

            
            if (nread < 256)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }


        }

        shutdown(listenfd,SD_SEND);
       // sleep(1);
    }


    return 0;
}