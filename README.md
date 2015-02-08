# GCHQ
GCHQ is a free, high-quality reverb built with the JUCE libraries (http://www.juce.com/)

GCHQ is a high-quality impulse response filter which uses impulse response data from various famous landmarks found on the OpenAir project (http://www.openairlib.net/). 

GCHQ was programmed in roughly one day as part of a job application and therefore there are several **limitations** (see Limitations).

Building
========

GCHQ is a JUCE project and therefore requires the Introjucer to create the necessary build files. You can find the Introjucer in the juce SDK available at http://www.juce.com/

GCHQ requires the single precision version of the fftw FFT libraries (http://www.fftw.org/) when linking. For optimal portability, it is best to supply the 'libfftw3f.a' library as a static link library in the appropriate target folder in the Builds folder.

1. Launch the Introjucer and open the GCHQ jucer file: Introjucer ./GCHQ.jucer
2. Click on 'Config' -> 'Modules' and then "Set paths for all modules..." to select the path of your JUCE module directory
3. Click File->Save
4. Your build files are now in the Builds directory

Limitations
===========

* GCHQ currently does not report the correct latency when the reverb program is changed during playback. You will need to reload GCHQ or restart the audio engine for correct latency compensation everytime the reverb program is changed
* The user interface looks like a design accident from the 90s...
* There are several potential optimizations in the impulse response code that haven't been implemented yet
* Currently, the impulse responses are mono only
* I still don't know what GCHQ actually stands for :-). Any suggestions?
