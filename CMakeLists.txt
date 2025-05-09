cmake_minimum_required(VERSION 3.16)

project(Test3DVisual VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui 3DCore 3DExtras 3DRender 3DInput SerialPort)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(libs/QCustomPlot-library) # new shit
include_directories(libs/Eigen)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        my3dwindow.h
        my3dwindow.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Test3DVisual
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        my3dwindow.h my3dwindow.cpp
        Madgwick.h
        imucalibrator.h imucalibrator.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Test3DVisual APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Test3DVisual SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Test3DVisual
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Test3DVisual PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                            Qt${QT_VERSION_MAJOR}::Core
                                            Qt${QT_VERSION_MAJOR}::Gui
                                            Qt${QT_VERSION_MAJOR}::Widgets
                                            Qt${QT_VERSION_MAJOR}::3DCore
                                            Qt${QT_VERSION_MAJOR}::3DExtras
                                            Qt${QT_VERSION_MAJOR}::3DRender
                                            Qt${QT_VERSION_MAJOR}::3DInput
                                            Qt${QT_VERSION_MAJOR}::SerialPort)
target_link_libraries(Test3DVisual PRIVATE qcustomplot)
target_compile_definitions(Test3DVisual PRIVATE QCUSTOMPLOT_USE_LIBRARY)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Test3DVisual)
endif()
set_target_properties(Test3DVisual PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Test3DVisual
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Test3DVisual)
endif()
