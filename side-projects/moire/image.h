#include "omni-include.h"

void random_png(u32 width, u32 height, u32 NChannels, const char* path_to_save);

//percent is in the range 0-1, and is the number of pixels that are a random color
void random_png_percent(u32 width, u32 height, u32 NChannels, const char* path_to_save, f32 percent);