
#include <iostream>
#include <sys/inotify.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;


void error( const char *message ){

    cout << "Error: " << message << endl;

}


class INotifyEvent : inotify_event{

    /*

    int wd;             // Watch Descriptor
    uint32_t mask;      // Bits describing event that occur
    uint32_t cookie;    // Cookie for related events (for rename())
    uint32_t len;       // Size of the 'name' field
    char name[];        // Optional null terminated filename

    */

    public:

        uint32_t getSize() const{

            return sizeof( struct inotify_event ) + len;

        }

        string getDescription() const{

            string result;

            if( this->mask & IN_ACCESS ) result += "IN_ACCESS ";
            if( this->mask & IN_ATTRIB ) result += "IN_ATTRIB ";
            if( this->mask & IN_CLOSE_WRITE ) result += "IN_CLOSE_WRITE ";
            if( this->mask & IN_CLOSE_NOWRITE ) result += "IN_CLOSE_NOWRITE ";
            if( this->mask & IN_CREATE ) result += "IN_CREATE ";
            if( this->mask & IN_DELETE ) result += "IN_DELETE ";
            if( this->mask & IN_DELETE_SELF ) result += "IN_DELETE_SELF ";
            if( this->mask & IN_MODIFY ) result += "IN_MODIFY ";
            if( this->mask & IN_MOVE_SELF ) result += "IN_MOVE_SELF ";
            if( this->mask & IN_MOVED_FROM ) result += "IN_MOVED_FROM ";
            if( this->mask & IN_MOVED_TO ) result += "IN_MOVED_TO ";
            if( this->mask & IN_OPEN ) result += "IN_OPEN ";
            //if( this->mask & IN_ALL_EVENTS ) result += "IN_ALL_EVENTS ";
            if( this->mask & IN_MOVE ) result += "IN_MOVE ";
            if( this->mask & IN_CLOSE ) result += "IN_CLOSE ";

            if( this->len > 0 ){
                result.append( this->name );
            }

            return result;

        }



};




int main( int argc, char** argv ){

    int inotify_instance, watch_descriptor;
    ssize_t read_result;

    INotifyEvent* current_event;

    char *event_read_buffer;
    char *buffer_iterator;

    const size_t READ_BUFFER_SIZE = 4096;

    event_read_buffer = (char*) malloc( READ_BUFFER_SIZE );


    if( (inotify_instance = inotify_init()) == -1 ){
        error( "inotify_init()" );
    }

    for( int x = 1; x < argc; x++ ){

        if( (watch_descriptor = inotify_add_watch( inotify_instance, argv[x], IN_ALL_EVENTS)) == -1 ){
            error( "inotify_add_watch()" );
        }

        cout << "Watching: " << argv[x] << endl;

    }


    bool continue_listening = true;

    while( continue_listening ){

        read_result = read( inotify_instance, event_read_buffer, READ_BUFFER_SIZE );

        if( read_result < 1 ){
            error( "read()" );
            continue_listening = false;
            break;
        }

        cout << "Read " << read_result << " bytes from inotify fd." << endl;

        buffer_iterator = event_read_buffer;

        do{

            cout << "Event: ";

            current_event = (INotifyEvent*) buffer_iterator;

            cout << current_event->getDescription() << endl;

            buffer_iterator += current_event->getSize();

        }while( buffer_iterator < event_read_buffer + read_result );


    }


    free( event_read_buffer );

    return 1;

}
