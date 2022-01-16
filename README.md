# yolov4-opencv-cpp-python-perf

This repository is intended to investigating the overall performance of YOLOV4 dnn detection on CUDA for C++ and Python

## I'm getting different performances

The experiment is pretty straight forward: running YOLOV4 dnn detection 2000 times over 4 different images:

```python
frames = ...
for i in range(2000):
    frame = frames[i % 4]
    model.detect(frame, .2, .4)
```

For some unknown reason, I'm getting a significant faster performances when I run this code in Python if compared to the same code in C++ with CUBA enabled. Using only the CPU, both codes take same time.

## Running with CUDA

Running it on my machine (Intel I9, NVIDIA RTX 2080) with CUBA enabled I get:

```
$ time python3 python/yolo.py 

Frames count: 2000
real    0m6,374s
user    0m8,902s
sys     0m0,783s
```

Which give me 333 FPS. However, if I execute the counterpart code in C++:

```c++
frames = ...
for(int i = 0; i < 2000; ++i) {
    const cv::Mat & frame = frames[i % 4];
    model.detect(frame, classIds, confidences, boxes, .2, .4);
}
```
I ended up with:

```
$ time ./yolo_example 

Frames count: 2000

real    0m33,179s
user    6m14,921s
sys     0m6,942s
```

around 60 FPS, a relevant difference.

## Performances on CPU

Running using only the CPU, the Python code results in:

```
$ time python3 python/yolo.py 

Frames count: 2000

real    0m33,461s
user    6m18,398s
sys     0m6,928s
```

By running the C++ code, I get:

```
$ time ./yolo_example

Frames count: 2000

real    0m34,341s
user    6m19,379s
sys     0m7,908s
```

Roughly the same time.

## How did I compile the C++ code?

The C++ code was compiled with the following command:

```
g++ -O3 cpp/yolo.cpp -o yolo_example `pkg-config --cflags --libs opencv4`
```

## Some environment specs

### Python Version

3.8.10 (default, Nov 26 2021, 20:14:08) 
[GCC 9.3.0]

### OpenCV Version

4.5.3

### Operatin System

Ubuntu 20.04.3 LTS

### OpenCV CUDA Support 

['NVIDIA CUDA: YES (ver 11.6, CUFFT CUBLAS FAST_MATH)', 'NVIDIA GPU arch: 75', 'NVIDIA PTX archs:', 'cuDNN: YES (ver 8.3.2)']

### General configuration for OpenCV 4.5.3 

  Version control:               4.5.3

  Extra modules:
    Location (extra):            /home/user/opencv_build/opencv_contrib/modules
    Version control (extra):     4.5.3

  Platform:
    Timestamp:                   2022-01-16T07:08:00Z
    Host:                        Linux 5.11.0-46-generic x86_64
    CMake:                       3.16.3
    CMake generator:             Unix Makefiles
    CMake build tool:            /usr/bin/make
    Configuration:               RELEASE

  CPU/HW features:
    Baseline:                    SSE SSE2 SSE3
      requested:                 SSE3
    Dispatched code generation:  SSE4_1 SSE4_2 FP16 AVX AVX2 AVX512_SKX
      requested:                 SSE4_1 SSE4_2 AVX FP16 AVX2 AVX512_SKX
      SSE4_1 (15 files):         + SSSE3 SSE4_1
      SSE4_2 (1 files):          + SSSE3 SSE4_1 POPCNT SSE4_2
      FP16 (0 files):            + SSSE3 SSE4_1 POPCNT SSE4_2 FP16 AVX
      AVX (4 files):             + SSSE3 SSE4_1 POPCNT SSE4_2 AVX
      AVX2 (29 files):           + SSSE3 SSE4_1 POPCNT SSE4_2 FP16 FMA3 AVX AVX2
      AVX512_SKX (4 files):      + SSSE3 SSE4_1 POPCNT SSE4_2 FP16 FMA3 AVX AVX2 AVX_512F AVX512_COMMON AVX512_SKX

  C/C++:
    Built as dynamic libs?:      YES
    C++ standard:                11
    C++ Compiler:                /usr/bin/c++  (ver 9.3.0)
    C++ flags (Release):         -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wuninitialized -Wsuggest-override -Wno-delete-non-virtual-dtor -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -msse -msse2 -msse3 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -DNDEBUG  -DNDEBUG
    C++ flags (Debug):           -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wuninitialized -Wsuggest-override -Wno-delete-non-virtual-dtor -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -msse -msse2 -msse3 -fvisibility=hidden -fvisibility-inlines-hidden -g  -O0 -DDEBUG -D_DEBUG
    C Compiler:                  /usr/bin/cc
    C flags (Release):           -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wuninitialized -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -msse -msse2 -msse3 -fvisibility=hidden -O3 -DNDEBUG  -DNDEBUG
    C flags (Debug):             -fsigned-char -ffast-math -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wuninitialized -Wno-comment -Wimplicit-fallthrough=3 -Wno-strict-overflow -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -msse -msse2 -msse3 -fvisibility=hidden -g  -O0 -DDEBUG -D_DEBUG
    Linker flags (Release):      -Wl,--exclude-libs,libippicv.a -Wl,--exclude-libs,libippiw.a   -Wl,--gc-sections -Wl,--as-needed  
    Linker flags (Debug):        -Wl,--exclude-libs,libippicv.a -Wl,--exclude-libs,libippiw.a   -Wl,--gc-sections -Wl,--as-needed  
    ccache:                      NO
    Precompiled headers:         NO
    Extra dependencies:          m pthread cudart_static dl rt nppc nppial nppicc nppidei nppif nppig nppim nppist nppisu nppitc npps cublas cudnn cufft -L/usr/local/cuda/lib64 -L/usr/lib/x86_64-linux-gnu
    3rdparty dependencies:

  OpenCV modules:
    To be built:                 aruco barcode bgsegm bioinspired calib3d ccalib core cudaarithm cudabgsegm cudacodec cudafeatures2d cudafilters cudaimgproc cudalegacy cudaobjdetect cudaoptflow cudastereo cudawarping cudev datasets dnn dnn_objdetect dnn_superres dpm face features2d flann freetype fuzzy gapi hfs highgui img_hash imgcodecs imgproc intensity_transform line_descriptor mcc ml objdetect optflow phase_unwrapping photo plot python3 quality rapid reg rgbd saliency shape stereo stitching structured_light superres surface_matching text tracking video videoio videostab wechat_qrcode xfeatures2d ximgproc xobjdetect xphoto
    Disabled:                    world
    Disabled by dependency:      -
    Unavailable:                 alphamat cvv hdf java julia matlab ovis python2 sfm ts viz
    Applications:                apps
    Documentation:               NO
    Non-free algorithms:         YES

  GUI: 
    GTK+:                        YES (ver 3.24.20)
      GThread :                  YES (ver 2.64.6)
      GtkGlExt:                  NO
    VTK support:                 NO

  Media I/O: 
    ZLib:                        /usr/lib/x86_64-linux-gnu/libz.so (ver 1.2.11)
    JPEG:                        /usr/lib/x86_64-linux-gnu/libjpeg.so (ver 80)
    WEBP:                        build (ver encoder: 0x020f)
    PNG:                         /usr/lib/x86_64-linux-gnu/libpng.so (ver 1.6.37)
    TIFF:                        /usr/lib/x86_64-linux-gnu/libtiff.so (ver 42 / 4.1.0)
    JPEG 2000:                   build (ver 2.4.0)
    OpenEXR:                     build (ver 2.3.0)
    HDR:                         YES
    SUNRASTER:                   YES
    PXM:                         YES
    PFM:                         YES

  Video I/O:
    DC1394:                      YES (2.2.5)
    FFMPEG:                      YES
      avcodec:                   YES (58.54.100)
      avformat:                  YES (58.29.100)
      avutil:                    YES (56.31.100)
      swscale:                   YES (5.5.100)
      avresample:                NO
    v4l/v4l2:                    YES (linux/videodev2.h)

  Parallel framework:            TBB (ver 2020.2 interface 11102)

  Trace:                         YES (with Intel ITT)

  Other third-party libraries:
    Intel IPP:                   2020.0.0 Gold [2020.0.0]
           at:                   /home/user/opencv_build/opencv/build/3rdparty/ippicv/ippicv_lnx/icv
    Intel IPP IW:                sources (2020.0.0)
              at:                /home/user/opencv_build/opencv/build/3rdparty/ippicv/ippicv_lnx/iw
    VA:                          NO
    Lapack:                      NO
    Eigen:                       NO
    Custom HAL:                  NO
    Protobuf:                    build (3.5.1)

  NVIDIA CUDA:                   YES (ver 11.6, CUFFT CUBLAS FAST_MATH)
    NVIDIA GPU arch:             75
    NVIDIA PTX archs:

  cuDNN:                         YES (ver 8.3.2)

  OpenCL:                        YES (no extra features)
    Include path:                /home/user/opencv_build/opencv/3rdparty/include/opencl/1.2
    Link libraries:              Dynamic load

  Python 3:
    Interpreter:                 /usr/bin/python3 (ver 3.8.10)
    Libraries:                   /usr/lib/x86_64-linux-gnu/libpython3.8.so (ver 3.8.10)
    numpy:                       /usr/lib/python3/dist-packages/numpy/core/include (ver 1.17.4)
    install path:                lib/python3.8/dist-packages/cv2/python-3.8

  Python (for build):            /usr/bin/python3

  Java:                          
    ant:                         NO
    JNI:                         NO
    Java wrappers:               NO
    Java tests:                  NO

  Install to:                    /usr/local