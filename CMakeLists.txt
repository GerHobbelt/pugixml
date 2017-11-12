include(CMakePackageConfigHelpers)

project(pugixml)

cmake_minimum_required(VERSION 2.8.12)

option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_PKGCONFIG "Build in PKGCONFIG mode" OFF)

set(BUILD_DEFINES "" CACHE STRING "Build defines")

if(MSVC)
	option(STATIC_CRT "Use static CRT libraries" OFF)

	# Rewrite command line flags to use /MT if necessary
	if(STATIC_CRT)
		foreach(flag_var
				CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif(${flag_var} MATCHES "/MD")
		endforeach(flag_var)
	endif()
endif()

# Pre-defines standard install locations on *nix systems.
include(GNUInstallDirs)
mark_as_advanced(CLEAR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)

set(HEADERS src/pugixml.hpp src/pugiconfig.hpp)
set(SOURCES ${HEADERS} src/pugixml.cpp)

if(DEFINED BUILD_DEFINES)
	foreach(DEFINE ${BUILD_DEFINES})
		add_definitions("-D" ${DEFINE})
	endforeach()
endif()

if(BUILD_SHARED_LIBS)
	add_library(pugixml SHARED ${SOURCES})
else()
	add_library(pugixml STATIC ${SOURCES})
endif()

# Set version
#
# To update the version:
#		1. Change the major, minor and patch below
#   2. Change the mahor, minor and patch in pugixml.hpp
#
set(PUGIXML_VERSION_MAJOR "1")
set(PUGIXML_VERSION_MINOR "8")
set(PUGIXML_VERSION_PATCH "2")

set(PUGIXML_VERSION "${PUGIXML_VERSION_MAJOR}.${PUGIXML_VERSION_MINOR}.${PUGIXML_VERSION_PATCH}")
set(PUGIXML_VERSION_SOVERSION "${PUGIXML_VERSION_MAJOR}")

set(version_file "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake/pugixml/pugixml-config-version.cmake")
write_basic_package_version_file(${version_file} VERSION ${PUGIXML_VERSION} COMPATIBILITY AnyNewerVersion)
install(FILES ${version_file} DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pugixml)

# Enable C++11 long long for compilers that are capable of it
if(NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} STRLESS 3.1 AND ";${CMAKE_CXX_COMPILE_FEATURES};" MATCHES ";cxx_long_long_type;")
	target_compile_features(pugixml PUBLIC cxx_long_long_type)
endif()

set_target_properties(pugixml PROPERTIES VERSION ${PUGIXML_VERSION} SOVERSION ${PUGIXML_VERSION_SOVERSION})
get_target_property(PUGIXML_VERSION_STRING pugixml VERSION)

if(BUILD_PKGCONFIG)
	# Install library into its own directory under LIBDIR
	set(INSTALL_SUFFIX /pugixml-${PUGIXML_VERSION_STRING})
endif()

target_include_directories(pugixml PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}${INSTALL_SUFFIX}>)

install(TARGETS pugixml EXPORT pugixml-config
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}${INSTALL_SUFFIX})
install(EXPORT pugixml-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pugixml)

if(BUILD_PKGCONFIG)
	configure_file(scripts/pugixml.pc.in ${PROJECT_BINARY_DIR}/pugixml.pc @ONLY)
	install(FILES ${PROJECT_BINARY_DIR}/pugixml.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
endif()

if(BUILD_TESTS)
	file(GLOB TEST_SOURCES tests/*.cpp)
	file(GLOB FUZZ_SOURCES tests/fuzz_*.cpp)
	list(REMOVE_ITEM TEST_SOURCES ${FUZZ_SOURCES})

	add_executable(check ${TEST_SOURCES})
	target_link_libraries(check pugixml)
	add_custom_command(TARGET check POST_BUILD COMMAND check WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
