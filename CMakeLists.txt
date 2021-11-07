cmake_minimum_required(VERSION 3.20)
project(cs_asn4_server)
#project(cs_asn4_server C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

add_executable(cs_asn4_server multithreadedserver.cpp Servlet.hpp UploadServlet.cpp HttpResponse.cpp HttpRequest.cpp HttpRequest.hpp ServerThread.cpp ServerThread.hpp FilePart.cpp FilePart.hpp)