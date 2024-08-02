cmake_minimum_required(VERSION 3.5)

project(lidar_data_handle VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.12.12\\5.12.12\\mingw73_64\\")
set(QT_VERSION_MAJOR 6)
set(CMAKE_TOOLCHAIN_FILE "D:/bin/vcpkg/scripts/buildsystems/vcpkg.cmake")

# find_package(QT NAMES Qt5 COMPONENTS Core Widgets Network SerialPort PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Network SerialPort PrintSupport REQUIRED)

add_subdirectory("../sample_lib" "sample_lib")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        QCustomPlot/qcustomplot.cpp
        QCustomPlot/qcustomplot.h

        maininfo/maininfoform.h
        maininfo/maininfoform.ui
        maininfo/maininfoform.cpp

        maininfo/instanceinfoform.h
        maininfo/instanceinfoform.ui
        maininfo/instanceinfoform.cpp

        maininfo/paractrltool.h
        maininfo/paractrltool.ui
        maininfo/paractrltool.cpp

        # netclient/netclient.h
        # netclient/netclient.cpp

        datasub/datasubwidget.cpp
        datasub/datasubwidget.h
        datasub/datasubwidget.ui

        main.rc
        
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(lidar_data_handle
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET lidar_data_handle APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(lidar_data_handle SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(lidar_data_handle
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(lidar_data_handle PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::SerialPort 
    Qt${QT_VERSION_MAJOR}::Network 
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Sample_Obj
)

target_compile_definitions(lidar_data_handle PRIVATE QT_MESSAGELOGCONTEXT)
target_compile_definitions(Sample_Obj PRIVATE QT_MESSAGELOGCONTEXT)

set_target_properties(lidar_data_handle PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lidar_data_handle)
endif()
