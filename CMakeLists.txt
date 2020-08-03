cmake_minimum_required(VERSION 2.8)
project(homeApp)

# cmake设置
set(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -W -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -W")

# 设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


message("++++++++++++++++++++Start Build++++++++++++++++++++++++++")

# 头文件搜索目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 链接库搜索目录
link_directories(${PROJECT_BINARY_DIR}/lib /usr/local/lib /usr/lib)


#设置链接库
add_library(${PROJECT_NAME} SHARED
        src/homeAppComm.cpp 
        src/homeAppCore.cpp
        src/homeAppSys.cpp
        src/dbsql.cpp
        
)
target_link_libraries(${PROJECT_NAME}
    cryptopp
    mysqlclient
    jsoncpp
)


# 可执行文件

add_executable(aestest test/aestest.cpp)
target_link_libraries(aestest ${PROJECT_NAME})


add_executable(homeApp_server test/server.cpp)
target_link_libraries(homeApp_server ${PROJECT_NAME})


add_executable(dotaskTest test/dotaskTest.cpp)
target_link_libraries(dotaskTest ${PROJECT_NAME})

