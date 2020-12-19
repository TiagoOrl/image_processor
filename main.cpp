#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#include <chrono>

#include <iostream>


// uso / usage (linux)

// compilação, comile with:
// g++ -o process main.cpp -I /usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

// run / rodar:
// process filename

void blur_gauss(unsigned char * channel_input, unsigned char &p, int index, int colSize);
void sobel_edgeDetect(unsigned char * channel_input, unsigned char & p, int index, int colSize);
void threshold(unsigned char & p);
void filter_contrast(unsigned char & p);



int main(int argc, char const *argv[])
{
    if (argc != 2){
        printf("usage: process imgfilename");
        return EXIT_FAILURE;
    }

    std::string image_path = cv::samples::findFile(argv[1]);
    cv::Mat img_input = cv::imread(image_path, cv::IMREAD_REDUCED_COLOR_4);


    if(img_input.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }

    cv::cvtColor(img_input, img_input, cv::COLOR_RGBA2RGB);

    cv::Mat rgb[3];
    cv::Mat new_rgb[3];

    // create copies of each 3 channel into a Mat[3] array
    cv::split(img_input, rgb);
    cv::split(img_input, new_rgb);

    // then we get the pointer ref from each channel from the Mat[3] into a uchar * pointer/array
    uchar *c_blue = rgb[0].data;
    uchar *c_green = rgb[1].data;
    uchar *c_red = rgb[2].data;

    // same as before, but in new copies to modify it and then create a new cv::Mat object
    uchar *c_newBlue = new_rgb[0].data;
    uchar *c_newGreen = new_rgb[1].data;
    uchar *c_newRed = new_rgb[2].data;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < img_input.rows; i++)
        for (int j = 0;j < img_input.cols; j++) {

            int index = i * img_input.cols + j;

            // here is where we process each channel, uncomment/comment then to change the filter you want to use
            // to apply a filter to an image, you have to apply the filter to each of the 3 channel image

            // threshold(c_red[index]);
            // threshold(c_green[index]);
            // threshold(c_blue[index]);

            sobel_edgeDetect(c_red, c_newRed[index], index, img_input.cols);
            sobel_edgeDetect(c_green, c_newGreen[index], index, img_input.cols);
            sobel_edgeDetect(c_blue, c_newBlue[index], index, img_input.cols);

            // filter_contrast(c_blue[index]);
            // filter_contrast(c_red[index]);
            // filter_contrast(c_green[index]);

        }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    
    printf("Tempo decorrido/ time passed: %ld ms \n", duration.count());
    
    cv::Mat output_img;
    cv::merge(new_rgb, 3, output_img);
        
    cv::imshow("Img Output::", output_img);


    char k;

    while (k != 'q'){
        k = cv::waitKey(0); // img_input for a keystroke in the window
    }

    return 0;
}

void filter_contrast(unsigned char & channel_pixel) {
    if (channel_pixel > 240) channel_pixel *= 0.98;
    if (channel_pixel < 30) channel_pixel *= 2.356;
}


void threshold(unsigned char & channel_pixel){
    if (channel_pixel > 210) channel_pixel = 200;
    if (channel_pixel < 30) channel_pixel = 10;
}

void sobel_edgeDetect(unsigned char * channel_input, unsigned char & c_pixel, int index, int colSize) {

    int v_kernel = (
        channel_input[index] * 0.0 +
                
        channel_input[index - 1] * 2.0 +     // west
        channel_input[index + 1] * -2.0 +     // east
        
        channel_input[index - colSize] * 0.0 +    // north
        channel_input[index + colSize] * 0.0 +    // south
        
        channel_input[index - colSize - 1] * 1.0 +    // northwest
        channel_input[index - colSize + 1] * -1.0 +    // northeast
        channel_input[index + colSize - 1] * 1.0 +    // southwest
        channel_input[index + colSize + 1] * -1.0 ) ;   // southeast

    int h_kernel = (
        channel_input[index] * 0.0 +
                
        channel_input[index - 1] * 0.0 +     // west
        channel_input[index + 1] * 0.0 +     // east
        
        channel_input[index - colSize] * 2.0 +    // north
        channel_input[index + colSize] * -2.0 +    // south
        
        channel_input[index - colSize - 1] * 1.0 +    // northwest
        channel_input[index - colSize + 1] * 1.0 +    // northeast
        channel_input[index + colSize - 1] * -1.0 +    // southwest
        channel_input[index + colSize + 1] * -1.0 ) ;   // southeast

        c_pixel = (unsigned char) std::abs( (h_kernel + v_kernel) / 2);
    
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
