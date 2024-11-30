#include "scene.h"

#include <stdio.h>
#include <string.h>
#include "model.h"
#include "vertices.h"
#include "geometry.h"
#include "texture.h"
#include "utilities.h"
#include "material.h"

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif


Texture* tex_from_ai_mat(struct aiMaterial* material, const char * directory_of_scene, PBRTextureChannel channel) {
        //just a little rename for simplicity in the code
        const char * dir = directory_of_scene;

        enum aiTextureType tex_type = PBRTextureChannelToAI(channel);

        struct aiString rel_tex_path;
        enum aiReturn ret = aiGetMaterialTexture(
            material, tex_type, 0,  
            &rel_tex_path, 
            NULL, NULL, NULL, NULL, NULL, NULL
            );
        if(ret != aiReturn_SUCCESS) {
            printf("failed to get %s texture from material\n", PBRTextureChannelToStr(channel));
            return NULL;
        } else {

            u32 full_path_len = strlen(dir) + strlen(rel_tex_path.data) + 1 + 1; //+1 for '/' and +1 for '\0'
            char * full_tex_path = calloc(full_path_len, sizeof(char));
            strcat(full_tex_path, dir);
            strcat(full_tex_path, "/"); 
            strcat(full_tex_path, rel_tex_path.data);

            //TODO: Detect alpha
            return tex_new(full_tex_path, false);
        }
}



int scene_from_file(Scene* dest, const char* pFile) {
    const mat4 IDENTITY;
    glm_mat4_identity(IDENTITY);


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
        return -1; //ToDo: return better error?
    }

    printf("Scene has %u meshes.\n", scene->mNumMeshes);
    printf("Scene has %u materials.\n", scene->mNumMaterials);
    printf("Scene has %u textures.\n", scene->mNumTextures);
    printf("Scene has %u cameras.\n", scene->mNumCameras);
    printf("Scene has %u lights.\n", scene->mNumLights);
    printf("Scene has %u skeletons.\n", scene->mNumSkeletons);
    printf("Scene has %u animations.\n", scene->mNumAnimations);

    // Now we can access the file's contents
    struct aiNode* root_node = scene->mRootNode;
    struct aiNode* curr_node;

    printf("num of root node children: %u\n", root_node->mNumChildren);
    if(root_node->mNumChildren == 0 ) {
        curr_node = root_node;
    } else {
        curr_node = root_node->mChildren[0];
    }

    

    if(curr_node->mNumChildren >= 1) {
        //recurse 
        printf("cannot handle multiple children yet");
        return -1; //ToDo: return better error?

        //When not in the root node, we need to keep track of the relative transformations of each node
    }


    printf("curr_node num children: %u\n", curr_node->mNumChildren);
    printf("curr_node num meshes: %u\n", curr_node->mNumMeshes);

    C_STRUCT aiMatrix4x4 curr_transformation = curr_node->mTransformation;

    dynarr models = dynarr_new(sizeof(Model), curr_node->mNumMeshes);
    dynarr materials = dynarr_new(sizeof(PBRMaterial*), scene->mNumMaterials);

    for(u32 i = 0; i < curr_node->mNumMeshes; i++) {
        u32 mesh_idx = curr_node->mMeshes[i];
        struct aiMesh* mesh = scene->mMeshes[i];

        if(mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
            printf("mesh contains non-triangle vertices");
            return -1; //ToDo: return better error?
        }



        struct aiVector3D* positions =  mesh->mVertices;
        struct aiVector3D* normals =  mesh->mNormals; 
        //For now, we are going to assume that there is only one texture coord per vertex,
        //and that it has 2 components.
        struct aiVector3D* texCoords = mesh->mTextureCoords[0]; 

        //interleave vertices 
        dynarr vertices = dynarr_new(sizeof(ThreeNormTexPoint), mesh->mNumVertices);
        for(u32 vx = 0; vx < mesh->mNumVertices; vx++) {
            ThreeNormTexPoint vertex = {0};
            memcpy(&vertex.pos,  positions + vx, sizeof(f32) * 3);
            memcpy(&vertex.norm, normals   + vx, sizeof(f32) * 3);
            memcpy(&vertex.tex,  texCoords + vx, sizeof(f32) * 2); //only copying the first two components to vertex.tex
            
            dynarr_push(&vertices, &vertex);
        }


        //interleave indices
        dynarr indices = dynarr_new(sizeof(INDEX_TYPE), mesh->mNumFaces * 3);

        for(u32 fx = 0; fx < mesh->mNumFaces; fx++) {
            struct aiFace face = mesh->mFaces[fx];
            //weve already made sure all the faces are just triangles.
            for(u32 k = 0; k < 3; k++) {
                u32 index = (INDEX_TYPE) face.mIndices[k];
                dynarr_push(&indices, &index);
            }
        }

        VERTEX_BLUEPRINT* bp = ThreeNormTexPointBlueprint;

        // upload geometry
        FullGeometry geom = full_geom_new(
            bp,
            sizeof(ThreeNormTexPoint),
            vertices, indices,
            GL_TRIANGLES, GL_STATIC_DRAW
        );

        //handle materials and textures
        //ToDo: dont duplicate materials

        struct aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        PBRMaterial* mat = malloc(sizeof(PBRMaterial));

        

        //get material name.
        struct aiString name;
        aiReturn ret = aiGetMaterialString(material, AI_MATKEY_NAME,&name);
        if(ret != aiReturn_SUCCESS) {
            printf("cannot get name of material? index is %u\n", mesh->mMaterialIndex);
        } else {
            mat->name = calloc(name.length + 1, 1); //gets the '\0'
            memcpy(mat->name, name.data, name.length); 
        }

        printf("Material with index %u has the following textures:\n", mesh->mMaterialIndex);

        //looping through every variant in aiTextureType
        for(u32 i = 0; i < AI_TEXTURE_TYPE_MAX; i++) {
            u32 tex_count = aiGetMaterialTextureCount(material, i);
            const char* tex_name = aiTextureTypeToString(i);
            printf("    %u %s textures.\n", tex_count, tex_name);
        }




        const char* dir = get_dir_from_file_path(pFile);
        for(u32 i = 0; i < PBR_CHANNEL_COUNT; i++) {
            mat->channels[i] = tex_from_ai_mat(material, dir, (PBRTextureChannel)i );
        }
        dynarr_push(&materials, &mat);
        

        Model m = model_new(geom, mat); //When we handle materials, this will be modified.
        dynarr_push(&m.model_instances, IDENTITY);

        dynarr_push(&models, &m);
    }

    dest->models = models;
    dest->materials = materials;

    // We're done. Release all resources associated with this import
    aiReleaseImport( scene);

    return 1;
}