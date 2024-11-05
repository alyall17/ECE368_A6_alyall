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

test3: a6
	./a6 examples/3.po out3_1 out3_2 out3_3
	diff out3_1 examples/3.pr
	diff out3_2 examples/3.dim
	diff out3_3 examples/3.pck

clean: # remove all machine generated files
	rm -f a6 *.o out* *~