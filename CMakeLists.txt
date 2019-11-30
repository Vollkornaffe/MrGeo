cmake_minimum_required(VERSION 3.3)
project(MrGeo)

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_library (MrGeo SHARED
        cpp_src/C_interface.cpp
)

target_link_libraries (MrGeo Eigen3::Eigen)

# -fvisibility=hidden
set_target_properties(MrGeo PROPERTIES CXX_VISIBILITY_PRESET hidden)

# this is a really ugly solution, but it should work
# the problem is that the binary code isn't reloaded after a change
# the workaround is to name the liraries different each time
if (NOT DEFINED CACHE{RELOAD_COUNTER})
  set(RELOAD_COUNTER "0" CACHE INTERNAL "")
else()
  math(EXPR RELOAD_COUNTER "${RELOAD_COUNTER} + 1")
  set(RELOAD_COUNTER ${RELOAD_COUNTER} CACHE INTERNAL "")
endif()
message("Reload counter at ${RELOAD_COUNTER}")
set_target_properties(MrGeo PROPERTIES SUFFIX ".so.${RELOAD_COUNTER}")

configure_file(python_src/MrGeo.py.in.py MrGeo.py @ONLY)

install(FILES
  ${CMAKE_SOURCE_DIR}/python_src/MrGeo_blender.py
  ${CMAKE_SOURCE_DIR}/python_src/MrGeo_interface.py

  ${CMAKE_BINARY_DIR}/MrGeo.py

  ${CMAKE_SOURCE_DIR}/arrow_mesh.stl
  ${CMAKE_SOURCE_DIR}/x_vertex_mesh.stl

  DESTINATION ${CMAKE_INSTALL_PREFIX}/MrGeo
)

install(TARGETS MrGeo DESTINATION ${CMAKE_INSTALL_PREFIX}/MrGeo)

# there are no tests yet
#add_subdirectory(test)
