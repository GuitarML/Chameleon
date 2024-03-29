cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")
project(Chameleon VERSION 1.2.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(modules)
include_directories(modules)

#juce_set_aax_sdk_path(C:/SDKs/AAX_SDK/)

set(JUCE_FORMATS AU VST3)

# Build LV2 only on Linux
if(UNIX AND NOT APPLE)
    message(STATUS "Building LV2 plugin format")
    list(APPEND JUCE_FORMATS LV2)
endif()

# Build AAX if SDK target exists
if(TARGET juce_aax_sdk)
    message(STATUS "Building AAX plugin format")
    list(APPEND JUCE_FORMATS AAX)
endif()

option(BUILD_RELEASE "Set build flags for release builds" OFF)
if(BUILD_RELEASE)
    set(HARDENED_RUNTIME_ENABLED YES)
else()
    set(HARDENED_RUNTIME_ENABLED NO)
endif()


juce_add_plugin(Chameleon
    COMPANY_NAME GuitarML
    PLUGIN_MANUFACTURER_CODE GtML
    PLUGIN_CODE Chm3 
    FORMATS ${JUCE_FORMATS}
    ProductName "Chameleon"
    LV2URI https://github.com/GuitarML/Chameleon
    LV2_SHARED_LIBRARY_NAME Chameleon
    ICON_BIG resources/logo.png

    AU_MAIN_TYPE kAudioUnitType_Effect
    AAX_CATEGORY AAX_ePlugInCategory_Harmonic

    MICROPHONE_PERMISSION_ENABLED TRUE
    HARDENED_RUNTIME_ENABLED ${HARDENED_RUNTIME_ENABLED}
)

# create JUCE header
juce_generate_juce_header(Chameleon)

# add sources
add_subdirectory(Source)
include_directories(Source)
add_subdirectory(resources)

target_compile_definitions(Chameleon
    PUBLIC
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Chameleon PUBLIC
    juce_plugin_modules
)
