cmake_minimum_required(VERSION 3.22)

project(IIRFilter VERSION 0.1.0)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/IIRFilter")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs")

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME TriCerebrado
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE TRIC
    PLUGIN_CODE IIRF
    FORMATS Standalone VST3
    PRODUCT_NAME "IIRFilter"
)

juce_generate_juce_header(IIRFilter)

target_sources(${PROJECT_NAME}
    PRIVATE
        source/PluginEditor.cpp
        source/PluginProcessor.cpp
        ${INCLUDE_DIR}/PluginEditor.h
        ${INCLUDE_DIR}/PluginProcessor.h
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LIB_DIR}/juce/modules
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_audio_basics
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


