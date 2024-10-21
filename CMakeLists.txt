cmake_minimum_required(VERSION 3.16.3)

project(MiniShell)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

file(GLOB SRC "src/*.c" "src/*/*.c")
file(GLOB LIBFT_SRC "lib/libft/src/*.c")
file(GLOB LIBFT_PRINTF_SRC "lib/libft/src/ft_printf/*.c")

add_executable(minishell ${SRC} ${LIBFT_SRC} ${LIBFT_PRINTF_SRC})

target_link_libraries(minishell readline)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
set(BUILD_GMOCK OFF)
FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
    FetchContent_MakeAvailable(googletest)
endif()

# =============================== MAKE DOC =====================================
# find_package(Doxygen REQUIRED)
#
# set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
# set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)
#
# if(NOT EXISTS ${DOXYGEN_IN})
#     message(FATAL_ERROR "Doxyfile não encontrado. Por favor, crie um Doxyfile com 'doxygen -g'.")
# endif()
#
# add_custom_target(doc ALL
#     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     COMMENT "Gerando documentação com Doxygen"
#     VERBATIM)
#
# install(DIRECTORY ${DOXYGEN_OUT} DESTINATION doc)

# ============================== TESTS =========================================
enable_testing()
set(TEST_DIR "test/")
file(GLOB TEST_SRC "${TEST_DIR}*.cpp")

add_executable(test_minishell ${TEST_SRC})
target_link_libraries(test_minishell gtest gtest_main)
add_test(NAME TestMinishell COMMAND test_minishell)
