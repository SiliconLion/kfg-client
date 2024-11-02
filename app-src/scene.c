#include "scene.h"

#include <stdio.h>
#include <string.h>


bool DoTheImportThing( const char* pFile) {
    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile( pFile,
                                                aiProcess_CalcTangentSpace       |
                                                aiProcess_Triangulate            |
                                                aiProcess_JoinIdenticalVertices  |
                                                aiProcess_SortByPType);

    // If the import failed, report it
    if( NULL == scene) {
        //ToDo: Better Error logging. Need like an "error.h"
        const char* err_msg = aiGetErrorString();

        //TODO: fix validation. strlen_s doesn't want to load rn and I have other compilation problems I have to fix first.
//        if(strlen_s(err_msg, 10000) >= 10000) {
//            printf("assimp errored but error message is mallformed.");
//            exit(-1);
//        }
    //        we've validated that the string is okay to print, dispite not being a str litterals
        printf("Assimp errored: %s\n", err_msg);
        return false;
    }

    // Now we can access the file's contents
    struct aiNode* root_node = scene->mRootNode;
    printf("num of root node children: %u\n", root_node->mNumChildren);


    // We're done. Release all resources associated with this import
    aiReleaseImport( scene);
    return true;
}