run: no_mutex mutex_inside mutex_outside csapp.h csapp.c
	./no_mutex
	./mutex_inside
	./mutex_outside

no_mutex:
	gcc no_mutex.c -o no_mutex csapp.c -lpthread

mutex_inside:
	gcc mutex_inside.c -o mutex_inside csapp.c -lpthread

mutex_outside:
	gcc mutex_outside.c -o mutex_outside csapp.c -lpthread


clean:
	rm no_mutex mutex_inside mutex_outside
