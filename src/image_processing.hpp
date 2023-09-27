#ifndef IMAGE_CLASS_H
#define IMAGE_CLASS_H

#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core/types.hpp>


class ImageProcessing 
{
    public:
        static void sobel(cv::Mat &imgInput, cv::Mat &imgOutput);
        static void filter_contrast(cv::Mat &imgInput, cv::Mat &imgOutput);
        static void threshold(cv::Mat &imgInput, cv::Mat &imgOutput, uint thresholdVal);
};


#endif