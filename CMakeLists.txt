cmake_minimum_required(VERSION 3.13)
project(u2f_auth_server_cli C)

set(CMAKE_C_STANDARD 99)

add_executable(u2f_auth_server_cli main.c u2fdbt.c sha1.c)
