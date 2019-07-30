object = io.o fun.o test.o
all : $(object)
	cc -o all $(object) -lm
io.o : cv.h
fun.o : cv.h
test.o : test.c io.c fun.c cv.h
.PHONY : clean
clean : 
	rm -r all $(object)  Get.bmp