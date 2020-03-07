cmake_minimum_required(VERSION 3.12)
project(Plug VERSION 0.1.1)
add_definitions("-fPIC")

set(CMAKE_CXX_STANDARD 17)
set(VERBOSE)

include(${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}.cmake)
