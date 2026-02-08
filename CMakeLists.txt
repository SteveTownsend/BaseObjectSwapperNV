cmake_minimum_required(VERSION 3.21)
set(NAME "BaseObjectSwapper" CACHE STRING "")
set(VERSION 2.0.0 CACHE STRING "")
set(FalloutNVPath ${CMAKE_CURRENT_SOURCE_DIR}/Data)

# ---- Options ----

option(COPY_BUILD "Copy the build output to the Skyrim directory." TRUE)

# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(VCPKG_ROOT)

if (DEFINED VCPKG_ROOT)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TARGET_TRIPLET "x86-windows-static" CACHE STRING "")
else ()
	message(
		WARNING
		"Variable VCPKG_ROOT is not set. Continuing without vcpkg."
	)
endif ()

set(Boost_USE_STATIC_RUNTIME OFF CACHE BOOL "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")

# ---- Project ----

project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()

# ---- Globals ----

if (MSVC)
	if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
		add_compile_options(
			/MP	# Build with Multiple Processes
		)
	endif ()
endif ()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_STATIC_LIBS ON)

# ---- Dependencies ----

include(FetchContent)

FetchContent_Declare(
  unordered_dense
  GIT_REPOSITORY https://github.com/martinus/unordered_dense
  GIT_TAG        v4.8.1
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(unordered_dense)

find_package(unordered_dense CONFIG REQUIRED)
find_package(boost_container_hash CONFIG REQUIRED)

find_path(SRELL_INCLUDE_DIRS "srell.hpp")
find_path(CLIBUTIL_INCLUDE_DIRS "CLibUtil/string.hpp")

# ---- Add source files ----

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)
set (sources ${sources}
	main.cpp
	extern/NVSE/nvse/nvse/containers.cpp
	extern/NVSE/nvse/nvse/GameAPI.cpp
	extern/NVSE/nvse/nvse/GameBSExtraData.cpp
	extern/NVSE/nvse/nvse/GameData.cpp
	extern/NVSE/nvse/nvse/GameExtraData.cpp
	extern/NVSE/nvse/nvse/GameForms.cpp
	extern/NVSE/nvse/nvse/GameObjects.cpp
	extern/NVSE/nvse/nvse/GameOSDepend.cpp
	extern/NVSE/nvse/nvse/GameProcess.cpp
	extern/NVSE/nvse/nvse/GameRTTI.cpp
	extern/NVSE/nvse/nvse/GameScript.cpp
	extern/NVSE/nvse/nvse/GameSettings.cpp
	extern/NVSE/nvse/nvse/GameTasks.cpp
	extern/NVSE/nvse/nvse/GameTiles.cpp
	extern/NVSE/nvse/nvse/GameTypes.cpp
	extern/NVSE/nvse/nvse/NiTypes.cpp
	extern/NVSE/nvse/nvse/printf.cpp
	extern/NVSE/nvse/nvse/SafeWrite.cpp
	extern/NVSE/nvse/nvse/Utilities.cpp
	extern/NVSE/nvse/nvse/utility.cpp
	)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
		)

source_group(
	TREE
		${CMAKE_CURRENT_BINARY_DIR}
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

# ---- Create DLL ----

add_library(
	${PROJECT_NAME}
	SHARED
	${headers}
	${sources}
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
		_MBCS
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${SRELL_INCLUDE_DIRS}
		${CLIBUTIL_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/extern/NVSE/
		${CMAKE_CURRENT_SOURCE_DIR}/extern/NVSE/nvse
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		unordered_dense::unordered_dense
		Boost::container_hash
		winmm
)

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

if (MSVC)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			/sdl             # Enable Additional Security Checks
			/utf-8           # Set Source and Executable character sets to UTF-8
			/Zi              # Debug Information Format

			/permissive-     # Standards conformance
			/Zc:preprocessor # Enable preprocessor conformance mode

			/wd4200          # nonstandard extension used : zero-sized array in struct/union

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif ()

# ---- Post build ----

if (COPY_BUILD)
	if (DEFINED FalloutNVPath)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${FalloutNVPath}/NVSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${FalloutNVPath}/NVSE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable \${FalloutNVPath} is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
