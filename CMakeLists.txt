cmake_minimum_required(VERSION 3.14)
project(test_vtk LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(VTK REQUIRED)
find_package(LibPressio REQUIRED)
find_package(std_compat REQUIRED)

option(BUILD_SHARED_LIBS "build libraries as shared libraries" ON)

add_library(libpressio_vtk src/libpressio_vtk.cc)
target_link_libraries(libpressio_vtk PRIVATE ${VTK_LIBRARIES} LibPressio::libpressio)
target_include_directories(libpressio_vtk PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
  )
