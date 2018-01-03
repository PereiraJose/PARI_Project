cmake_minimum_required (VERSION 2.8)
project (Test_stream)
set(PROG1 client)
set(PROG2 server)

set(SOURCES1 src/client/client.cpp )
set(SOURCES2 src/server/server.cpp )

add_executable(${PROG1} ${SOURCES1})
add_executable(${PROG2} ${SOURCES2})

find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROG1} m ${OpenCV_LIBS} -lpthread)
target_link_libraries(${PROG2} m ${OpenCV_LIBS} -lpthread)



