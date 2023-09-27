#include "image_processing.hpp"



void ImageProcessing::sobel(cv::Mat &imgInput, cv::Mat &imgOutput)
{
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

            int h_kernel = (
                inputGrey[index] * 0.0 +
                        
                inputGrey[index - 1] * 2.0 +     // west
                inputGrey[index + 1] * -2.0 +     // east
                
                inputGrey[index - colSize] * 0.0 +    // north
                inputGrey[index + colSize] * 0.0 +    // south
                
                inputGrey[index - colSize - 1] *  1.0 +    // northwest
                inputGrey[index - colSize + 1] * -1.0 +    // northeast
                inputGrey[index + colSize - 1] *  1.0 +    // southwest
                inputGrey[index + colSize + 1] * -1.0 ) ;   // southeast

            int v_kernel = (
                inputGrey[index] * 0.0 +
                        
                inputGrey[index - 1] * 0.0 +     // west
                inputGrey[index + 1] * 0.0 +     // east
                
                inputGrey[index - colSize] * -2.0 +    // north
                inputGrey[index + colSize] * 2.0 +    // south
                
                inputGrey[index - colSize - 1] * -1.0 +    // northwest
                inputGrey[index - colSize + 1] * -1.0 +    // northeast
                inputGrey[index + colSize - 1] *  1.0 +    // southwest
                inputGrey[index + colSize + 1] *  1.0 ) ;   // southeast

            outGrey[index] = (unsigned char) std::abs( (h_kernel + v_kernel) / 2);
    }

    cv::merge(outChannels, sizeof(outChannels)/sizeof(*outChannels), imgOutput);
}

void ImageProcessing::filter_contrast(cv::Mat &imgInput, cv::Mat &imgOutput)
{
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

    cv::merge(outChannels, sizeof(outChannels)/sizeof(*outChannels), imgOutput);   
}

void ImageProcessing::threshold(cv::Mat &imgInput, cv::Mat &imgOutput, uint thresholdVal)
{
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

            if (inputC[index] > thresholdVal) 
                outC[index] = 255;
            else
                outC[index] = 0;

        }

    cv::merge(outChannels, sizeof(outChannels)/sizeof(*outChannels), imgOutput);
}