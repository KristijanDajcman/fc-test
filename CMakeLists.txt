cmake_minimum_required(VERSION 3.12)
project(run)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.10.4 
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(run main.cpp)

target_link_libraries(run PRIVATE nlohmann_json::nlohmann_json)
