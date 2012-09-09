#include <signal.h>

/* include serv09 */
#include	"unpthread.h"
#include	"pthread09.h"


void sigterm_handler( int name, siginfo_t *name2, void * name3 ){

    printf( "Exiting\n" );

}


void register_signal_handlers(){

    struct sigaction act;

    // ignore SIGPIPE
    act.sa_handler = SIG_IGN;
    sigemptyset( &act.sa_mask );
    act.sa_flags = 0;
    sigaction( SIGPIPE, &act, NULL );

    // set my handler for SIGTERM
    act.sa_handler = sigterm_handler;
    sigemptyset( &act.sa_mask );
    act.sa_flags = 0;
    sigaction( SIGTERM, &act, NULL );

}





int
main(int argc, char **argv)
{

    register_signal_handlers();

	int		i;
	void	sig_int(int), thread_make(int);

	if (argc == 3)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 4)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: serv09 [ <host> ] <port#> <#threads>");
    number_of_threads = atoi(argv[argc-1]);
    threads_array = Calloc( number_of_threads, sizeof(Thread) );

    for (i = 0; i < number_of_threads; i++)
		thread_make(i);			/* only main thread returns */

	Signal(SIGINT, sig_int);

	for ( ; ; )
		pause();	/* everything done by threads */
}
/* end serv09 */

void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

	pr_cpu_time();

    printf("Yo");

    for (i = 0; i < number_of_threads; i++)
        printf( "thread %d, %ld connections\n", i, threads_array[i].thread_count );

	exit(0);
}
