cmake_minimum_required(VERSION 2.8)
project(Sinsy)

set(PROJECT_VER_MAJOR 0)
set(PROJECT_VER_MINOR 9)
set(PROJECT_VER_PATCH 2)
set(PROJECT_VER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}.${PROJECT_VER_PATCH}")
set(PROJECT_APIVER "${PROJECT_VER_MAJOR}.${PROJECT_VER_MINOR}")

# set the default path for built executables to the "bin" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# set the default path for built libraries to the "lib" directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# find HTSEngine
# adapted from https://github.com/r9y9/open_jtalk/blob/1.10/src/CMakeLists.txt

add_subdirectory(lib/hts_engine_API/hts_engine/src)

configure_file(sinsy.pc.in "${PROJECT_BINARY_DIR}/sinsy.pc" @ONLY)

aux_source_directory(lib/converter converter_source)
aux_source_directory(lib/hts_engine_API hts_engine_API_source)
aux_source_directory(lib/japanese japanese_source)
aux_source_directory(lib/label label_source)
aux_source_directory(lib/score score_source)
aux_source_directory(lib/temporary temporary_source)
aux_source_directory(lib/util util_source)
aux_source_directory(lib/xml xml_source)

include_directories(sinsy
  include
  include/sinsy
  lib
  lib/converter
  lib/hts_engine_API/hts_engine/src/include
  lib/hts_engine_API
  lib/japanese
  lib/label
  lib/score
  lib/temporary
  lib/util
  lib/xml
  ${PROJECT_BINARY_DIR} # for generated headers
)

add_library(sinsy lib/Sinsy.cpp
  ${converter_source} ${hts_engine_API_source} ${japanese_source} ${label_source}
  ${score_source} ${temporary_source} ${util_source} ${xml_source})
set_target_properties(sinsy PROPERTIES
  VERSION ${PROJECT_VER}
  SOVERSION ${PROJECT_APIVER}
)

add_executable(sinsy-bin bin/sinsy.cpp)
target_link_libraries(sinsy hts_engine_API)
target_link_libraries(sinsy-bin sinsy)
if (WIN32)
target_link_libraries(sinsy-bin -static-libgcc -static-libstdc++)
endif()
set_target_properties(sinsy-bin PROPERTIES OUTPUT_NAME sinsy)

install(TARGETS sinsy sinsy-bin DESTINATION lib RUNTIME DESTINATION bin)
install(DIRECTORY include/sinsy DESTINATION include)
install(DIRECTORY dic DESTINATION lib/sinsy PATTERN "dic/Makefile*" EXCLUDE)
install(FILES "${PROJECT_BINARY_DIR}/sinsy.pc" DESTINATION lib/pkgconfig/)
