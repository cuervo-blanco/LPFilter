# LPFilter

This project implements a stereo IIR low-pass filter audio plugin, based on 
Jan Wilczek's lesson on IIR filters at [Wolf Sound](https://thewolfsound.com/). 
For a deeper dive into audio processing, check out Jan's course. The plugin 
was created using Jan's [JUCE Audio Plugin Template](https://github.com/JanWilczek/audio-plugin-template), 
a great starting point for audio plugin development. A Python implementation of 
the IIR filter class can be found [here](https://github.com/cuervo-blanco/iir_filter).

The plugin processes audio using a dynamically adjustable IIR low-pass filter, 
with user controls for **cutoff frequency** (20 Hz to 20,000 Hz) and **Q factor** (0.1 to 10). 
A simple GUI with rotary sliders connects these parameters to real-time audio processing. 
The core of the plugin is an efficient `IIRFilter` class that manages filter state 
and applies the filter equation, with coefficients dynamically calculated based on user input. 

To use, clone the repository, build the project with JUCE or CMake, and load the plugin into a DAW. 
Adjust sliders to modify the filter settings. For further exploration or enhancements, 
refer to the JUCE documentation and Jan Wilczek's resources.

