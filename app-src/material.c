#include "material.h"
#include <assimp/cimport.h> 
#include <assimp/scene.h>   


const char* PBRTextureChannelToStr(PBRTextureChannel c) {
    switch(c) {
        case PBR_CHANNEL_DIFFUSE :
            return "DIFFUSE"; 
        case PBR_CHANNEL_SPECULAR :
            return "SPECULAR";
        // case PBR_CHANNEL_AMBIENT :
            // return "ambient";
        // case PBR_CHANNEL_EMISSIVE :
        //     return "emmisive";
        // case PBR_CHANNEL_HEIGHT :
        //     return "height";
        case PBR_CHANNEL_NORMALS :
            return "NORMALS";
        // case PBR_CHANNEL_SHININESS :
        //     return "shininess";
        // case PBR_CHANNEL_OPACITY :
        //     return "opacity";
        // case PBR_CHANNEL_DISPLACEMENT :
        //     return "displacement";
        // case PBR_CHANNEL_LIGHTMAP :
        //     return "lightmap";
        // case PBR_CHANNEL_REFLECTION :
        //     return "reflection";
        // case PBR_CHANNEL_BASE_COLOR :
        //     return "base_color";
        // case PBR_CHANNEL_NORMAL_CAMERA :
        //     return "normal_camera";
        // case PBR_CHANNEL_EMISSION_COLOR :
        //     return "emmision_color";
        // case PBR_CHANNEL_METALNESS :
        //     return "METALNESS";
        case PBR_CHANNEL_DIFFUSE_ROUGHNESS :
            return "ROUGHNESS";
        // case PBR_CHANNEL_AMBIENT_OCCLUSION :
        //     return "ambient_occlusion";
        // case PBR_CHANNEL_UNKNOWN :
        //     return "unknown";
        // case PBR_CHANNEL_SHEEN :
        //     return "sheen";
        // case PBR_CHANNEL_CLEARCOAT :
        //     return "clearcoat";
        // case PBR_CHANNEL_TRANSMISSION :
        //     return "transmission";
        default:
            printf("thats not a PBRTextureChannel\n");
            exit(-1);
    }
}


enum aiTextureType PBRTextureChannelToAI(PBRTextureChannel channel){
    switch(channel) {
        case PBR_CHANNEL_DIFFUSE :
            return aiTextureType_DIFFUSE; 
        case PBR_CHANNEL_SPECULAR :
            return  aiTextureType_SPECULAR;
        case PBR_CHANNEL_NORMALS :
            return aiTextureType_NORMALS;
        case PBR_CHANNEL_DIFFUSE_ROUGHNESS :
            return aiTextureType_DIFFUSE_ROUGHNESS;
        default:
            printf("thats not a PBRTextureChannel\n");
            exit(-1);
    }
}

// PBRTextureChannel AIToPBRTextureChannel(enum aiTextureType){

// }




void PBRMaterialBind(PBRMaterial* mat) {
    for(u32 i = 0; i < PBR_CHANNEL_COUNT; i++) {
        Texture* channel = mat->channels[i];
        if(channel == NULL) {continue;} 
        tex_bind(channel, i);
    }
}

//deletes the material, but does not call delete on any of the textures it refrences.
void PBRMaterialSoftDelete(PBRMaterial* mat);

//deletes the material and calls delete on every texture it refrences.
void PBRMaterialDeepDelete(PBRMaterial* mat);