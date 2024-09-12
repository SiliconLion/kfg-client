
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform float x_rotation;
uniform float y_rotation;
uniform float x_offset;
uniform float y_offset;

out vec3 pos;
out vec3 norm;



void main()
{
    mat4 transformX = mat4(	1.0,		0,			0,			0,
                        0, 	cos(x_rotation),	-sin(x_rotation),		0,
                        0, 	sin(x_rotation),	 cos(x_rotation),		0,
                        0, 			0,			  0, 		1);


    mat4 transformY = mat4(	cos(y_rotation),		0,	  sin(y_rotation),	   0,
                                    0,	    1.0,  		 0,	       0,
                            -sin(y_rotation),	0,	  cos(y_rotation),	   0,
                                    0, 		0,	  		 0,	       1);

    norm = (transformX * vec4(aNorm, 1.0)).xyz;
    pos = (transformX * vec4(aPos, 1.0)).xyz;

    norm = (transformY * vec4(norm, 1.0)).xyz;
    pos = (transformY * vec4(pos, 1.0)).xyz;

    gl_Position =  vec4(pos.x + x_offset, pos.y + y_offset, pos.z, 1.0);
}
