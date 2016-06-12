objects = main.c memory.c machine1.c machine2.c machine3.c \
		L2cache.c Victim.c
edit: $(objects)
	    cc $(objects) -g -o cache
clean:
	    rm cache 
