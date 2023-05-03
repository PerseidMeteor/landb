cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("landb")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Replxx REQUIRED)
find_package(Threads REQUIRED)
# find_package(Clipp REQUIRED)

# add_library(${PROJECT_NAME}

# )

# target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# target_link_libraries(${PROJECT_NAME}
# )


add_executable(
    landb 
    main.cc
)

target_link_libraries(
  ${PROJECT_NAME}
  replxx::replxx
  # clipp::clipp
)
##############################  test  ##############################