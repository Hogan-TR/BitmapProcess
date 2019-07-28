object = bmpImg.o
all : $(object)
	cc -o all $(object) -lm
bmpImg.o : bmpImg.c
.PHONY : clean
clean : 
	rm -r all $(object)