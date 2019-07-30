object = main.o io.o fun.o
all : $(object)
	cc -o all $(object) -lm
io.o : CV.h
fun.o : CV.h
main.o : main.c io.c fun.c CV.h
.PHONY : clean
clean : 
	rm -r all $(object)