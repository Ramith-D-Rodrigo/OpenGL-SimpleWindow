#version 330

in vec4 vCol;   //get it from the vShader                                               
                                                                            
out vec4 colour;                                                            
                                                                            
void main()                                                                 
{                                                                           
    colour = vCol;                                      
}