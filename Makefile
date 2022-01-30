GCC=g++

OPENCV_INCLUDEPATH=/usr/include/opencv4
OPENCV_LIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

process: 
	$(GCC) -o process main.cpp -I $(OPENCV_INCLUDEPATH) $(OPENCV_LIBS)


