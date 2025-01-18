#include "scene.h"

#include <stdio.h>
#include <string.h>
#include "model.h"
#include "vertices.h"
#include "geometry.h"
#include "texture.h"
#include "utilities.h"

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#endif


bool ai_mesh_to_geom(FullGeometry* geom, struct aiMesh* mesh) {
       if(mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
            printf("mesh contains non-triangle vertices");
            return false;
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

        *geom = full_geom_new(
            bp,
            sizeof(ThreeNormTexPoint),
            vertices, indices,
            GL_TRIANGLES, GL_STATIC_DRAW
        );

        return true;
}

//TODO: the double pointer for tex is annoying, but the current texture module is written around a Texture
//being a pointer. So thats what it is for now
bool load_texture_from_mat(const char* pFile, Texture** tex, struct aiMaterial* mat, enum aiTextureType type, u32 mat_idx) {
    Texture* diffuse_tex;

        struct aiString rel_tex_path;
        enum aiReturn ret = aiGetMaterialTexture(
            mat, type, 0, 
            // material, aiTextureType_BASE_COLOR, 0,   
            &rel_tex_path, 
            NULL, NULL, NULL, NULL, NULL, NULL
            );
        if(ret != aiReturn_SUCCESS) {
            const char * type_as_str = aiTextureTypeToString(type);
            printf("failed to get %s from scene for mesh %u\n", type_as_str, mat_idx);
            return false;
        } else {

            char * dir = get_dir_from_file_path(pFile);

            //ToDo: refactor this out into a utility function?
            u32 full_path_len = strlen(dir) + strlen(rel_tex_path.data) + 1 + 1; //+1 for '/' and +1 for '\0'
            char * full_tex_path = calloc(full_path_len, sizeof(char));
            strcat(full_tex_path, dir);
            strcat(full_tex_path, "/"); 
            strcat(full_tex_path, rel_tex_path.data);

            //TODO: Detect alpha
            *tex = tex_new(full_tex_path, false);
        }

        return true;
}


bool import_scene(Scene* scene_out, const char* pFile) {
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
        return false;
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

    

    // if(curr_node->mNumChildren >= 1) {
    //     //recurse 
    //     printf("cannot handle multiple children yet");
    //     return DYNARR_ZERO;

    //     //When not in the root node, we need to keep track of the relative transformations of each node
    // }


    // printf("curr_node num children: %u\n", curr_node->mNumChildren);
    // printf("curr_node num meshes: %u\n", curr_node->mNumMeshes);

    // C_STRUCT aiMatrix4x4 curr_transformation = curr_node->mTransformation;

    // dynarr models = dynarr_new(sizeof(Model), curr_node->mNumMeshes);


//load every mesh into a FullGeometry 
    dynarr geometries = dynarr_new(sizeof(FullGeometry), scene->mNumMeshes);
    for(u32 i = 0; i < scene->mNumMeshes; i++) {
        
        struct aiMesh* mesh = scene->mMeshes[i];
        FullGeometry g;
        if(!ai_mesh_to_geom(&g, mesh)) {
            return false;
        }
        dynarr_push(&geometries, &g);
    }

//load the materials by loading each relavent texture

    //TODO: refactor the texture module to not be based around a Texture pointer but just a 
    //texture struct
    dynarr diffuse_textures = dynarr_new(sizeof(Texture*), scene->mNumMaterials);
    dynarr normals_textures = dynarr_new(sizeof(Texture*), scene->mNumMaterials);

    for(u32 i = 0; i < scene->mNumMaterials; i++) {
        struct aiMaterial* material = scene->mMaterials[i];

        if(i == 25) {
            printf("here\n");
        }

        { //just debug printing stuff. good info. 
            printf("Material with index %u has the following textures:\n", i);

            // //looping through every variant in aiTextureType
            for(u32 j = 0; j < AI_TEXTURE_TYPE_MAX; j++) {
                u32 tex_count = aiGetMaterialTextureCount(material, j);
                const char* tex_name = aiTextureTypeToString(j);
                printf("    %u %s textures.\n", tex_count, tex_name);
            }
        }

        Texture* diffuse;
        Texture* normals; 

        if(!load_texture_from_mat(pFile, &diffuse, material, aiTextureType_DIFFUSE, i)) {
            diffuse = NULL;
        }
        dynarr_push(&diffuse_textures, &diffuse);

        if(!load_texture_from_mat(pFile, &normals, material, aiTextureType_NORMALS, i)) {
            normals = NULL;
        }
        dynarr_push(&normals_textures, &normals);
    }

//join the geometries and textures into model prototypes
    dynarr model_prototypes = dynarr_new(sizeof(ModelPrototype), scene->mNumMeshes);
        
    for(u32 i = 0; i < scene->mNumMeshes; i++) {
        struct aiMesh* mesh = scene->mMeshes[i];
        u32 mat_idx = mesh->mMaterialIndex;

        FullGeometry* geom = dynarr_at(&geometries, i);

        Texture** diffuse_tex = dynarr_at(&diffuse_textures, mat_idx);
        Texture** normals_tex = dynarr_at(&normals_textures, mat_idx);

        ModelPrototype m = model_prototype_new(geom, diffuse_tex, normals_tex); //When we handle materials, this will be modified.
        dynarr_push(&model_prototypes, &m);
    }

//create all the model instances

    dynarr model_instances = dynarr_new(sizeof(ModelInstance), scene->mNumMeshes);
    //ToDo: Trace the scene herarchy/nodes . However for the sponza model, which has
    //only one node, it doesn't matter. So for now just give them all the identity transformation
    for(u32 i = 0; i < scene->mNumMeshes; i++) {
        ModelInstance m = model_instance_new(dynarr_at(&model_prototypes, i), IDENTITY); 
        dynarr_push(&model_instances, &m);
    }

    

    *scene_out = (Scene){
        .geometries = geometries,
        .diffuse_textures = diffuse_textures,
        .normals_textures = normals_textures,
        .model_prototypes = model_prototypes,
        .model_instances  = model_instances
    };

    //Release all resources associated with this import
    aiReleaseImport( scene);

    return true;
}