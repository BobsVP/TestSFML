cmake_minimum_required(VERSION 3.13)

project(SFMLTest VERSION 1.0 
 DESCRIPTION "SFML test"
 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SOURCES main.cpp funct.cpp Head.h)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/obj/")
message("${CMAKE_CURRENT_SOURCE_DIR}/obj/")
# Условие для операционной системы Ubuntu
if(UNIX AND NOT APPLE)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
# Условие для операционной системы Windows
elseif(WIN32)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1win/lib/cmake/SFML")
    #Устанавливаем статическую линковку библиотек
    set(SFML_STATIC_LIBRARIES TRUE)
endif()
#Убрать окно консоли
#set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")

# Подключаем SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(SFMLTest main.cpp funct.cpp)

# Линкуем SFML
target_link_libraries(SFMLTest sfml-graphics sfml-audio)


