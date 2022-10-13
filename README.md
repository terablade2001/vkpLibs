# vkpLibs
A library of small code utilities.
-- Mainly used on the project [vp-cpp-template](https://github.com/terablade2001/vp-cpp-template) --

* **vkpBuildVersioner** [C++, Python]: System for automated versioning in C++ and Python.
* **vkpConfigReader** [C++]: Utility to load multiple variables from configuration files directly to the wanted types. In version (0.219) it also supports CLI input reading.
* **vkpProgressBar** [C++]: A class to create custom progress bars in the stdout, to monitorprogress graphically.
* **vkpCircularBuffer** [C++, Requires: CECS]: A very simple template-class for circular buffers where their index is treated alike infinite arrays.
* **vkpTimer** [C++]: Classes for easily measuring and handling processing times between two points of code.
* **vkpOpmlCpp** [C++]: Export data as strings to MindMap's opml format.
* **vkpCSVHandler** [C++]: Load, handle and store CSV files in C++.

# Versioning

## version (0.221)
- [x] vkpTimer::reset() also set totalTime to 0.
## version (0.220)
### - vkpConfigReader updates
- [x] If it detects two consecutive directives (`-`) then it doesn't produce error. Instead takes the second directive as the value for the first. Example: `-i -3` means `i = -3`.
- [x] Negative numbers are not count for directives. Example `./main -2` now gives `argv[0] = ./main` and `argv[1] = -2`.
- [x] cfg_ValueConvert() prints a message if exception during conversion is captured.

