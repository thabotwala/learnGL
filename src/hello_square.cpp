#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "../include/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
void process(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(){
    //init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //vreate window
    GLFWwindow *window = glfwCreateWindow(800, 600, "", NULL, NULL);
    if(window == NULL){
        std::cerr << "Failed to create window object\n\r";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // load glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Could not Load GLAD\n\r";
        return -1;
    }
    Shader shaders("shaders/vertex.glsl", "shaders/fragment.glsl");
    //define vertices
/*    float vertices[] = {0.5, 0.5, 0.0,
                        0.5,-0.5,0.0,
                        -0.5,-0.5,0.0,
                        -0.5,0.5,0.0};*/
    float tri_one[] = { // verts for triangle 1
			// verts pos  	 //colours	 //tex coords
			0.5f,0.5f,0.0f,     1.0f,0.0f,0.0f, 1.0f,1.0f,
			0.5f,-0.5f,0.0f,    0.0f,1.0f,0.0f, 1.0f,0.0f,
			-0.5f,-0.5f,0.0f,   0.0f,0.0f,1.0f, 0.0f,0.0f,
			-0.5f,0.5f,0.0f,    1.0f,1.0f,0.0f, 0.0f,1.0f
    };
   /* float tri_two[] = { // verts for tri 2
			0.9,0.0,0.0,
			0.1,0.0,0.0,
			0.1,0.5,0.0};
   */
    		      
    //define indices
    unsigned int indices[] = {0,1, 3,
                              1,2,3};

    //creaet buffers objects
    unsigned int VBO1,VBO2,VAO1, VAO2,EBO;
    glGenVertexArrays(1, &VAO1);
 //   glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO1);
 //   glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    //bind buffers
    glBindVertexArray(VAO1);

    //bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    //binding verts to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_one), &tri_one, GL_STATIC_DRAW);
    //bind ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    //----------------------------------
/*    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_two), &tri_two, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);*/
    glEnableVertexAttribArray(0);
    //render loop
    //some vertex checking stuff
    int nrAttrib = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttrib);
    std::cout << nrAttrib << std::endl;
    //load/create texture 
    int texWidth, texHeight, nrChannels;
    unsigned int texture;
    glGenTextures(1, &texture); //create a texture 'object'
    glBindTexture(GL_TEXTURE_2D, texture);
    //set texture warp/filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char* data = stbi_load("textures/container.jpg", &texWidth, &texHeight, &nrChannels, 0);
    if(data){
	    //generate texture
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{ std::cerr << "Failed to load texture\n"; }
	stbi_image_free(data);
    while(!glfwWindowShouldClose(window)){
        //input
        process(window);
        //render
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        shaders.use();
	glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO1);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
//	    glBindVertexArray(VAO2);
//	    glDrawArrays(GL_TRIANGLES, 0, 3);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clean up

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void process(GLFWwindow *window){
	bool wireframe = false;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    }
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
    	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}
