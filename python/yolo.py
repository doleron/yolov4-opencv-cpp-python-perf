import cv2
import sys
import re

YOLO_VERSION = "v4-tiny"

def build_model(is_cuda):
    net = cv2.dnn.readNet("config_files/yolo" + YOLO_VERSION + ".weights", "config_files/yolo" + YOLO_VERSION + ".cfg")
    if is_cuda:
        print("Attempty to use CUDA")
        net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
        net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA_FP16)
    else:
        print("Running on CPU")
        net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
        net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)

    model = cv2.dnn_DetectionModel(net)
    model.setInputParams(size=(416, 416), scale=1./255, swapRB=True)
    return model

frames = [cv2.imread("frames/frame-0.png", cv2.IMREAD_COLOR), 
    cv2.imread("frames/frame-1.png", 
    cv2.IMREAD_COLOR), cv2.imread("frames/frame-2.png", cv2.IMREAD_COLOR), 
    cv2.imread("frames/frame-3.png", cv2.IMREAD_COLOR)]

frames_size = len(frames)

model = build_model(False)

total_frames = 2000
frames_count = 0

for i in range(total_frames):

    frame = frames[i % frames_size]

    model.detect(frame, .2, .4)
    frames_count += 1

print("Frames count: " + str(frames_count))