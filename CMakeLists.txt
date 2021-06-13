cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")
project(Chameleon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

set(RTNEURAL_XSIMD ON CACHE BOOL "Use RTNeural with this backend" FORCE)
add_subdirectory(modules/RTNeural)

add_subdirectory(modules)
include_directories(modules)


juce_add_plugin(Chameleon
    COMPANY_NAME GuitarML
    PLUGIN_MANUFACTURER_CODE GML
    PLUGIN_CODE spg3 
    FORMATS AU VST3 Standalone LV2
    ProductName "Chameleon"
    LV2_URI https://github.com/GuitarML/Chameleon
    ICON_BIG resources/logo.png
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

# we need these flags for notarization on MacOS
option(MACOS_RELEASE "Set build flags for MacOS Release" OFF)
if(MACOS_RELEASE)
    message(STATUS "Setting MacOS release flags...")
    set_target_properties(Chameleon_Standalone PROPERTIES
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME YES)
endif()