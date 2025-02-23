cmake_minimum_required(VERSION 3.28)
set(CMAKE_C_STANDARD 23)

project(au VERSION 1.0 LANGUAGES C)

set(TARGET_NAME au)

add_executable(au main.c)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()