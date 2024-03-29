cmake_minimum_required(VERSION 3.28)
project(lb3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNICODE -D_UNICODE")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")

add_library(lb2rsa MODULE lb2rsa.cpp)

add_library(locale_en MODULE locale_en.cpp locale_en.rc)
add_library(locale_uk MODULE locale_uk.cpp locale_uk.rc)

add_custom_command(TARGET locale_en PRE_BUILD COMMAND ${CMAKE_RC_COMPILER} -i ${CMAKE_CURRENT_SOURCE_DIR}/locale_en.rc -o ${CMAKE_CURRENT_BINARY_DIR}/locale_en.o WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET locale_uk PRE_BUILD COMMAND ${CMAKE_RC_COMPILER} -i ${CMAKE_CURRENT_SOURCE_DIR}/locale_uk.rc -o ${CMAKE_CURRENT_BINARY_DIR}/locale_uk.o WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_sources(locale_en PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/locale_en.o)
target_sources(locale_uk PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/locale_uk.o)

add_executable(lb3main main.cpp)
add_dependencies(lb3main lb2rsa locale_en locale_uk)