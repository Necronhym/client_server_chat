/*
	Multithread online chat clinet

	Build with:
	g++ -lpthread c.cxx -o c

	Run as:
	./c ServerIP Port
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//Struct for storing passable inforamtion 
struct SPass
	{
		int iSocketFileDescription;
	};
//Send over network:
void *Send ( void* Send )
	{
		struct SPass *SSend = (struct SPass*) Send;
		char chBuffer[256];
		while( 1 )
			{
				memset( &chBuffer , 0, sizeof chBuffer );
				fgets( chBuffer, 255, stdin );
				write( (*SSend).iSocketFileDescription, chBuffer, sizeof chBuffer );
			}
	}
//Recive form network:
void *Recive ( void* Recive )
	{
		struct SPass *SSend = (struct SPass*) Recive;
		char chBuffer[256];
		while( 1 )
			{
				memset( chBuffer , 0, sizeof chBuffer );
				read( (*SSend).iSocketFileDescription, chBuffer, sizeof chBuffer );
				std::cout<<chBuffer;
			}
	}
//Main class takes arguments:
int main( int argc, char* argv[] )
	{
		struct sockaddr_in SSocketAddress;
		struct hostent *SHostServer;
		pthread_t thread[2];
		SPass SPass;
		SPass.iSocketFileDescription = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		SHostServer = gethostbyname( argv[1] );	
		memset( ( char* ) &SSocketAddress, 0,sizeof( &SSocketAddress ) );
		SSocketAddress.sin_family = AF_INET;
		bcopy( ( char* ) SHostServer->h_addr, ( char* )&SSocketAddress.sin_addr.s_addr, SHostServer -> h_length );
		SSocketAddress.sin_port = htons( atoi( argv[2] ) );
		connect( SPass.iSocketFileDescription, ( struct sockaddr * )&SSocketAddress, sizeof(SSocketAddress));
		pthread_create( &thread[0], NULL, &Send, ( void * )&SPass );
		pthread_create( &thread[1], NULL, &Recive, ( void * )&SPass );
		pthread_join( thread[0], NULL );
		pthread_join( thread[1], NULL );
		close( SPass.iSocketFileDescription );
		return 0;
	}
