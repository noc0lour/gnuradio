# Copyright 2010-2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

if(DEFINED __INCLUDED_GR_SWIG_CMAKE)
    return()
endif()
set(__INCLUDED_GR_SWIG_CMAKE TRUE)

include(GrPython)

########################################################################
# Builds a swig documentation file to be generated into python docstrings
# Usage: GR_SWIG_MAKE_DOCS(output_file input_path input_path....)
#
# Set the following variable to specify extra dependent targets:
#   - GR_SWIG_DOCS_SOURCE_DEPS
#   - GR_SWIG_DOCS_TARGET_DEPS
########################################################################
function(GR_SWIG_MAKE_DOCS output_file)
    if(ENABLE_DOXYGEN)

        #setup the input files variable list, quote formatted
        set(input_files)
        unset(INPUT_PATHS)
        foreach(input_path ${ARGN})
            if(IS_DIRECTORY ${input_path}) #when input path is a directory
                file(GLOB input_path_h_files ${input_path}/*.h)
            else() #otherwise its just a file, no glob
                set(input_path_h_files ${input_path})
            endif()
            list(APPEND input_files ${input_path_h_files})
            set(INPUT_PATHS "${INPUT_PATHS} \"${input_path}\"")
        endforeach(input_path)

        #determine the output directory
        get_filename_component(name ${output_file} NAME_WE)
        get_filename_component(OUTPUT_DIRECTORY ${output_file} PATH)
        set(OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}/${name}_swig_docs)
        make_directory(${OUTPUT_DIRECTORY})

        #generate the Doxyfile used by doxygen
        configure_file(
            ${CMAKE_SOURCE_DIR}/docs/doxygen/Doxyfile.swig_doc.in
            ${OUTPUT_DIRECTORY}/Doxyfile
        @ONLY)

        #Create a dummy custom command that depends on other targets
        include(GrMiscUtils)
        GR_GEN_TARGET_DEPS(_${name}_tag tag_deps ${GR_SWIG_DOCS_TARGET_DEPS})

        #call doxygen on the Doxyfile + input headers
        add_custom_command(
            OUTPUT ${OUTPUT_DIRECTORY}/xml/index.xml
            DEPENDS ${input_files} ${GR_SWIG_DOCS_SOURCE_DEPS} ${tag_deps}
            COMMAND ${DOXYGEN_EXECUTABLE} ${OUTPUT_DIRECTORY}/Doxyfile
            COMMENT "Generating doxygen xml for ${name} docs"
        )

        #call the swig_doc script on the xml files
        add_custom_command(
            OUTPUT ${output_file}
            DEPENDS ${input_files} ${stamp-file} ${OUTPUT_DIRECTORY}/xml/index.xml
            COMMAND ${PYTHON_EXECUTABLE} -B
                ${CMAKE_SOURCE_DIR}/docs/doxygen/swig_doc.py
                ${OUTPUT_DIRECTORY}/xml
                ${output_file}
            COMMENT "Generating python docstrings for ${name}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs/doxygen
        )

    else(ENABLE_DOXYGEN)
        file(WRITE ${output_file} "\n") #no doxygen -> empty file
    endif(ENABLE_DOXYGEN)
endfunction(GR_SWIG_MAKE_DOCS)

########################################################################
# Build a swig target for the common gnuradio use case. Usage:
# GR_SWIG_MAKE(target ifile ifile ifile...)
#
# Set the following variables before calling:
#   - GR_SWIG_FLAGS
#   - GR_SWIG_INCLUDE_DIRS
#   - GR_SWIG_LIBRARIES
#   - GR_SWIG_SOURCE_DEPS
#   - GR_SWIG_TARGET_DEPS
#   - GR_SWIG_DOC_FILE
#   - GR_SWIG_DOC_DIRS
########################################################################
macro(GR_SWIG_MAKE name)
    set(ifiles ${ARGN})

    #do swig doc generation if specified
    if(GR_SWIG_DOC_FILE)
        set(GR_SWIG_DOCS_SOURCE_DEPS ${GR_SWIG_SOURCE_DEPS})
        list(APPEND GR_SWIG_DOCS_TARGET_DEPS ${GR_SWIG_TARGET_DEPS})
        GR_SWIG_MAKE_DOCS(${GR_SWIG_DOC_FILE} ${GR_SWIG_DOC_DIRS})
        add_custom_target(${name}_swig_doc DEPENDS ${GR_SWIG_DOC_FILE})
        list(APPEND GR_SWIG_TARGET_DEPS ${name}_swig_doc)
    endif()

    #append additional include directories
    list(APPEND GR_SWIG_INCLUDE_DIRS ${PYTHON_INCLUDE_PATH}) #deprecated name (now dirs)
    list(APPEND GR_SWIG_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})

    #prepend local swig directories
    list(INSERT GR_SWIG_INCLUDE_DIRS 0 ${CMAKE_CURRENT_SOURCE_DIR})
    list(INSERT GR_SWIG_INCLUDE_DIRS 0 ${CMAKE_CURRENT_BINARY_DIR})

    #determine include dependencies for swig file
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE}
            ${CMAKE_BINARY_DIR}/get_swig_deps.py
            "${ifiles}" "${GR_SWIG_INCLUDE_DIRS}"
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE SWIG_MODULE_${name}_EXTRA_DEPS
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    #Create a dummy custom command that depends on other targets
    include(GrMiscUtils)
    GR_GEN_TARGET_DEPS(_${name}_swig_tag tag_deps ${GR_SWIG_TARGET_DEPS})
    set(tag_file ${CMAKE_CURRENT_BINARY_DIR}/${name}.tag)
    add_custom_command(
        OUTPUT ${tag_file}
        DEPENDS ${GR_SWIG_SOURCE_DEPS} ${tag_deps}
        COMMAND ${CMAKE_COMMAND} -E touch ${tag_file}
    )

    #append the specified include directories
    list(APPEND SWIG_MODULE_${name}_EXTRA_DEPS ${tag_file})

    include_directories(${GR_SWIG_INCLUDE_DIRS})
    if (PYTHON3)
        set(py3 "-py3")
    endif (PYTHON3)

    #setup the swig flags with flags and include directories
    set(CMAKE_SWIG_FLAGS -fvirtual -modern -keyword -w511 -w314 -relativeimport ${py3} -module ${name} ${GR_SWIG_FLAGS})

    #set the C++ property on the swig .i file so it builds
    set_source_files_properties(${ifiles} PROPERTIES CPLUSPLUS ON)

    #setup the actual swig library target to be built
    include(UseSWIG)
    SWIG_ADD_LIBRARY(${name} LANGUAGE python SOURCES ${ifiles})
    if(APPLE)
      set(PYTHON_LINK_OPTIONS "-undefined dynamic_lookup")
    else()
      set(PYTHON_LINK_OPTIONS ${PYTHON_LIBRARIES})
    endif(APPLE)
    if(${name} STREQUAL "runtime_swig")
        SET_TARGET_PROPERTIES(runtime_swig PROPERTIES DEFINE_SYMBOL "gnuradio_runtime_EXPORTS")
    endif(${name} STREQUAL "runtime_swig")
    target_link_libraries(${name} ${PYTHON_LINK_OPTIONS} ${GR_SWIG_LIBRARIES})
endmacro(GR_SWIG_MAKE)

########################################################################
# Install swig targets generated by GR_SWIG_MAKE. Usage:
# GR_SWIG_INSTALL(
#   TARGETS target target target...
#   [DESTINATION destination]
# )
########################################################################
macro(GR_SWIG_INSTALL)

    include(CMakeParseArgumentsCopy)
    CMAKE_PARSE_ARGUMENTS(GR_SWIG_INSTALL "" "DESTINATION" "TARGETS" ${ARGN})

    foreach(name ${GR_SWIG_INSTALL_TARGETS})
        install(TARGETS ${name}
            DESTINATION ${GR_SWIG_INSTALL_DESTINATION}
        )

        include(GrPython)
        GR_PYTHON_INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${name}.py
          DESTINATION ${GR_SWIG_INSTALL_DESTINATION}
          DEPENDS ${name}
        )

        GR_LIBTOOL(
            TARGET ${name}
            DESTINATION ${GR_SWIG_INSTALL_DESTINATION}
        )

    endforeach(name)

endmacro(GR_SWIG_INSTALL)

########################################################################
# Generate a python file that can determine swig dependencies.
# Used by the make macro above to determine extra dependencies.
# When you build C++, CMake figures out the header dependencies.
# This code essentially performs that logic for swig includes.
########################################################################
file(WRITE ${CMAKE_BINARY_DIR}/get_swig_deps.py "

import os, sys, re, io

i_include_matcher = re.compile(r'%(include|import)\\s*[<|\"](.*)[>|\"]')
h_include_matcher = re.compile(r'#(include)\\s*[<|\"](.*)[>|\"]')
include_dirs = sys.argv[2].split(';')

def get_swig_incs(file_path):
    if file_path.endswith('.i'): matcher = i_include_matcher
    else: matcher = h_include_matcher
    file_contents = io.open(file_path, 'r', encoding='utf-8').read()
    return matcher.findall(file_contents, re.MULTILINE)

def get_swig_deps(file_path, level):
    deps = [file_path]
    if level == 0: return deps
    for keyword, inc_file in get_swig_incs(file_path):
        for inc_dir in include_dirs:
            inc_path = os.path.join(inc_dir, inc_file)
            if not os.path.exists(inc_path): continue
            deps.extend(get_swig_deps(inc_path, level-1))
            break #found, we don't search in lower prio inc dirs
    return deps

if __name__ == '__main__':
    ifiles = sys.argv[1].split(';')
    deps = sum([get_swig_deps(ifile, 3) for ifile in ifiles], [])
    #sys.stderr.write(';'.join(set(deps)) + '\\n\\n')
    print(';'.join(set(deps)))
")
