# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug

# Include any dependencies generated for this target.
include libigl/nanogui/CMakeFiles/bin2c.dir/depend.make

# Include the progress variables for this target.
include libigl/nanogui/CMakeFiles/bin2c.dir/progress.make

# Include the compile flags for this target's objects.
include libigl/nanogui/CMakeFiles/bin2c.dir/flags.make

libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o: libigl/nanogui/CMakeFiles/bin2c.dir/flags.make
libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o: /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/libigl/external/nanogui/resources/bin2c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o"
	cd /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui && /bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bin2c.dir/resources/bin2c.c.o   -c /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/libigl/external/nanogui/resources/bin2c.c

libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bin2c.dir/resources/bin2c.c.i"
	cd /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui && /bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/libigl/external/nanogui/resources/bin2c.c > CMakeFiles/bin2c.dir/resources/bin2c.c.i

libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bin2c.dir/resources/bin2c.c.s"
	cd /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui && /bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/libigl/external/nanogui/resources/bin2c.c -o CMakeFiles/bin2c.dir/resources/bin2c.c.s

libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.requires:

.PHONY : libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.requires

libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.provides: libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.requires
	$(MAKE) -f libigl/nanogui/CMakeFiles/bin2c.dir/build.make libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.provides.build
.PHONY : libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.provides

libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.provides.build: libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o


# Object files for target bin2c
bin2c_OBJECTS = \
"CMakeFiles/bin2c.dir/resources/bin2c.c.o"

# External object files for target bin2c
bin2c_EXTERNAL_OBJECTS =

libigl/nanogui/bin2c: libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o
libigl/nanogui/bin2c: libigl/nanogui/CMakeFiles/bin2c.dir/build.make
libigl/nanogui/bin2c: libigl/nanogui/CMakeFiles/bin2c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin2c"
	cd /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bin2c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libigl/nanogui/CMakeFiles/bin2c.dir/build: libigl/nanogui/bin2c

.PHONY : libigl/nanogui/CMakeFiles/bin2c.dir/build

libigl/nanogui/CMakeFiles/bin2c.dir/requires: libigl/nanogui/CMakeFiles/bin2c.dir/resources/bin2c.c.o.requires

.PHONY : libigl/nanogui/CMakeFiles/bin2c.dir/requires

libigl/nanogui/CMakeFiles/bin2c.dir/clean:
	cd /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui && $(CMAKE_COMMAND) -P CMakeFiles/bin2c.dir/cmake_clean.cmake
.PHONY : libigl/nanogui/CMakeFiles/bin2c.dir/clean

libigl/nanogui/CMakeFiles/bin2c.dir/depend:
	cd /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1 /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/libigl/external/nanogui /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui /home/zettanux/Desktop/ETHZ/ShapeModelling/gp18-Bobavince/assignment1/Debug/libigl/nanogui/CMakeFiles/bin2c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libigl/nanogui/CMakeFiles/bin2c.dir/depend

