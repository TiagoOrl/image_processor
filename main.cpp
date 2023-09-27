#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/types.hpp>

#include <chrono>
#include <string>
#include <iostream>


#include "main.h"
#include "src/image_processing.hpp"

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
        ImageProcessing::threshold(img_input, imgOutput, std::stoi(argv[2]));

    }
        
    else if (selectedFilter.compare("--contrast") == 0) {
        ImageProcessing::filter_contrast(img_input, imgOutput);

    } 

    else if (selectedFilter.compare("--sobel") == 0) {
        ImageProcessing::sobel(img_input, imgOutput);
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