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

    for (int i = 0; i < imgInput.rows; i++) {
        for (int j = 0;j < imgInput.cols; j++) {

            int index = i * imgInput.cols + j;

            float hKernel[] = {
                1.0f, 0.0f, -1.0f,
                2.0f, 0.0f, -2.0f,
                1.0f, 0.0f, -1.0f
            };

            float vKernel[] = {
                1.0f, 2.0f, 1.0f,
                0.0f, 0.0f, 0.0f,
                -1.0f, -2.0f, -1.0f
            };

            int hSum = 0;
            int vSum = 0;
            int rowCount = -1;
            int colCount = -1;
            size_t size = imgInput.cols * imgInput.rows;

            for (size_t i = 0; i < 9; i++)
            {
                if (i % 3 == 0 && i != 0)
                    rowCount++;
                
                int kIndex = index + rowCount * imgInput.cols + colCount;
                if (kIndex < 0 || kIndex >= size)
                    continue;
                
                hSum += inputGrey[kIndex] * hKernel[i];
                vSum += inputGrey[kIndex] * vKernel[i];

                colCount++;
                if (colCount == 1)
                    colCount = -1;
            }
        
            outGrey[index] = (unsigned char) std::abs( (hSum + vSum) / 2);
        }
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