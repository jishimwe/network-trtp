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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/ishim/CLionProjects/Network

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/new_network_project.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/new_network_project.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/new_network_project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/new_network_project.dir/flags.make

CMakeFiles/new_network_project.dir/entity/log.c.o: CMakeFiles/new_network_project.dir/flags.make
CMakeFiles/new_network_project.dir/entity/log.c.o: ../entity/log.c
CMakeFiles/new_network_project.dir/entity/log.c.o: CMakeFiles/new_network_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/new_network_project.dir/entity/log.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/new_network_project.dir/entity/log.c.o -MF CMakeFiles/new_network_project.dir/entity/log.c.o.d -o CMakeFiles/new_network_project.dir/entity/log.c.o -c /mnt/c/Users/ishim/CLionProjects/Network/entity/log.c

CMakeFiles/new_network_project.dir/entity/log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/new_network_project.dir/entity/log.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/ishim/CLionProjects/Network/entity/log.c > CMakeFiles/new_network_project.dir/entity/log.c.i

CMakeFiles/new_network_project.dir/entity/log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/new_network_project.dir/entity/log.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/ishim/CLionProjects/Network/entity/log.c -o CMakeFiles/new_network_project.dir/entity/log.c.s

CMakeFiles/new_network_project.dir/entity/packet_interface.c.o: CMakeFiles/new_network_project.dir/flags.make
CMakeFiles/new_network_project.dir/entity/packet_interface.c.o: ../entity/packet_interface.c
CMakeFiles/new_network_project.dir/entity/packet_interface.c.o: CMakeFiles/new_network_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/new_network_project.dir/entity/packet_interface.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/new_network_project.dir/entity/packet_interface.c.o -MF CMakeFiles/new_network_project.dir/entity/packet_interface.c.o.d -o CMakeFiles/new_network_project.dir/entity/packet_interface.c.o -c /mnt/c/Users/ishim/CLionProjects/Network/entity/packet_interface.c

CMakeFiles/new_network_project.dir/entity/packet_interface.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/new_network_project.dir/entity/packet_interface.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/ishim/CLionProjects/Network/entity/packet_interface.c > CMakeFiles/new_network_project.dir/entity/packet_interface.c.i

CMakeFiles/new_network_project.dir/entity/packet_interface.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/new_network_project.dir/entity/packet_interface.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/ishim/CLionProjects/Network/entity/packet_interface.c -o CMakeFiles/new_network_project.dir/entity/packet_interface.c.s

CMakeFiles/new_network_project.dir/entity/main.c.o: CMakeFiles/new_network_project.dir/flags.make
CMakeFiles/new_network_project.dir/entity/main.c.o: ../entity/main.c
CMakeFiles/new_network_project.dir/entity/main.c.o: CMakeFiles/new_network_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/new_network_project.dir/entity/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/new_network_project.dir/entity/main.c.o -MF CMakeFiles/new_network_project.dir/entity/main.c.o.d -o CMakeFiles/new_network_project.dir/entity/main.c.o -c /mnt/c/Users/ishim/CLionProjects/Network/entity/main.c

CMakeFiles/new_network_project.dir/entity/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/new_network_project.dir/entity/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/ishim/CLionProjects/Network/entity/main.c > CMakeFiles/new_network_project.dir/entity/main.c.i

CMakeFiles/new_network_project.dir/entity/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/new_network_project.dir/entity/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/ishim/CLionProjects/Network/entity/main.c -o CMakeFiles/new_network_project.dir/entity/main.c.s

CMakeFiles/new_network_project.dir/entity/receiver.c.o: CMakeFiles/new_network_project.dir/flags.make
CMakeFiles/new_network_project.dir/entity/receiver.c.o: ../entity/receiver.c
CMakeFiles/new_network_project.dir/entity/receiver.c.o: CMakeFiles/new_network_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/new_network_project.dir/entity/receiver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/new_network_project.dir/entity/receiver.c.o -MF CMakeFiles/new_network_project.dir/entity/receiver.c.o.d -o CMakeFiles/new_network_project.dir/entity/receiver.c.o -c /mnt/c/Users/ishim/CLionProjects/Network/entity/receiver.c

CMakeFiles/new_network_project.dir/entity/receiver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/new_network_project.dir/entity/receiver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/ishim/CLionProjects/Network/entity/receiver.c > CMakeFiles/new_network_project.dir/entity/receiver.c.i

CMakeFiles/new_network_project.dir/entity/receiver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/new_network_project.dir/entity/receiver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/ishim/CLionProjects/Network/entity/receiver.c -o CMakeFiles/new_network_project.dir/entity/receiver.c.s

CMakeFiles/new_network_project.dir/entity/sender.c.o: CMakeFiles/new_network_project.dir/flags.make
CMakeFiles/new_network_project.dir/entity/sender.c.o: ../entity/sender.c
CMakeFiles/new_network_project.dir/entity/sender.c.o: CMakeFiles/new_network_project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/new_network_project.dir/entity/sender.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/new_network_project.dir/entity/sender.c.o -MF CMakeFiles/new_network_project.dir/entity/sender.c.o.d -o CMakeFiles/new_network_project.dir/entity/sender.c.o -c /mnt/c/Users/ishim/CLionProjects/Network/entity/sender.c

CMakeFiles/new_network_project.dir/entity/sender.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/new_network_project.dir/entity/sender.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/ishim/CLionProjects/Network/entity/sender.c > CMakeFiles/new_network_project.dir/entity/sender.c.i

CMakeFiles/new_network_project.dir/entity/sender.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/new_network_project.dir/entity/sender.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/ishim/CLionProjects/Network/entity/sender.c -o CMakeFiles/new_network_project.dir/entity/sender.c.s

# Object files for target new_network_project
new_network_project_OBJECTS = \
"CMakeFiles/new_network_project.dir/entity/log.c.o" \
"CMakeFiles/new_network_project.dir/entity/packet_interface.c.o" \
"CMakeFiles/new_network_project.dir/entity/main.c.o" \
"CMakeFiles/new_network_project.dir/entity/receiver.c.o" \
"CMakeFiles/new_network_project.dir/entity/sender.c.o"

# External object files for target new_network_project
new_network_project_EXTERNAL_OBJECTS =

new_network_project: CMakeFiles/new_network_project.dir/entity/log.c.o
new_network_project: CMakeFiles/new_network_project.dir/entity/packet_interface.c.o
new_network_project: CMakeFiles/new_network_project.dir/entity/main.c.o
new_network_project: CMakeFiles/new_network_project.dir/entity/receiver.c.o
new_network_project: CMakeFiles/new_network_project.dir/entity/sender.c.o
new_network_project: CMakeFiles/new_network_project.dir/build.make
new_network_project: /usr/lib/x86_64-linux-gnu/libz.so
new_network_project: CMakeFiles/new_network_project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable new_network_project"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/new_network_project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/new_network_project.dir/build: new_network_project
.PHONY : CMakeFiles/new_network_project.dir/build

CMakeFiles/new_network_project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/new_network_project.dir/cmake_clean.cmake
.PHONY : CMakeFiles/new_network_project.dir/clean

CMakeFiles/new_network_project.dir/depend:
	cd /mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/ishim/CLionProjects/Network /mnt/c/Users/ishim/CLionProjects/Network /mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug /mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug /mnt/c/Users/ishim/CLionProjects/Network/cmake-build-debug/CMakeFiles/new_network_project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/new_network_project.dir/depend

