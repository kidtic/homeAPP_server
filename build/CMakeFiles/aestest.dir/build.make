# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kk/SynologyDrive/bigProject/homeApp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kk/SynologyDrive/bigProject/homeApp/build

# Include any dependencies generated for this target.
include CMakeFiles/aestest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/aestest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/aestest.dir/flags.make

CMakeFiles/aestest.dir/test/aestest.cpp.o: CMakeFiles/aestest.dir/flags.make
CMakeFiles/aestest.dir/test/aestest.cpp.o: ../test/aestest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kk/SynologyDrive/bigProject/homeApp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/aestest.dir/test/aestest.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aestest.dir/test/aestest.cpp.o -c /home/kk/SynologyDrive/bigProject/homeApp/test/aestest.cpp

CMakeFiles/aestest.dir/test/aestest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aestest.dir/test/aestest.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kk/SynologyDrive/bigProject/homeApp/test/aestest.cpp > CMakeFiles/aestest.dir/test/aestest.cpp.i

CMakeFiles/aestest.dir/test/aestest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aestest.dir/test/aestest.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kk/SynologyDrive/bigProject/homeApp/test/aestest.cpp -o CMakeFiles/aestest.dir/test/aestest.cpp.s

# Object files for target aestest
aestest_OBJECTS = \
"CMakeFiles/aestest.dir/test/aestest.cpp.o"

# External object files for target aestest
aestest_EXTERNAL_OBJECTS =

../bin/aestest: CMakeFiles/aestest.dir/test/aestest.cpp.o
../bin/aestest: CMakeFiles/aestest.dir/build.make
../bin/aestest: ../lib/libhomeApp.so
../bin/aestest: CMakeFiles/aestest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kk/SynologyDrive/bigProject/homeApp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/aestest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aestest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/aestest.dir/build: ../bin/aestest

.PHONY : CMakeFiles/aestest.dir/build

CMakeFiles/aestest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/aestest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/aestest.dir/clean

CMakeFiles/aestest.dir/depend:
	cd /home/kk/SynologyDrive/bigProject/homeApp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kk/SynologyDrive/bigProject/homeApp /home/kk/SynologyDrive/bigProject/homeApp /home/kk/SynologyDrive/bigProject/homeApp/build /home/kk/SynologyDrive/bigProject/homeApp/build /home/kk/SynologyDrive/bigProject/homeApp/build/CMakeFiles/aestest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/aestest.dir/depend

