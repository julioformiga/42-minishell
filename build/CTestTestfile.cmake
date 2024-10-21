# CMake generated Testfile for 
# Source directory: /home/julio/dev/42/cursus/minishell
# Build directory: /home/julio/dev/42/cursus/minishell/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(TestMinishell "/home/julio/dev/42/cursus/minishell/build/test_minishell")
set_tests_properties(TestMinishell PROPERTIES  _BACKTRACE_TRIPLES "/home/julio/dev/42/cursus/minishell/CMakeLists.txt;58;add_test;/home/julio/dev/42/cursus/minishell/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
