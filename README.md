# WinUI 3, C++/WinRT, and Xaml with CMake

![A screenshot of the app, showing a left hand navigation view](screenshot.png)

## Status

This is an as-is, unsupported, proof of concept. No help is available.

## Notes

For a real-world application, see https://github.com/openkneeboard/openkneeboard

* Xaml-related header files **must** be listed as targert sources
* The `Directory.Build.targets` file is currently required; it slightly modifies the CMake generator
* I've avoided `use namespace` to be explicit; I'm not aiming for this to be 'best practice', just to be unambiguous and minimal
* `pch.h` and its' contents are required by the generated code
* `pch.h` is not currently a pre-compiled header
