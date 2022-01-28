#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#include <chrono>
#include <string>
#include <iostream>


// uso / usage (linux)

// install libopencv-dev

// compilação, compile with:
// g++ -o process main.cpp -I /usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

// run / rodar:
// process filename

void blur_gauss(unsigned char * channel_input, unsigned char &p, int index, int colSize);
void sobel_edgeDetect(cv::Mat &imgInput, cv::Mat &imgOutput);
void threshold (cv::Mat &imgInput, cv::Mat &imgOut, int threVal);
void filter_contrast(cv::Mat &imgInput, cv::Mat &imgOut);

std::string selectedFilter;


int main(int argc, char const *argv[])
{
    if (argc < 3){
        printf("usage: process --type imgfilename\n");
        return EXIT_FAILURE;
    }

    std::string image_path = cv::samples::findFile(argv[argc - 1]);
    selectedFilter = argv[1];
    cv::Mat img_input = cv::imread(image_path, cv::IMREAD_REDUCED_COLOR_2);


    if(img_input.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    
    cv::Mat imgOutput;

    auto start = std::chrono::high_resolution_clock::now();

    if (selectedFilter.compare("--thre") == 0) {
        if (argc != 4) {
            std::cout << "Usage: ./process --thre {threVal} img.png" << std::endl;
            return 1;
        }
        threshold(img_input, imgOutput, std::stoi(argv[2]));

    }
        
    else if (selectedFilter.compare("--contrast") == 0) {
        filter_contrast(img_input, imgOutput);

    } 

    else if (selectedFilter.compare("--sobel") == 0) {
        sobel_edgeDetect(img_input, imgOutput);
    }

    else {
        printf("invalid filter param\nsupported params: --constrast, --thre --sobel \naborting...\n\n");
        return 1;
    }


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    
    printf("Tempo decorrido\n time passed: %ld ms \n", duration.count());

        
    cv::imshow("Img Output", imgOutput);
    char k;

    while (k != 'q') {
        k = cv::waitKey(0); // img_input for a keystroke in the window
    }

    return 0;
}

void filter_contrast (cv::Mat &imgInput, cv::Mat &imgOut) {

    cv::cvtColor(imgInput, imgInput, cv::COLOR_RGBA2RGB);

    cv::Mat inputChannels[3];
    cv::Mat outChannels[3];

    cv::split(imgInput, inputChannels);
    cv::split(imgInput, outChannels);

    uchar *inputBlue = inputChannels[0].data;
    uchar *inputGreen = inputChannels[1].data;
    uchar *inputRed = inputChannels[2].data;

    uchar *outBlue = outChannels[0].data;
    uchar *outGreen = outChannels[1].data;
    uchar *outRed = outChannels[2].data;

    for (int i = 0; i < imgInput.rows; i++)
        for (int j = 0;j < imgInput.cols; j++) {

            int index = i * imgInput.cols + j;

            if (inputRed[index] > 240) 
                outRed[index] *= 0.98;
            if (inputRed[index] < 30) 
                outRed[index] *= 2.356;

            if (inputGreen[index] > 240) 
                outGreen[index] *= 0.98;
            if (inputGreen[index] < 30) 
                outGreen[index] *= 2.356;

            if (inputBlue[index] > 240) 
                outBlue[index] *= 0.98;
            if (inputBlue[index] < 30) 
                outBlue[index] *= 2.356;

        }

    cv::merge(outChannels, sizeof(outChannels)/sizeof(*outChannels), imgOut);
}


void threshold (cv::Mat &imgInput, cv::Mat &imgOut, int threVal) {

    cv::cvtColor(imgInput, imgInput, cv::COLOR_RGBA2GRAY);

    cv::Mat inputChannels[1];
    cv::Mat outChannels[1];

    cv::split(imgInput, inputChannels);
    cv::split(imgInput, outChannels);

    uchar *inputC = inputChannels[0].data;
    uchar *outC = outChannels[0].data;
    

    for (int i = 0; i < imgInput.rows; i++)
        for (int j = 0;j < imgInput.cols; j++) {

            int index = i * imgInput.cols + j;

            if (inputC[index] > threVal) 
                outC[index] = 255;
            else
                outC[index] = 0;

        }

    cv::merge(outChannels, sizeof(outChannels)/sizeof(*outChannels), imgOut);
}


void sobel_edgeDetect(cv::Mat &imgInput, cv::Mat &imgOutput) {

    cv::cvtColor(imgInput, imgInput, cv::COLOR_RGBA2GRAY);

    cv::Mat inputChannels[1];
    cv::Mat outChannels[1];

    cv::split(imgInput, inputChannels);
    cv::split(imgInput, outChannels);

    uchar *inputGrey = inputChannels[0].data;

    uchar *outGrey = outChannels[0].data;

    int colSize = imgInput.cols;

    for (int i = 0; i < imgInput.rows; i++)
        for (int j = 0;j < imgInput.cols; j++) {

            int index = i * imgInput.cols + j;

            int v_kernel = (
                inputGrey[index] * 0.0 +
                        
                inputGrey[index - 1] * 2.0 +     // west
                inputGrey[index + 1] * -2.0 +     // east
                
                inputGrey[index - colSize] * 0.0 +    // north
                inputGrey[index + colSize] * 0.0 +    // south
                
                inputGrey[index - colSize - 1] * 1.0 +    // northwest
                inputGrey[index - colSize + 1] * -1.0 +    // northeast
                inputGrey[index + colSize - 1] * 1.0 +    // southwest
                inputGrey[index + colSize + 1] * -1.0 ) ;   // southeast

            int h_kernel = (
                inputGrey[index] * 0.0 +
                        
                inputGrey[index - 1] * 0.0 +     // west
                inputGrey[index + 1] * 0.0 +     // east
                
                inputGrey[index - colSize] * 2.0 +    // north
                inputGrey[index + colSize] * -2.0 +    // south
                
                inputGrey[index - colSize - 1] * 1.0 +    // northwest
                inputGrey[index - colSize + 1] * 1.0 +    // northeast
                inputGrey[index + colSize - 1] * -1.0 +    // southwest
                inputGrey[index + colSize + 1] * -1.0 ) ;   // southeast

            outGrey[index] = (unsigned char) std::abs( (h_kernel + v_kernel) / 2);
    }

    cv::merge(outChannels, sizeof(outChannels)/sizeof(*outChannels), imgOutput);
}

void blur_gauss(unsigned char * channel_input, unsigned char & c_pixel, int index, int colSize) {

            c_pixel = 
                (channel_input[index] * 0.5 +
                
                channel_input[index - 1] * 0.125 +     // west
                channel_input[index + 1] * 0.125 +     // east
                
                channel_input[index - colSize] * 0.125 +    // north
                channel_input[index + colSize] * 0.125 +    // south
                
                channel_input[index - colSize - 1] * 0.0 +    // northwest
                channel_input[index - colSize + 1] * 0.0 +    // northeast
                channel_input[index + colSize - 1] * 0.0 +    // southwest
                channel_input[index + colSize + 1] * 0.0 ) ;   // southeast
}
