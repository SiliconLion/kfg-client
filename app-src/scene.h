#pragma once

#include "omni-include.h"

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include "dynarr.h"


//Returns a dynarr<Model> of all the models in the scene. Returns DYNARR_ZERO on error
dynarr DoTheImportThing( const char* pFile);