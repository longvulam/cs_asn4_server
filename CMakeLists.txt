cmake_minimum_required(VERSION 3.20)
project(cs_asn4_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

add_executable(cs_asn4_server
        multithreadedserver.cpp ServletBase.hpp
        UploadServlet.cpp UploadServlet.hpp
        HttpResponse.cpp HttpResponse.hpp
        HttpRequest.cpp HttpRequest.hpp
        Thread.cpp Thread.hpp
        ServerThread.cpp ServerThread.hpp
        ServerSocket.cpp ServerSocket.hpp
        Socket.cpp Socket.hpp
        FilePart.cpp FilePart.hpp)