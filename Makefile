WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 
#GCC = gcc -g $(WARNING) $(ERROR) 

SRCS = a6.c
OBJS = $(SRCS:%.c=%.o)

# diff -w means do not care about space

a6: $(OBJS) 
	$(GCC) $(OBJS) -o a6

.c.o: 
	$(GCC) -c $*.c

run: a6
	./a6

clean: # remove all machine generated files
	rm -f a6 *.o output* *~