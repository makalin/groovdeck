cmake_minimum_required(VERSION 3.15)
project(GroovDeck VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE
find_package(JUCE REQUIRED)

# Add JUCE modules
juce_add_module(JUCE/modules/juce_audio_basics)
juce_add_module(JUCE/modules/juce_audio_devices)
juce_add_module(JUCE/modules/juce_audio_formats)
juce_add_module(JUCE/modules/juce_audio_processors)
juce_add_module(JUCE/modules/juce_audio_utils)
juce_add_module(JUCE/modules/juce_core)
juce_add_module(JUCE/modules/juce_data_structures)
juce_add_module(JUCE/modules/juce_events)
juce_add_module(JUCE/modules/juce_graphics)
juce_add_module(JUCE/modules/juce_gui_basics)
juce_add_module(JUCE/modules/juce_gui_extra)

# Add source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# Create executable
juce_add_gui_app(GroovDeck
    SOURCES ${SOURCES}
)

# Link JUCE modules
target_link_libraries(GroovDeck
    PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Set properties for the target
set_target_properties(GroovDeck PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
) 