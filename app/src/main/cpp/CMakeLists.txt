
cmake_minimum_required(VERSION 3.4.1)

set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../distribution)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavcodec-57.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavformat-57.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/ffmpeg/lib/${ANDROID_ABI}/libavutil-55.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/ffmpeg/lib/${ANDROID_ABI}/libswresample-2.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/ffmpeg/lib/${ANDROID_ABI}/libswscale-4.so)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library(audio SHARED
            AudioJNI.c
            opensl.c
            AudioDecoder.c
           )

target_include_directories(audio PRIVATE
                           ${distribution_DIR}/ffmpeg/include)

target_link_libraries(audio
                      android
                      log
                      z
                      OpenSLES
                      avcodec
                      avformat
                      avutil
                      swresample
                      swscale
                     )