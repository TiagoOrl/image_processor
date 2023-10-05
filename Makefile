GCC=g++

OPENCV_INCLUDEPATH=/usr/include/opencv4
OPEN_CV_LINK = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
OMP = -fopenmp
FLAGS= $(OPEN_CV_LINK)

run: img_proc.o main.o
	$(GCC) -o run main.o img_proc.o $(FLAGS) && rm *.o

main.o: main.cpp
	$(GCC) -c -o main.o main.cpp -I $(OPENCV_INCLUDEPATH)

img_proc.o: src/image_processing.cpp
	$(GCC) -c -o img_proc.o src/image_processing.cpp -I $(OPENCV_INCLUDEPATH)

cl:
	rm process *.o


