# 指定cmake最低编译版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)

# 指定工程的名称
PROJECT(modbuTest)

# 将源文件赋值给 SRC_LIST 变量，也可以指定多个源文件，用空格隔开
# SET(SRC_LIST xx1.cpp xx2.cpp xx3.cpp)
#SET(SRC_LIST src/main.cpp src/file.cpp src/fw-upg.cpp)

# 把当前工程目录下的 src 目录的下的所有 .cpp 和 .c 文件赋值给 SRC_LIST
# AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SRC_LIST)
FILE(GLOB SRC_LIST "${PROJECT_SOURCE_DIR}/src/*.c")

# 动态库文件
SET(LINK_LIBS libmodbus.so)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11")
FIND_PACKAGE(Threads)

#指定头文件目录位置
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/inc/modbus)

#添加共享库搜索路径
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

#生成可执行文件
ADD_EXECUTABLE(modbus ${SRC_LIST})

#添加共享库链接
TARGET_LINK_LIBRARIES(modbus ${LINK_LIBS})
TARGET_LINK_LIBRARIES(modbus ${CMAKE_THREAD_LIBS_INIT})
#cd build
#cmake .
#make