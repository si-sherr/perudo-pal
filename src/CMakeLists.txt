find_package(Git)
set(VERSION_DIR ${CMAKE_CURRENT_LIST_DIR}/version)
add_custom_target(
  version
  ${CMAKE_COMMAND}
  -D
  SRC=${VERSION_DIR}/ppal_version.h.in
  -D
  DST=${VERSION_DIR}/ppal_version.h
  -D
  GIT_EXECUTABLE=${GIT_EXECUTABLE}
  -P
  ${VERSION_DIR}/generate_version_header.cmake)

add_subdirectory(core)
add_subdirectory(cli)
