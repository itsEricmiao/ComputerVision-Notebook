cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
add_executable( Threshold_Demo Main.cpp )
target_link_libraries( Threshold_Demo ${OpenCV_LIBS} )
install(FILES "opencv2/opencv.hpp"
        DESTINATION ${OPENCV_INCLUDE_INSTALL_PATH}/opencv2
        COMPONENT dev)
