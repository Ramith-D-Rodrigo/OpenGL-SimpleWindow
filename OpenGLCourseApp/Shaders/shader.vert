#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;    //how the camera translates what can be seen the screen  

out vec4 vCol; //vertex color                                        
                                                                            
void main()                                                                 
{                                                                          
    gl_Position = projection * model * vec4(pos, 1.0); //we can pass pos vector directly, no need to access each axis
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);  
    //make the color to the position values (using clamp to deal with the negative values
    //set between 0 and 1)
}