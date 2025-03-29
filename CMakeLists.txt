cmake_minimum_required(VERSION 3.30)
project(COMP155Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(COMP155Project src/main.cpp
        include/user.h
        include/employee.h
        include/administrator.h
        include/helper.h
        include/leave_request.h
        include/leave_manager.h)
