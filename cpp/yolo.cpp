#include <fstream>

#include <opencv2/opencv.hpp>

const std::string YOLO_VERSION = "v4-tiny";

void load_net(cv::dnn::Net &net, bool is_cuda) {
    auto result = cv::dnn::readNetFromDarknet("config_files/yolo" + YOLO_VERSION + ".cfg", "config_files/yolo" + YOLO_VERSION + ".weights");
    if (is_cuda) {
        std::cout << "Attempty to use CUDA\n";
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    } else {
        std::cout << "Running on CPU\n";
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }
    net = result;
}

int main(int, char **)
{

    const std::vector<cv::Mat> frames = {
        cv::imread("frames/frame-0.png", cv::IMREAD_COLOR), 
        cv::imread("frames/frame-1.png", cv::IMREAD_COLOR),
        cv::imread("frames/frame-2.png", cv::IMREAD_COLOR),
        cv::imread("frames/frame-3.png", cv::IMREAD_COLOR)};

    const int frames_size = frames.size();

    cv::dnn::Net net;
    load_net(net, false);

    auto model = cv::dnn::DetectionModel(net);
    model.setInputParams(1./255, cv::Size(416, 416), cv::Scalar(), true);

    const int total_frames = 2000;
    int frames_count = 0;

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for(int i = 0; i < total_frames; ++i) {
        const cv::Mat & frame = frames[i % frames_size];
        model.detect(frame, classIds, confidences, boxes, .2, .4);
        frames_count++;
    }

    std::cout << "Frames count: " << frames_count << "\n";

    return 0;
}