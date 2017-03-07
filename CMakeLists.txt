cmake_minimum_required(VERSION 2.8)
project(pgm)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (HEADERS
    ${PROJECT_SOURCE_DIR}/include/pgm/pgm.h
    ${PROJECT_SOURCE_DIR}/include/pgm/variable.h
    ${PROJECT_SOURCE_DIR}/include/pgm/dataset.h
    ${PROJECT_SOURCE_DIR}/include/pgm/frequency.h
    ${PROJECT_SOURCE_DIR}/include/pgm/dgraph.h
    ${PROJECT_SOURCE_DIR}/include/pgm/bayesnet.h
    ${PROJECT_SOURCE_DIR}/include/pgm/sampleestimate.h
    ${PROJECT_SOURCE_DIR}/include/pgm/score/score.h
    ${PROJECT_SOURCE_DIR}/include/pgm/score/bdeu.h
)

set (SOURCES
    ${PROJECT_SOURCE_DIR}/src/pgm/variable.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/dataset.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/frequency.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/dgraph.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/bayesnet.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/sampleestimate.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/score/score.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/score/bdeu.cpp
)

include(CMakeToolsHelpers OPTIONAL)

include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_SOURCE_DIR}/include )

add_library (${PROJECT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

add_executable(sandbox "sandbox.cpp")
target_link_libraries (sandbox ${PROJECT_NAME})
