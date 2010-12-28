
#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;


/**
 * Prints to the screen, a binary representation of the value passed.
 *
 * @return void
 */
void showBits( unsigned value ){

	const int SHIFT = 8 * sizeof( unsigned ) - 1;
	const unsigned MASK = 1 << SHIFT;

	for( int i = 1; i <= SHIFT + 1; i++ ){
		cout << ( value & MASK ? '1' : '0' );
		value <<= 1;
	    if ( i % 8 == 0 ){
			cout << ' ';
		}
	}
	cout << endl;

}



/**
 * Main Function
 *
 */
int main(){

    char ch = 'A';
    short s = ch;

    cout << s << endl;
 	showBits( s );	

    return 0;

}
