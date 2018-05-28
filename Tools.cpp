#include "Tools.h"

void CVRGBFrames2FloatArrayBridge(const std::vector<cv::Mat> inputFrames, float** buffer, const int imgH, const int imgW, const int batchSize) {
    // binding cv::Mat vector to array data
    std::vector<std::vector<cv::Mat>> input_channels;
	for (int n = 0; n < batchSize; ++n) {
        std::cout << "Creating the bridge on " << n << " image" << std::endl;
        float* input_data = buffer[n];
		input_channels.push_back(std::vector<cv::Mat>());
		for (int i = 0; i < 3; ++i) {
			cv::Mat channel(imgH, imgW, CV_32FC1, input_data);
			input_channels[n].push_back(channel);
			input_data += imgH * imgW;
		}
	}
   
    try {
        for(int index = 0; index < batchSize; index++) {
            cv::Mat img = inputFrames[index];
            cv::Mat sample;
            int num_channels_ = input_channels[index].size();
            if (img.channels() == 3 && num_channels_ == 1)
                cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
            else if (img.channels() == 4 && num_channels_ == 1)
                cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
            else if (img.channels() == 4 && num_channels_ == 3)
                cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
            else if (img.channels() == 1 && num_channels_ == 3)
                cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
            else
                sample = img;

            cv::Size input_geometry = cv::Size(input_channels[0][0].cols, input_channels[0][0].rows);
            std::cout << "The " << index << " image size is: " << input_channels[index][0].cols << " X " << input_channels[index][0].rows << std::endl;
            
            cv::Mat sample_resized;
            /*preproc-resample */
            if (sample.size() != input_geometry)
                cv::resize(sample, sample_resized, input_geometry);
            else
                sample_resized = sample;
            cv::Mat sample_float;
            if (num_channels_ == 3)
                sample_resized.convertTo(sample_float, CV_32FC3);
            else
                sample_resized.convertTo(sample_float, CV_32FC1);
            std::cout << "Resize success... " << std::endl;
            /* END */        
            /* preproc-normalize */
            cv::Mat sample_normalized(imgH, imgW, CV_32FC3);
            std::cout << "Normalized success... " << std::endl;
            
            bool _rescaleTo01 = true;
            if (_rescaleTo01) {
                sample_float = sample_float / 255.f;
            }
            std::cout << "Rescale success... " << std::endl;

            sample_float.convertTo(sample_normalized, CV_32FC3);
            std::cout << "Convert success... " << std::endl;
            cv::split(sample_normalized, input_channels[index]);
            std::cout << "Split success... " << std::endl;
        }
    }catch(cv::Exception& e){
        const char* err_msg = e.what();
        std::cout << err_msg << std::endl;
    }
}