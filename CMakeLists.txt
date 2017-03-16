cmake_minimum_required(VERSION 2.8)
project(pgm)
include(ExternalProject)
find_package(Git REQUIRED)

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
    ${PROJECT_SOURCE_DIR}/include/pgm/score/loglikelihood.h
    ${PROJECT_SOURCE_DIR}/include/pgm/score/aic.h
    ${PROJECT_SOURCE_DIR}/include/pgm/score/bic.h
    ${PROJECT_SOURCE_DIR}/include/pgm/score/fcll.h
    ${PROJECT_SOURCE_DIR}/include/pgm/search/greedyhillclimbing.h
    ${PROJECT_SOURCE_DIR}/include/pgm/search/simulatedannealing.h
    ${PROJECT_SOURCE_DIR}/include/pgm/utility.h
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
    ${PROJECT_SOURCE_DIR}/src/pgm/score/loglikelihood.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/score/aic.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/score/bic.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/score/fcll.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/search/greedyhillclimbing.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/search/simulatedannealing.cpp
    ${PROJECT_SOURCE_DIR}/src/pgm/utility.cpp
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

# Unit test
ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/single_include CACHE INTERNAL "Path to include folder for Catch")

# Includes Catch in the project:
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)  # Enables unit-testing.

set(TEST
    ${PROJECT_SOURCE_DIR}/test/pgm_test.cpp
    ${PROJECT_SOURCE_DIR}/test/frequency_test.cpp
)
add_executable(pgm_test ${TEST})
target_link_libraries(pgm_test ${PROJECT_NAME})
add_test(pgm_test pgm_test)