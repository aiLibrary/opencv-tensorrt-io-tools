#include <opencv2/core.hpp>
#include <vector>

void CVRGBFrames2FloatArrayBridge(const std::vector<cv::Mat> inputFrames, float** buffer, const int imgH, const int imgW, const int batchSize);
