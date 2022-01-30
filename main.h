

void blur_gauss(unsigned char * channel_input, unsigned char &p, int index, int colSize);
void sobel_edgeDetect(cv::Mat &imgInput, cv::Mat &imgOutput);
void threshold (cv::Mat &imgInput, cv::Mat &imgOut, int threVal);
void filter_contrast(cv::Mat &imgInput, cv::Mat &imgOut);
