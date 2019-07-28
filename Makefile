object = io.o test.o
all : $(object)
	cc -o all $(object) -lm
io.o : cv.h
test.o : test.c io.c cv.h
.PHONY : clean
clean : 
	rm -r all $(object)  Get.bmp