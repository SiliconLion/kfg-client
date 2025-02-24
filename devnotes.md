9/10/24 - David S- did all the work to use glfw precompiled binary because I couldn't get the system-installed 
library to work, but then figured out the problem was something else and the `find_package(glfw3 ...)` way worked. 
But I am not deleting `glfw-3.4.bin.MACOS` just yet. This may be the way I want to do it in the future. Easier to 
pin a specific version of glfw, and may be an easier way to compile cross platform than faffing around with system 
specific "find package" stuff. Or maybe not. staying here for now though unused.

9/17/24 - David S- just a note, gotta rerun cmake whenever you add assets or they dont get copied it seems. At least 
not through my CLion setup. 

9/30/24 - David S- The "omni-include.h" file is meant to be included in pretty much all new code I write. It's inspired 
by Nullprogram's 2023 C style guide https://nullprogram.com/blog/2023/10/08/ . It's mostly just convenient type 
definitions and will expand over time. But may eventually include some configuration stuff I haven't decided.

10/29/24 - David S- remember to run cmake whenever updating shaders cuz they're in the assets folder, and the 
updated version wont get copied unless you do. 
TODO: make it so CMake always checks for changes in the assets folder
12/28/24 - David S- working on getting this to build on windows. ASSIMP has an error when building with mingw. The suggested fix was to add a line `set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "[Assimp] Treat all warnings as errors.")` in the assimp cmake file. I added it to line 43. Hasnt done the trick yet but leaving it for the moment cuz I gotta go home. 
01/16/25 - David S- There is a choice to be made about using what ASSIMP reports as a `diffuse_texture` and a `base_color` texture. Theoretically the difference is diffuse textures had ambient occlusion baked into them and are pre pbr, whereas base_color is just albedo. However it is not clear to me that this is the case from this sponza model. Since i do not currently calculate ambient occlusion, I am going with `diffuse_texture`. I will continue building based on this sponza model, but this will be something to revist with other standard test models and with models we export from blender. 
2/19/25 - David S- I had to update glfw via brew randomly because cmake seemed to be having trouble finding it suddenly? This could get tedious across different machines. May want to look into building from source.
2/19/25 - David S- Joystick/Gamepad seems to not recognize certian buttons. For now I am just going to work around this. I started getting the joystick example from GLFW running, but it needs several dependencies, and it may be best to (as in my last note) just build from source. But not a problem for the moment. Will just work around it. 
2/24/25 - David S- On a new windows laptop. Built with CLion which bundles mingw. We are now building glfw from source but thats for the best honestly.