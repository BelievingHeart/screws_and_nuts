cmake_minimum_required(VERSION 3.1...3.12)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(cv_window_interactions)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_LINK_WHAT_YOU_USE TRUE)
set(CMAKE_CXX_COMPILE_FEATURES  -Wall -Wextra -Wpedantic -Weffc++)

#find_program(TIDY "clang-tidy")
#set(CMAKE_CXX_CLANG_TIDY ${TIDY} "-checks=*")
#find_program(CPPCHECK cppcheck "--std=c++14")
#set(CMAKE_CXX_CPPCHECK ${CPPCHECK})
find_program(IWYU include-what-you-use)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE  ${IWYU})

find_package(OpenCV REQUIRED)

add_library(histogram SHARED draw_histogram.cpp)
target_include_directories(histogram PRIVATE ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(binarization PRIVATE ${OpenCV_LIBS})
target_link_libraries(histogram PRIVATE opencv_highgui opencv_core opencv_imgproc)

add_library(binarization SHARED binarization.cpp)
target_include_directories(binarization PRIVATE ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(binarization PRIVATE ${OpenCV_LIBS})
target_link_libraries(binarization PRIVATE opencv_imgproc histogram)

add_executable(main main.cpp)
target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(main PRIVATE ${OpenCV_LIBS})
target_link_libraries(main PRIVATE opencv_highgui opencv_core opencv_imgcodecs binarization histogram)