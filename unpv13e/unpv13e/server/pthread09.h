typedef struct {
  pthread_t		thread_tid;		/* thread ID */
  long			thread_count;	/* #connections handled */
} Thread;
Thread	*threads_array;		/* array of Thread structures; calloc'ed */

int				listenfd, number_of_threads;
socklen_t		addrlen;
