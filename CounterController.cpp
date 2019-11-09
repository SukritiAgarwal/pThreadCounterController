#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
using namespace std;

int value = 10;
char input = 'i';
int speed = 3;
bool status = true;
bool direction = true;

void *inputReader( void *param );
void *countDownThread( void *param );

int main( int argc, char ** argv ){
	pthread_t tid_reader, tid_countDownThread;
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_create( &tid_reader, &attr, inputReader, NULL ); // create the thread
	pthread_create( &tid_countDownThread, &attr, countDownThread, NULL ); // create the thread
	cout << "Main thread blocking until worker threads finish\n";
	pthread_join( tid_reader, NULL );
	pthread_join( tid_countDownThread, NULL );
	cout << "Main thread unblocked and outta here\n";
}

void *inputReader( void *param ){
	cout << "!!! Reader Thread Running!\n";
	while( input != 'a' ){
		cin >> input; // BLOCKING operation
		switch (input) {
		case '+':
			if (speed >= 0 && speed <= 2) 
				speed++;
			break;

		case '-':
			if (speed >= 1 && speed <= 4)
				speed--;
			break;

		case 's':
			if (status)
				status = false;
			else 
				status = true;
			cout << value << flush;
			break;

		case 'd':
			if (direction)
				direction = false;
			else 
				direction = true;
			break;
		default:
			break;
		}
	}
	cout << "inputReader thread exited\n";
	pthread_exit( 0 );
}

void *countDownThread( void *param ){
	cout << "!!! Count Down Thread Running!\n";
	struct timespec timing;
	timing.tv_nsec = 125000000L; 
	while( input != 'a' ){
		if (direction == true && status == true) 
			cout << value++ << flush;

		if (direction == false && status == true)
			cout << value-- << flush;

		nanosleep( &timing, NULL );
		cout << "\r                      \r";
		if (value == 100 && direction == true) 
			value = 0;

		if (value == -1 && direction == false) 
			value = 99;
		
		switch(speed){
		case 0:
			timing.tv_sec = 1;
			timing.tv_nsec = 0; 
			break;
		case 1:
			timing.tv_sec = 0; 
			timing.tv_nsec = 500000000L; 
			break;
		case 2:
			timing.tv_sec = 0;
			timing.tv_nsec = 250000000L; 
			break;
		case 3:
			timing.tv_sec = 0;
			timing.tv_nsec = 125000000L; 
			break;
		}
	}
	cout << "countDownThread thread exited\n";
	pthread_exit( 0 );
}