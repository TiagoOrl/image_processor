GCC=g++

OPENCV_INCLUDEPATH=/usr/include/opencv4
OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

run: img_proc.o main.o
	$(GCC) -o run main.o img_proc.o $(OPENCV_LIBS) && rm *.o

main.o: main.cpp
	$(GCC) -c -o main.o main.cpp -I $(OPENCV_INCLUDEPATH)

img_proc.o: src/image_processing.cpp
	$(GCC) -c -o img_proc.o src/image_processing.cpp -I $(OPENCV_INCLUDEPATH)

cl:
	rm process *.o


