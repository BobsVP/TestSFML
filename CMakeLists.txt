cmake_minimum_required(VERSION 3.13)

project(SFMLTest VERSION 1.0 
 DESCRIPTION "SFML test"
 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(SOURCES main.cpp)

# Условие для операционной системы Ubuntu
if(UNIX AND NOT APPLE)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
# Условие для операционной системы Windows
elseif(WIN32)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
endif()

# Подключаем SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(SFMLTest main.cpp)

# Линкуем SFML
target_link_libraries(SFMLTest sfml-graphics sfml-audio)


