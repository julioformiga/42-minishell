# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/julio/dev/42/cursus/minishell

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/julio/dev/42/cursus/minishell/build

# Include any dependencies generated for this target.
include CMakeFiles/test_minishell.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_minishell.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_minishell.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_minishell.dir/flags.make

CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o: CMakeFiles/test_minishell.dir/flags.make
CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o: /home/julio/dev/42/cursus/minishell/test/test_minishell.cpp
CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o: CMakeFiles/test_minishell.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/julio/dev/42/cursus/minishell/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o -MF CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o.d -o CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o -c /home/julio/dev/42/cursus/minishell/test/test_minishell.cpp

CMakeFiles/test_minishell.dir/test/test_minishell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_minishell.dir/test/test_minishell.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julio/dev/42/cursus/minishell/test/test_minishell.cpp > CMakeFiles/test_minishell.dir/test/test_minishell.cpp.i

CMakeFiles/test_minishell.dir/test/test_minishell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_minishell.dir/test/test_minishell.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julio/dev/42/cursus/minishell/test/test_minishell.cpp -o CMakeFiles/test_minishell.dir/test/test_minishell.cpp.s

# Object files for target test_minishell
test_minishell_OBJECTS = \
"CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o"

# External object files for target test_minishell
test_minishell_EXTERNAL_OBJECTS =

test_minishell: CMakeFiles/test_minishell.dir/test/test_minishell.cpp.o
test_minishell: CMakeFiles/test_minishell.dir/build.make
test_minishell: lib/libgtest.a
test_minishell: lib/libgtest_main.a
test_minishell: lib/libgtest.a
test_minishell: CMakeFiles/test_minishell.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/julio/dev/42/cursus/minishell/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_minishell"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_minishell.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_minishell.dir/build: test_minishell
.PHONY : CMakeFiles/test_minishell.dir/build

CMakeFiles/test_minishell.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_minishell.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_minishell.dir/clean

CMakeFiles/test_minishell.dir/depend:
	cd /home/julio/dev/42/cursus/minishell/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/julio/dev/42/cursus/minishell /home/julio/dev/42/cursus/minishell /home/julio/dev/42/cursus/minishell/build /home/julio/dev/42/cursus/minishell/build /home/julio/dev/42/cursus/minishell/build/CMakeFiles/test_minishell.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test_minishell.dir/depend

