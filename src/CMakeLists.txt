#
# Compiler flags come from parent
#

#
#
# Build files configuration
#
#

#
#Source files for the library
#
file(GLOB_RECURSE LIB_VCD_PARSE_SOURCES libvcdparse/*.cpp)
file(GLOB_RECURSE LIB_VCD_PARSE_HEADERS libvcdparse/*.hpp)

set(LEXER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libvcdparse/vcd_lexer.l)
set(PARSER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/libvcdparse/vcd_parser.y)

#Include directories
foreach(header ${LIB_VCD_PARSE_HEADERS})
    get_filename_component(incl_dir ${header} DIRECTORY)
    list(APPEND LIB_VCD_PARSE_INCLUDE_DIRS ${incl_dir})
endforeach()
#Remove duplicate include directories
list(REMOVE_DUPLICATES LIB_VCD_PARSE_INCLUDE_DIRS)

#
#Source files for the demo executable
#
file(GLOB_RECURSE VCD_PARSE_DEMO_SOURCES vcdparse_demo/*.cpp)
file(GLOB_RECURSE VCD_PARSE_DEMO_HEADERS vcdparse_demo/*.hpp)

#Include directories
foreach(header ${VCD_PARSE_DEMO_HEADERS})
    get_filename_component(incl_dir ${header} DIRECTORY)
    list(APPEND VCD_PARSE_DEMO_INCLUDE_DIRS ${incl_dir})
endforeach()
#Remove duplicate include directories
#list(REMOVE_DUPLICATES VCD_PARSE_DEMO_INCLUDE_DIRS)

#
#
# Configure intermediate files
#
#

#Directory to put the generated files (avoid clogging up main build directory)
set(GEN_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen_files)
file(MAKE_DIRECTORY ${GEN_FILES_DIR})

#We use cmake to find the relevant executables
find_package(BISON REQUIRED 3.0)
find_package(FLEX REQUIRED)

#The flex/bison code is not warning clean so we need to suppress some warnings
set(FLEX_BISON_WARN_SUPPRESS_FLAGS " ")
set(FLEX_BISON_WARN_SUPPRESS_FLAGS_TO_CHECK
    "-Wno-switch-default"   #Flex/bison generate switch statments w/o default cases
    )
foreach(flag ${FLEX_BISON_WARN_SUPPRESS_FLAGS_TO_CHECK})
    CHECK_CXX_COMPILER_FLAG(${flag} COMPILER_SUPPORTS_${flag})
    if(COMPILER_SUPPORTS_${flag})
        #Flag supported, so enable it
        set(FLEX_BISON_WARN_SUPPRESS_FLAGS "${FLEX_BISON_WARN_SUPPRESS_FLAGS} ${flag}")
    endif()
endforeach(flag)

#
#
# Define the actual build targets
#
#

#
# Library
#

set(FLEX_GEN_SRC ${GEN_FILES_DIR}/vcd_lexer.gen.cpp)
set(FLEX_GEN_HDR ${GEN_FILES_DIR}/vcd_lexer.gen.hpp)
set(FLEX_GEN_OUTPUTS ${FLEX_GEN_SRC} ${FLEX_GEN_HDR})

set(BISON_GEN_SRC ${GEN_FILES_DIR}/vcd_parser.gen.cpp)
set(BISON_GEN_HDR ${GEN_FILES_DIR}/vcd_parser.gen.hpp)
set(BISON_GEN_OUTPUTS ${BISON_GEN_SRC} ${BISON_GEN_HDR})

#Generate the lexer
add_custom_command(OUTPUT ${FLEX_GEN_SRC} ${FLEX_GEN_HDR}
                   COMMAND ${FLEX_EXECUTABLE} --header-file=${FLEX_GEN_HDR} -o ${FLEX_GEN_SRC} ${LEXER_SRC}
                   DEPENDS ${LEXER_SRC} ${BISON_GEN_HDR})

#Generate the parser
add_custom_command(OUTPUT ${BISON_GEN_SRC} ${BISON_GEN_HDR}
                   COMMAND ${BISON_EXECUTABLE} --report=all -d -o ${BISON_GEN_SRC} ${PARSER_SRC}
                   DEPENDS ${PARSER_SRC})


#Apply suppression flags to the relevant files, must come after flex/bision
#targets for output vars to be defined
set_source_files_properties(${FLEX_GEN_OUTPUTS}
                            ${BISON_GEN_OUTPUTS}
                            PROPERTIES COMPILE_FLAGS ${FLEX_BISON_WARN_SUPPRESS_FLAGS})

#Indicate where to look for generated header files
include_directories(${GEN_FILES_DIR})

#Define the library
add_library(vcdparse STATIC ${LIB_VCD_PARSE_SOURCES} ${LIB_VCD_PARSE_HEADERS} ${FLEX_GEN_OUTPUTS} ${BISON_GEN_OUTPUTS})

#Export library headers
target_include_directories(vcdparse PUBLIC ${LIB_VCD_PARSE_INCLUDE_DIRS})


#
#The demo executable
#
add_executable(vcdparse_demo
               ${VCD_PARSE_DEMO_SOURCES}
               ${VCD_PARSE_DEMO_SOURCES})

target_include_directories(vcdparse_demo PRIVATE ${VCD_PARSE_DEMO_INCLUDE_DIRS})

target_link_libraries(vcdparse_demo vcdparse)
