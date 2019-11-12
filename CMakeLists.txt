project(dream-atlas C)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_BUILD_TYPE Debug)

file(GLOB dream_atlas_sources src/*.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
find_package(cairo REQUIRED)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_definitions(${GTK_CFLAGS_OTHER})

add_executable(atlas ${dream_atlas_sources})

target_link_libraries(atlas ${GTK_LIBRARIES} cairo m)

#copy .ui files to bin
configure_file(test.ui bin/test.ui COPYONLY)



