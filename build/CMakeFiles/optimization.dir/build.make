# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/paranid5/PROGRAMMING/plusi_ebanie/optimization

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build

# Include any dependencies generated for this target.
include CMakeFiles/optimization.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/optimization.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/optimization.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/optimization.dir/flags.make

CMakeFiles/optimization.dir/main.cpp.o: CMakeFiles/optimization.dir/flags.make
CMakeFiles/optimization.dir/main.cpp.o: /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/main.cpp
CMakeFiles/optimization.dir/main.cpp.o: CMakeFiles/optimization.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/optimization.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/optimization.dir/main.cpp.o -MF CMakeFiles/optimization.dir/main.cpp.o.d -o CMakeFiles/optimization.dir/main.cpp.o -c /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/main.cpp

CMakeFiles/optimization.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/optimization.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/main.cpp > CMakeFiles/optimization.dir/main.cpp.i

CMakeFiles/optimization.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/optimization.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/main.cpp -o CMakeFiles/optimization.dir/main.cpp.s

# Object files for target optimization
optimization_OBJECTS = \
"CMakeFiles/optimization.dir/main.cpp.o"

# External object files for target optimization
optimization_EXTERNAL_OBJECTS =

optimization: CMakeFiles/optimization.dir/main.cpp.o
optimization: CMakeFiles/optimization.dir/build.make
optimization: CMakeFiles/optimization.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable optimization"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/optimization.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/optimization.dir/build: optimization
.PHONY : CMakeFiles/optimization.dir/build

CMakeFiles/optimization.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/optimization.dir/cmake_clean.cmake
.PHONY : CMakeFiles/optimization.dir/clean

CMakeFiles/optimization.dir/depend:
	cd /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paranid5/PROGRAMMING/plusi_ebanie/optimization /home/paranid5/PROGRAMMING/plusi_ebanie/optimization /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build /home/paranid5/PROGRAMMING/plusi_ebanie/optimization/build/CMakeFiles/optimization.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/optimization.dir/depend
