
cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

# ---------------------------------------------------------
#                      User Config
# ---------------------------------------------------------


# Most include and library paths are automatically found by
# cmake. Should it fail to find something, in this section it
# is possible to set useful paths according to the current 
# environment, which will be passed on to cmake as hints.

list(APPEND MY_INCLUDE_DIRS
    
    "/usr/local/include"

)

list(APPEND MY_LIBRARY_x64_RELEASE_DIRS

    "/usr/local/lib"
    "/usr/local/lib/7"
)

list(APPEND MY_LIBRARY_x64_DEBUG_DIRS

    "/usr/local/lib"
    "/usr/local/lib/7"
)

# ---------------------------------------------------------
#                  Build Configuration
# ---------------------------------------------------------

# Default settings
# ----------------

if(NOT BINARY_TYPE)
  set(BINARY_TYPE dynamic)
endif()

if(NOT BUILD_MODE)
  set(BUILD_MODE release)
endif()

if(NOT ARCH)
   set(ARCH x64)
endif()

if(NOT DATASTORE_T)
   set(DATASTORE_T "TCDataStore")
endif()


# Parameters check
# ----------------

if(BUILD_MODE STREQUAL "debug")
   set(CMAKE_BUILD_TYPE DEBUG)
elseif(BUILD_MODE STREQUAL "release")
   set(CMAKE_BUILD_TYPE RELEASE)
else()
   message(FATAL_ERROR 
       "\nERROR: Invalid build mode: ${BUILD_MODE}. Use BUILD_MODE=debug|release"
   )
endif()

if(NOT ARCH STREQUAL "x32" AND NOT ARCH STREQUAL "x64")
   message(FATAL_ERROR 
       "\nERROR: Invalid architecture: ${ARCH}. Use ARCH=x32|x64"
   )
endif()

if(DATASTORE_T STREQUAL "TCDataStore")
   # On Windows some libraries have different namings.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      set(DATASTORE_LIB_NAME libejdb)
   else()
      set(DATASTORE_LIB_NAME tokyocabinet)
   endif()
   add_definitions(-DDATASTORE_T_ID=1)
   add_definitions(-DDATASTORE_T=${DATASTORE_T})
elseif(DATASTORE_T STREQUAL "CBDataStore")
   # On Windows some libraries have different namings.
   if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      set(DATASTORE_LIB_NAME libcouchbase)
   else()
      set(DATASTORE_LIB_NAME couchbase)
   endif()
   add_definitions(-DDATASTORE_T_ID=2)
   add_definitions(-DDATASTORE_T=${DATASTORE_T})   
else()
   message(FATAL_ERROR 
           "\nERROR: Unsupported data store: ${DATASTORE_T}.")
endif()

# ---------------------------------------------------------
#                     Examples Module
# ---------------------------------------------------------

message(STATUS "Building examples")

include_directories("${AX_SRC_ROOT}/src"
                    "${AX_SRC_ROOT}/audio" 
                    "${AX_SRC_ROOT}/DAO"
                    "${AX_SRC_ROOT}/tools")

# --- Check module-specific parameters ---

# ...

# --- Platform-specific module compilation settings ---

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

   set(APP_CXX_FLAGS "")
   set(THREAD_LIB pthread)
   
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   
   set(APP_CXX_FLAGS "")
   
endif()

# --- Modules definitions ---

set(EXAMPLE1_SRCS example1.cpp 
                  ${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp 
                  ${AX_SRC_ROOT}/audio/AudioSource.cpp)

set(EXAMPLE2_SRCS example2.cpp 
                  ${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp)

set(EXAMPLE3_SRCS example3.cpp 
                  ${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp 
                  ${AX_SRC_ROOT}/audio/AudioSource.cpp)

set(EXAMPLE4_SRCS example4.cpp 
                  ${AX_SRC_ROOT}/DAO/${DATASTORE_T}.cpp 
                  ${AX_SRC_ROOT}/audio/AudioSource.cpp)

# --- Check for required libraries ---

find_library(DATASTORE_LIB
             NAMES ${DATASTORE_LIB_NAME}
             PATHS ${MY_LIBRARY_${ARCH}_${CMAKE_BUILD_TYPE}_DIRS})
			 
if(WITH_ID3)
find_library(ID3TAG_LIB
             NAMES ${ID3TAG_LIB_NAME} 
             PATHS ${MY_LIBRARY_${ARCH}_${CMAKE_BUILD_TYPE}_DIRS})
endif()

# --- Build the modules ---

add_executable(example1 ${EXAMPLE1_SRCS})
target_compile_options(example1 PRIVATE "${APP_CXX_FLAGS}")
target_link_libraries(example1 audioneex
                      ${Boost_LIBRARIES} ${THREAD_LIB}
                      ${DATASTORE_LIB} ${ID3TAG_LIB})

add_executable(example2 ${EXAMPLE2_SRCS})
target_compile_options(example2 PRIVATE "${APP_CXX_FLAGS}")
target_link_libraries(example2 audioneex
                      ${Boost_LIBRARIES}
                      ${DATASTORE_LIB} ${ID3TAG_LIB})

add_executable(example3 ${EXAMPLE3_SRCS})
target_compile_options(example3 PRIVATE "${APP_CXX_FLAGS}")
target_link_libraries(example3 audioneex
                      ${Boost_LIBRARIES} ${THREAD_LIB}
                      ${DATASTORE_LIB} ${ID3TAG_LIB})

add_executable(example4 ${EXAMPLE4_SRCS})
target_compile_options(example4 PRIVATE "${APP_CXX_FLAGS}")
target_link_libraries(example4 audioneex
                      ${Boost_LIBRARIES} ${THREAD_LIB}
                      ${DATASTORE_LIB} ${ID3TAG_LIB})

foreach(EXAMPLE example1 example2 example3 example4)
    set_target_properties(${EXAMPLE} 
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${AX_OUTPUT_EXE_DIR}/examples"
    )
endforeach()
