cmake_minimum_required(VERSION 3.5)

project(vr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    optiondialog.cpp
    optiondialog.h
    optiondialog.ui
    VRRenderThread.h
    VRRenderThread.cpp
)

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

qt_add_executable(vr
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ModelPart.h ModelPartList.h
        ModelPart.cpp ModelPartList.cpp
        optiondialog.h optiondialog.cpp optiondialog.ui
        icons.qrc
)

target_link_libraries(vr PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

# Set properties for macOS bundle
if(APPLE)
    set_target_properties(vr PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.vr"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    )
endif()

#installer
set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "vr")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VR APPLICATION")
set(CPACK_PACKAGE_VENDOR "2023_GROUP_7")
set(CPACK_PACKAGE_EXECUTABLES "vr;vr")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "vr")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "vr")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_DISPLAY_NAME "vr")
set(CPACK_NSIS_CREATE_UNINSTALLER "ON")
set(CPACK_NSIS_MENU_LINKS "${CPACK_NSIS_EXECUTABLES_DIRECTORY}/vrApp.exe" "vrApp" "Uninstall.exe" "Uninstall vrApp")
set(CPACK_NSIS_MODIFY_PATH ON)

# Qt deployment
if(WIN32)
    get_filename_component(QT_BIN_DIR "${QT_DIR}/bin" ABSOLUTE)
    set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/deploy")
    file(MAKE_DIRECTORY ${DEPLOY_DIR})
    file(GLOB QT_DLLS "${QT_BIN_DIR}/*.dll")
    file(COPY ${QT_DLLS} DESTINATION ${DEPLOY_DIR})
    file(GLOB QT_PLUGINS "${QT_BIN_DIR}/plugins/platforms/*.dll")
    file(COPY ${QT_PLUGINS} DESTINATION ${DEPLOY_DIR}/platforms)
    set(ENV{QT_QPA_PLATFORM_PLUGIN_PATH} "${DEPLOY_DIR}/platforms")
endif()

# Windows-specific deployment
include(CPack)

# Specify the target and its type
install(TARGETS vr RUNTIME DESTINATION .
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the VRBindings
install(DIRECTORY ${CMAKE_BINARY_DIR}/vrbindings/
    DESTINATION ${CMAKE_INSTALL_BINDIR}/vrbindings
    COMPONENT headers
)

add_custom_command(TARGET vr POST_BUILD
    COMMAND "${WINDEPLOYQT_EXECUTABLE}"
    --verbose 0
    --no-compiler-runtime
    --no-opengl-sw
    --dir "${CMAKE_BINARY_DIR}/windeployqt"
    "$<TARGET_FILE:vr>"
)

install(DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/" DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install VTK DLLs
install(FILES $<TARGET_RUNTIME_DLLS:vr>
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT RuntimeLibraries
)

# Install OpenVR DLLs
install(FILES "${OpenVR_INCLUDE_DIR}/../bin/win64/openvr_api.dll"
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT RuntimeLibraries
)

cpack_add_component(libraries)
cpack_add_component(headers)