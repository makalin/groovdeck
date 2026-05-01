cmake_minimum_required(VERSION 3.22)
project(GroovDeck VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(JUCE)

file(GLOB_RECURSE GroovDeck_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

juce_add_gui_app(GroovDeck
    PRODUCT_NAME "GroovDeck")

juce_generate_juce_header(GroovDeck)

target_sources(GroovDeck PRIVATE ${GroovDeck_SOURCES})

target_compile_definitions(GroovDeck
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:GroovDeck,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:GroovDeck,JUCE_VERSION>")

target_link_libraries(GroovDeck
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

set_target_properties(GroovDeck PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)
