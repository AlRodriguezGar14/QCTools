# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.29.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.29.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/albertorodriguez/projects/QC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/albertorodriguez/projects/QC

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/opt/homebrew/Cellar/cmake/3.29.0/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/opt/homebrew/Cellar/cmake/3.29.0/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/albertorodriguez/projects/QC/CMakeFiles /Users/albertorodriguez/projects/QC//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/albertorodriguez/projects/QC/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named qctool

# Build rule for target.
qctool: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 qctool
.PHONY : qctool

# fast build rule for target.
qctool/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/build
.PHONY : qctool/fast

InactivePixelsTracker.o: InactivePixelsTracker.cpp.o
.PHONY : InactivePixelsTracker.o

# target to build an object file
InactivePixelsTracker.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/InactivePixelsTracker.cpp.o
.PHONY : InactivePixelsTracker.cpp.o

InactivePixelsTracker.i: InactivePixelsTracker.cpp.i
.PHONY : InactivePixelsTracker.i

# target to preprocess a source file
InactivePixelsTracker.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/InactivePixelsTracker.cpp.i
.PHONY : InactivePixelsTracker.cpp.i

InactivePixelsTracker.s: InactivePixelsTracker.cpp.s
.PHONY : InactivePixelsTracker.s

# target to generate assembly for a file
InactivePixelsTracker.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/InactivePixelsTracker.cpp.s
.PHONY : InactivePixelsTracker.cpp.s

converter.o: converter.cpp.o
.PHONY : converter.o

# target to build an object file
converter.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/converter.cpp.o
.PHONY : converter.cpp.o

converter.i: converter.cpp.i
.PHONY : converter.i

# target to preprocess a source file
converter.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/converter.cpp.i
.PHONY : converter.cpp.i

converter.s: converter.cpp.s
.PHONY : converter.s

# target to generate assembly for a file
converter.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/converter.cpp.s
.PHONY : converter.cpp.s

duplicateFramesTracker.o: duplicateFramesTracker.cpp.o
.PHONY : duplicateFramesTracker.o

# target to build an object file
duplicateFramesTracker.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/duplicateFramesTracker.cpp.o
.PHONY : duplicateFramesTracker.cpp.o

duplicateFramesTracker.i: duplicateFramesTracker.cpp.i
.PHONY : duplicateFramesTracker.i

# target to preprocess a source file
duplicateFramesTracker.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/duplicateFramesTracker.cpp.i
.PHONY : duplicateFramesTracker.cpp.i

duplicateFramesTracker.s: duplicateFramesTracker.cpp.s
.PHONY : duplicateFramesTracker.s

# target to generate assembly for a file
duplicateFramesTracker.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/duplicateFramesTracker.cpp.s
.PHONY : duplicateFramesTracker.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/qctool.dir/build.make CMakeFiles/qctool.dir/main.cpp.s
.PHONY : main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... qctool"
	@echo "... InactivePixelsTracker.o"
	@echo "... InactivePixelsTracker.i"
	@echo "... InactivePixelsTracker.s"
	@echo "... converter.o"
	@echo "... converter.i"
	@echo "... converter.s"
	@echo "... duplicateFramesTracker.o"
	@echo "... duplicateFramesTracker.i"
	@echo "... duplicateFramesTracker.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

