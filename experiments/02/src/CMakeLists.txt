cmake_minimum_required(VERSION 3.16)

project(Graph-Visual VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES Graph-Visual_zh_CN.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.ui
    ${TS_FILES}
)

set(PROJECT_HEADERS
    defines.hpp
    indexGraph.hpp
    components.hpp
    mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Graph-Visual
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
    qt_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Graph-Visual SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS})
    else()
        add_executable(Graph-Visual ${PROJECT_SOURCES} ${PROJECT_HEADERS})
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
endif()

# 部署目标: windeployqt自动部署Qt依赖到exe同目录
add_custom_target(deploy
    DEPENDS Graph-Visual
    COMMAND windeployqt.exe --no-translations "$<TARGET_FILE:Graph-Visual>"
    COMMENT "部署: windeployqt自动复制Qt依赖到exe同目录"
)

target_link_libraries(Graph-Visual PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(Graph-Visual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Graph-Visual)
endif()
set_target_properties(Graph-Visual PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
# 构建后自动部署Qt DLLs
if(WIN32)
    get_target_property(_qt_qmake Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    if(_qt_qmake)
        get_filename_component(_qt_bin "${_qt_qmake}" DIRECTORY)
        add_custom_command(TARGET Graph-Visual POST_BUILD
            COMMAND "${_qt_bin}/windeployqt.exe" --no-translations "$<TARGET_FILE:Graph-Visual>"
            COMMENT "部署Qt DLL到exe目录"
        )
    endif()
endif()

install(TARGETS Graph-Visual
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Graph-Visual)
endif()
