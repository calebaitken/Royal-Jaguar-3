# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.1.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.1.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\cmake-build-release"

# Include any dependencies generated for this target.
include CMakeFiles/stdthreadtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/stdthreadtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stdthreadtest.dir/flags.make

CMakeFiles/stdthreadtest.dir/tests.cpp.obj: CMakeFiles/stdthreadtest.dir/flags.make
CMakeFiles/stdthreadtest.dir/tests.cpp.obj: ../tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\cmake-build-release\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/stdthreadtest.dir/tests.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\stdthreadtest.dir\tests.cpp.obj -c "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\tests.cpp"

CMakeFiles/stdthreadtest.dir/tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stdthreadtest.dir/tests.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\tests.cpp" > CMakeFiles\stdthreadtest.dir\tests.cpp.i

CMakeFiles/stdthreadtest.dir/tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stdthreadtest.dir/tests.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\tests.cpp" -o CMakeFiles\stdthreadtest.dir\tests.cpp.s

# Object files for target stdthreadtest
stdthreadtest_OBJECTS = \
"CMakeFiles/stdthreadtest.dir/tests.cpp.obj"

# External object files for target stdthreadtest
stdthreadtest_EXTERNAL_OBJECTS =

stdthreadtest.exe: CMakeFiles/stdthreadtest.dir/tests.cpp.obj
stdthreadtest.exe: CMakeFiles/stdthreadtest.dir/build.make
stdthreadtest.exe: CMakeFiles/stdthreadtest.dir/linklibs.rsp
stdthreadtest.exe: CMakeFiles/stdthreadtest.dir/objects1.rsp
stdthreadtest.exe: CMakeFiles/stdthreadtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\cmake-build-release\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stdthreadtest.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\stdthreadtest.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stdthreadtest.dir/build: stdthreadtest.exe

.PHONY : CMakeFiles/stdthreadtest.dir/build

CMakeFiles/stdthreadtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\stdthreadtest.dir\cmake_clean.cmake
.PHONY : CMakeFiles/stdthreadtest.dir/clean

CMakeFiles/stdthreadtest.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests" "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests" "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\cmake-build-release" "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\cmake-build-release" "C:\Users\caleb\Project Files\Royal-Jaguar-3\include\mingw-std-threads-master\tests\cmake-build-release\CMakeFiles\stdthreadtest.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/stdthreadtest.dir/depend
