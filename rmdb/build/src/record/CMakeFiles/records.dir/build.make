# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yyk/db2024/rmdb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yyk/db2024/rmdb/build

# Include any dependencies generated for this target.
include src/record/CMakeFiles/records.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/record/CMakeFiles/records.dir/compiler_depend.make

# Include the progress variables for this target.
include src/record/CMakeFiles/records.dir/progress.make

# Include the compile flags for this target's objects.
include src/record/CMakeFiles/records.dir/flags.make

src/record/CMakeFiles/records.dir/rm_file_handle.cpp.o: src/record/CMakeFiles/records.dir/flags.make
src/record/CMakeFiles/records.dir/rm_file_handle.cpp.o: ../src/record/rm_file_handle.cpp
src/record/CMakeFiles/records.dir/rm_file_handle.cpp.o: src/record/CMakeFiles/records.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yyk/db2024/rmdb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/record/CMakeFiles/records.dir/rm_file_handle.cpp.o"
	cd /home/yyk/db2024/rmdb/build/src/record && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/record/CMakeFiles/records.dir/rm_file_handle.cpp.o -MF CMakeFiles/records.dir/rm_file_handle.cpp.o.d -o CMakeFiles/records.dir/rm_file_handle.cpp.o -c /home/yyk/db2024/rmdb/src/record/rm_file_handle.cpp

src/record/CMakeFiles/records.dir/rm_file_handle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/records.dir/rm_file_handle.cpp.i"
	cd /home/yyk/db2024/rmdb/build/src/record && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yyk/db2024/rmdb/src/record/rm_file_handle.cpp > CMakeFiles/records.dir/rm_file_handle.cpp.i

src/record/CMakeFiles/records.dir/rm_file_handle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/records.dir/rm_file_handle.cpp.s"
	cd /home/yyk/db2024/rmdb/build/src/record && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yyk/db2024/rmdb/src/record/rm_file_handle.cpp -o CMakeFiles/records.dir/rm_file_handle.cpp.s

src/record/CMakeFiles/records.dir/rm_scan.cpp.o: src/record/CMakeFiles/records.dir/flags.make
src/record/CMakeFiles/records.dir/rm_scan.cpp.o: ../src/record/rm_scan.cpp
src/record/CMakeFiles/records.dir/rm_scan.cpp.o: src/record/CMakeFiles/records.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yyk/db2024/rmdb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/record/CMakeFiles/records.dir/rm_scan.cpp.o"
	cd /home/yyk/db2024/rmdb/build/src/record && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/record/CMakeFiles/records.dir/rm_scan.cpp.o -MF CMakeFiles/records.dir/rm_scan.cpp.o.d -o CMakeFiles/records.dir/rm_scan.cpp.o -c /home/yyk/db2024/rmdb/src/record/rm_scan.cpp

src/record/CMakeFiles/records.dir/rm_scan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/records.dir/rm_scan.cpp.i"
	cd /home/yyk/db2024/rmdb/build/src/record && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yyk/db2024/rmdb/src/record/rm_scan.cpp > CMakeFiles/records.dir/rm_scan.cpp.i

src/record/CMakeFiles/records.dir/rm_scan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/records.dir/rm_scan.cpp.s"
	cd /home/yyk/db2024/rmdb/build/src/record && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yyk/db2024/rmdb/src/record/rm_scan.cpp -o CMakeFiles/records.dir/rm_scan.cpp.s

# Object files for target records
records_OBJECTS = \
"CMakeFiles/records.dir/rm_file_handle.cpp.o" \
"CMakeFiles/records.dir/rm_scan.cpp.o"

# External object files for target records
records_EXTERNAL_OBJECTS =

lib/librecords.so: src/record/CMakeFiles/records.dir/rm_file_handle.cpp.o
lib/librecords.so: src/record/CMakeFiles/records.dir/rm_scan.cpp.o
lib/librecords.so: src/record/CMakeFiles/records.dir/build.make
lib/librecords.so: src/record/CMakeFiles/records.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yyk/db2024/rmdb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../../lib/librecords.so"
	cd /home/yyk/db2024/rmdb/build/src/record && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/records.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/record/CMakeFiles/records.dir/build: lib/librecords.so
.PHONY : src/record/CMakeFiles/records.dir/build

src/record/CMakeFiles/records.dir/clean:
	cd /home/yyk/db2024/rmdb/build/src/record && $(CMAKE_COMMAND) -P CMakeFiles/records.dir/cmake_clean.cmake
.PHONY : src/record/CMakeFiles/records.dir/clean

src/record/CMakeFiles/records.dir/depend:
	cd /home/yyk/db2024/rmdb/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yyk/db2024/rmdb /home/yyk/db2024/rmdb/src/record /home/yyk/db2024/rmdb/build /home/yyk/db2024/rmdb/build/src/record /home/yyk/db2024/rmdb/build/src/record/CMakeFiles/records.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/record/CMakeFiles/records.dir/depend

