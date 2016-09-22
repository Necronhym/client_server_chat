/*
	Multithread online chat server

	Build with:
	g++ -lphtread c.cxx -o c

	Run as:
	./c Port
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
//Struct for passing passable indormation:
struct SPass
	{
		int iSocketFileDescription, iNewSocketFileDescription;
	};
//Recive information fron network:
void *Recive ( void* Recive )
	{
                struct SPass *SSend = (struct SPass*) Recive;
                char chBuffer[256];
                while( 1 ) 
                        {
                                memset( chBuffer , 0, sizeof chBuffer );
                                read( (*SSend).iNewSocketFileDescription, chBuffer, sizeof chBuffer );
                        	std::cout<<chBuffer;
			}
	}
//Recive information from network:
void *Send ( void* Send )
	{
		struct SPass *SSend = (struct SPass*) Send;
                char chBuffer[256];
                while( 1 ) 
                        {
                                memset( &chBuffer , 0, sizeof chBuffer );
			 	fgets( chBuffer, 255, stdin );
                                write( (*SSend).iNewSocketFileDescription, chBuffer, sizeof chBuffer );
                        }
	}
//Main fuction:
int main( int argc,char* argv[] )
	{
		struct sockaddr_in SServerAddress, SClientAddress;
		int iPortNumber = atoi( argv[1] );
		socklen_t socklenClient;
		pthread_t thread[2];
		SPass SPass;
		SPass.iSocketFileDescription = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		memset( ( char * ) &SServerAddress, 0, sizeof( SServerAddress ) );
		SServerAddress.sin_family = AF_INET;
		SServerAddress.sin_addr.s_addr = INADDR_ANY;
		SServerAddress.sin_port = htons( iPortNumber );
		bind( SPass.iSocketFileDescription, ( struct sockaddr *) &SServerAddress, sizeof( SServerAddress ) );
		listen( SPass.iSocketFileDescription, 5 );
		socklenClient = sizeof( SClientAddress );
		SPass.iNewSocketFileDescription = accept( SPass.iSocketFileDescription, (struct sockaddr *) &SClientAddress, &socklenClient );
		pthread_create( &thread[0], NULL, &Recive, ( void * )&SPass );
		pthread_create( &thread[1], NULL, &Send, ( void * )&SPass );
		pthread_join( thread[0], NULL );	
		pthread_join( thread[1], NULL );	
		close( SPass.iSocketFileDescription );
		close( SPass.iNewSocketFileDescription );
		return 0;
	}
