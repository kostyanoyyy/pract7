cmake_minimum_required(VERSION 3.10)

project(WordSearch)

set(CMAKE_C_STANDARD 99)

# Указываем путь к исходному коду в папке src
add_executable(wordsearch src/wordsearch.c)
