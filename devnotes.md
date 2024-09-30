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
