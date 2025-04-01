# Snake - Work in Progress

## 🚀 How to Run This Project

1. Copy the project using SSH or HTTPS.
2. Run the project in x64 configuration both for `debug` and `releae` modes, it will not work yet! To do this in the visual studio you should select first **debug** and then **release** options from the menu.
3. Copy the files from the bin folder:
   * openal32.dll
   * sfml-audio-d-2.dll
   * sfml-graphics-d-2.dll
   * sfml-system-d-2.dll
   * sfml-window-d-2.dll
3.a. Paste these files into x64/Debug folder.
4. Copy the files from the bin folder:
   * openal32.dll
   * sfml-audio-2.dll
   * sfml-graphics-2.dll
   * sfml-system-2.dll
   * sfml-window-2.dll
4.b. Paste these files into x64/Release folder.
5. Click **F5** in the visual studio and it should work.

### ⚙️ Visual Studio Project Settings

Make sure the following paths are set in your project settings:

```
Make sure that in the properties/C/C++/General/Additional Include Directories you have: `$(SolutionDir)\include;%(AdditionalIncludeDirectories)`.
Make sure that in the properties/Linker/General/Additional Library Directories you have: `$(SolutionDir)\lib;%(AdditionalLibraryDirectories)`.
Make sure that in the properties/Linker/Input/Additional Dependencies you have:
For debug mode: `sfml-system.lib;sfml-window.lib;sfml-graphics.lib;sfml-audio.lib`.
For release mode: `sfml-system-d.lib;sfml-window-d.lib;sfml-graphics-d.lib;sfml-audio-d.lib`.
```
