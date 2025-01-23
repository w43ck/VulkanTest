cmake_minimum_required(VERSION 3.5.0)
project(Vulk VERSION 0.1.0 LANGUAGES C CXX)
set(project_dir ${CMAKE_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
#include(CTest)
#enable_testing()

# Укажем компилятор
set(CMAKE_CXX_COMPILER clang++)

# Укажем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Устанавливаем флаги компиляции для дебаг-версии
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Weverything")

# Устанавливаем флаги компиляции для релиз-версии
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -s")

# Убираем флаги -nostartfiles и -nostdlib
# string(REPLACE "-nostartfiles" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# string(REPLACE "-nostdlib" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ compiler flags: ${CMAKE_CXX_FLAGS}")

#Дополнительные файлы проекта
include_directories(inc engine/src engine/src/core engine/src/platform ${VULKAN_SDK}/Include)
link_directories(${VULKAN_SDK}/Lib)
file(GLOB SRCC "./src/*.cpp")
set(libs "vulkan-1 user32")

file(GLOB LIBCORESRC "engine/src/core/*.cpp")
file(GLOB LIBPLATFORMSRC "engine/src/platform/*.cpp")
add_library(engine SHARED ${LIBCORESRC} ${LIBPLATFORMSRC})
target_compile_definitions(engine PRIVATE WEXPORT)
target_include_directories(engine PUBLIC engine/src engine/src/core engine/src/platform)

# Добавляем флаги компиляции
add_compile_options(-Wno-c++98-compat -Wno-pragma-pack)

add_executable(${PROJECT_NAME} engine/src/entry.cpp ${SRCC})

# ИКОНКА из ресурсного файла
#target_sources(${PROJECT_NAME} PRIVATE src/app.rc)

# Условное связывание библиотек в зависимости от конфигурации
target_link_libraries(${PROJECT_NAME} PRIVATE 
    #$<$<CONFIG:Debug>:sfml-main-d sfml-graphics-s-d sfml-window-s-d sfml-system-s-d>
    #$<$<CONFIG:Release>:sfml-main sfml-graphics-s sfml-window-s sfml-system-s>
		
		engine
)

# Указываем флаг подсистемы Windows для компоновщика чтоб запускалось графическое приложение без консольного окна
target_link_options(${PROJECT_NAME} PRIVATE  -Xlinker /SUBSYSTEM:CONSOLE) #WINDOWS


# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# #настройка метаданных проекта для упаковки
# set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
# set(CPACK_PACKAGE_VERSION "1.0.0")
# set(CPACK_PACKAGE_DESCRIPTION "Yea! My first text game")
# set(CPACK_PACKAGE_VENDOR "Wreck Co")
# set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

# # Указать тип пакета (NSIS для создания установщика .exe)
# #set(CPACK_GENERATOR ZIP)
# set(CPACK_GENERATOR "NSIS64")
# # Указать иконку для установщика (если нужно)
# #set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/1.ico")

# # Установка исполняемого файла
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# # Установка дополнительных файлов
# install(DIRECTORY ${CMAKE_SOURCE_DIR}/src DESTINATION resources)
# install(FILES ${CMAKE_SOURCE_DIR}/CMakeLists.txt DESTINATION config)