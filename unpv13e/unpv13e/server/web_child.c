
#include <stdlib.h>

#include	"unp.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/tcp.h>

#include <assert.h>


#define	MAXN	16384		/* max # bytes client can request */

#define REQUEST_BUFFER_SIZE 8000000 //segfaults at 9M, but not 8M

static int x = 0;


void testme( char **what ){

    printf( "1: %x", what );
    printf( "1: %x", *what );

    *what = malloc( 10 );
    memset( *what, 65, 10 );
    memset( (*what)+5, 0, 1 );

}


void string_concat( char **destination, char* first, char* second ){

    size_t len1 = strlen(first);
    size_t len2 = strlen(second);
    size_t new_size =  len1 + len2 + 1;

    if( *destination == NULL ){
        *destination = malloc( new_size );
    }else{
        *destination = realloc( *destination, new_size );
    }

    memcpy( *destination, first, len1 );

    memcpy( *destination + len1, second, len2 );

    (*destination)[ len1 + len2 ] = '\0';

}



void respond_to_request( int sockfd, char *output, size_t output_size ){


    //printf( "Request #%d\n", ++x );

    /*
    char *local_var;
    testme( &local_var );
    printf( "LocalVar %s\n", local_var );


    printf( "Request2 #%d\n", ++x );

    */

    //output = "abcd";
    //output_size = 4;

    //convert the output size message body to a string
    char content_length[100];
    sprintf( content_length, "%d", (int)output_size );
    //printf( "buffer_size: %s\n", content_length );

    //create the response header
    char *response_header = NULL;
    char *response = NULL;
    string_concat( &response_header, "HTTP/1.1 200 OK\r\nContent-Length: ", content_length );
    string_concat( &response_header, response_header, "\r\nContent-Type: text/html\r\n\r\n" );

    //write both the response header and the message body to the socket
    string_concat( &response, response_header, output );
    size_t response_length = strlen( response );

    Writen( sockfd, response, response_length );

    free( response_header );
    free( response );

}




void web_child( int sockfd ){

	int			ntowrite;
	ssize_t		nread;
    char		line[ MAXLINE ], result[ MAXN ]; //, request_buffer[ REQUEST_BUFFER_SIZE ];

    char *request_buffer_tail;
    char *request_buffer;

    int first_character;

    request_buffer = malloc( 100 * sizeof(char*) );
    assert( request_buffer != NULL );


    memset( request_buffer, 0, 100 * sizeof(char*) );

    size_t buffer_size = 0;
    size_t new_buffer_size = 0;

	for ( ; ; ) {

        if( (nread = Readline(sockfd, line, MAXLINE)) == 0 ){
            break;		/* connection closed by other end */
        }

        new_buffer_size += nread;
        request_buffer = (char*) realloc( request_buffer, new_buffer_size * sizeof(char*) );
        assert( request_buffer != NULL );

        request_buffer_tail = request_buffer + buffer_size;
        memcpy( request_buffer_tail, line, nread );
        buffer_size = new_buffer_size;

        first_character = (int)*line;

        if( first_character == 13 ){
            respond_to_request( sockfd, request_buffer, buffer_size );
            memset( request_buffer, 0, buffer_size );
            buffer_size = 0;
            new_buffer_size = 0;
        }

	}

    free( request_buffer );


}




