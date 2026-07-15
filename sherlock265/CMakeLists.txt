find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
  message(STATUS "Qt6 not found, skipping sherlock265")
  return()
endif()

set(CMAKE_AUTOMOC ON)

add_executable(sherlock265
  sherlock265.cc
  VideoPlayer.cc
  VideoDecoder.cc
  VideoWidget.cc
  VideoPlayer.h
  VideoDecoder.h
  VideoWidget.h
)

target_link_libraries(sherlock265 PRIVATE
  de265
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Threads::Threads
)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(SWSCALE IMPORTED_TARGET libswscale)
  if(SWSCALE_FOUND)
    target_compile_definitions(sherlock265 PRIVATE HAVE_SWSCALE)
    target_link_libraries(sherlock265 PRIVATE PkgConfig::SWSCALE)
  endif()
endif()

install(TARGETS sherlock265 DESTINATION ${CMAKE_INSTALL_BINDIR})
