#include "vertices.h"
#include "platform.h"

INCLUDE_OPENGL

#include <stddef.h>


// Note: A lot of these functions are totally untested, and writing them is very error prone.
// So if using one of them for the first time, may want to sanity check it somehow


void vert_enable_two_point() {
        //sets up the position attribute
        glVertexAttribPointer(0, 2,
                              GL_FLOAT, GL_FALSE,
                              sizeof(TwoPoint), (void*)offsetof(TwoPoint, pos)
                              );
        glEnableVertexAttribArray(0);
}

void vert_enable_three_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreePoint), (void*)offsetof(ThreePoint, pos)
    );
    glEnableVertexAttribArray(0);
}

void vert_enable_four_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 4,
                          GL_FLOAT, GL_FALSE,
                          sizeof(FourPoint), (void*)offsetof(ThreePoint, pos)
    );
    glEnableVertexAttribArray(0);
}

void vert_enable_two_tex_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 2,
                          GL_FLOAT, GL_FALSE,
                          sizeof(TwoTexPoint), (void*)offsetof(TwoTexPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the texture attributes
    glVertexAttribPointer(1, 2,
                          GL_FLOAT, GL_FALSE,
                          sizeof(TwoTexPoint), (void*)offsetof(TwoTexPoint , tex)
    );
    glEnableVertexAttribArray(1);
}

void vert_enable_three_tex_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeTexPoint), (void*)offsetof(ThreeTexPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the texture attributes
    glVertexAttribPointer(1, 2,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeTexPoint), (void*)offsetof(ThreeTexPoint , tex)
    );
    glEnableVertexAttribArray(1);
}

void vert_enable_four_tex_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 4,
                          GL_FLOAT, GL_FALSE,
                          sizeof(FourTexPoint), (void*)offsetof(FourTexPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the texture attributes
    glVertexAttribPointer(1, 2,
                          GL_FLOAT, GL_FALSE,
                          sizeof(FourTexPoint), (void*)offsetof(FourTexPoint , tex)
    );
    glEnableVertexAttribArray(1);
}






void vert_enable_three_norm_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormPoint), (void*)offsetof(ThreeNormPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the norm attributes
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormPoint), (void*)offsetof(ThreeNormPoint , norm)
    );
    glEnableVertexAttribArray(1);
}

void vert_enable_four_norm_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 4,
                          GL_FLOAT, GL_FALSE,
                          sizeof(FourNormPoint), (void*)offsetof(FourNormPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the norm attributes
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(FourNormPoint), (void*)offsetof(FourNormPoint , norm)
    );
    glEnableVertexAttribArray(1);
}






void vert_enable_three_norm_tex_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormTexPoint), (void*)offsetof(ThreeNormTexPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the normal attribute
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormTexPoint), (void*)offsetof(ThreeNormTexPoint , norm)
    );
    glEnableVertexAttribArray(1);

    //set up the texture-coord attribute
    glVertexAttribPointer(2, 2,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormTexPoint), (void*)offsetof(ThreeNormTexPoint , tex)
    );
    glEnableVertexAttribArray(2);

}

void vert_enable_four_norm_tex_point() {
    //sets up the position attribute
    glVertexAttribPointer(0, 4,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormTexPoint), (void*)offsetof(ThreeNormTexPoint , pos)
    );
    glEnableVertexAttribArray(0);

    //set up the texture attributes
    glVertexAttribPointer(1, 3,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormTexPoint), (void*)offsetof(ThreeNormTexPoint , norm)
    );
    glEnableVertexAttribArray(1);

    //set up the texture-coord attribute
    glVertexAttribPointer(2, 2,
                          GL_FLOAT, GL_FALSE,
                          sizeof(ThreeNormTexPoint), (void*)offsetof(ThreeNormTexPoint , tex)
    );
    glEnableVertexAttribArray(2);
}

VERTEX_BLUEPRINT* TwoPointBlueprint = &vert_enable_two_point;
VERTEX_BLUEPRINT* ThreePointBlueprint = &vert_enable_three_point;
VERTEX_BLUEPRINT* FourPointBlueprint = &vert_enable_four_point;
VERTEX_BLUEPRINT* TwoTexPointBlueprint = &vert_enable_two_tex_point;
VERTEX_BLUEPRINT* ThreeTexPointBlueprint = &vert_enable_three_tex_point;
VERTEX_BLUEPRINT* FourTexPointBlueprint = &vert_enable_four_tex_point;
VERTEX_BLUEPRINT* ThreeNormPointBlueprint = &vert_enable_three_norm_point;
VERTEX_BLUEPRINT* FourNormPointBlueprint = &vert_enable_four_norm_point;
VERTEX_BLUEPRINT* ThreeNormTexPointBlueprint = &vert_enable_three_norm_tex_point;
VERTEX_BLUEPRINT* FourNormTexPointBlueprint = &vert_enable_four_norm_tex_point;
