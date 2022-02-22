INCLUDE_DIRS = -I/usr/include/opencv4
LIB_DIRS = 
CC=g++
CC_c = gcc
CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video -lrt -lopencv_imgproc
LIBS_c = -lrt
HFILES= 
CFILES= capture.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.cpp=.o}
OBJS_c = ${CFILES:.c=.o}
all: cropvideo sobel_image canny_image simple_capture_Normal simple_capture_V4L2 canny_sobel

clean:
	-rm -f cropvideo sobel_image canny_image simple_capture_Normal simple_capture_V4L2

cropvideo: cropvideo.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) $@.cpp -o $@ `pkg-config --libs opencv4` $(LIBS)
sobel_image: sobel_image.cpp 
	$(CC) $(LDFLAGS) $(CFLAGS) $@.cpp -o $@ `pkg-config --libs opencv4` $(LIBS)
canny_image: canny_image.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) $@.cpp -o $@ `pkg-config --libs opencv4` $(LIBS)
simple_capture_Normal: capture.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) capture.cpp -o $@ `pkg-config --libs opencv4` $(LIBS)
canny_sobel: canny_sobel.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) canny_sobel.cpp -o $@ `pkg-config --libs opencv4` $(LIBS)
simple_capture_V4L2: capture.c
	$(CC_c) $(LDFLAGS) $(CFLAGS) -o $@ capture.c $(LIBS)
depend:

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -c $<

