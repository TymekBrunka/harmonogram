include(deps/CPM.cmake)
set(CPM_USE_LOCAL_PACKAGES ON)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  message("Ccache enabled")
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
endif()

# rendering
message(glfw)
CPMAddPackage(
  NAME glfw
  VERSION 3.4
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.4
  OPTIONS
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_DOCS OFF"
)

set(CPM_USE_LOCAL_PACKAGES OFF)
message(json)
CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.12.0
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.12.0
)
