#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
void process(GLFWwindow *window, float &mapx, float &mapy);
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
    float mapx = 1.0f, mapy = 1.0f;
    float tri_one[] = { // verts for triangle 1
			// verts pos  	 //colours	 //tex coords
			0.5f,0.5f,0.0f,     /*1.0f,0.0f,0.0f,*/ 1.0f,1.0f, //top right
			0.5f,-0.5f,0.0f,    /*0.0f,1.0f,0.0f,*/ 1.0f,0.0f, // bottom right
			-0.5f,-0.5f,0.0f,   /*0.0f,0.0f,1.0f,*/ 0.0f,0.0f, //bottom left
			-0.5f,0.5f,0.0f,    /*1.0f,1.0f,0.0f,*/ 0.0f,1.0f// top left
    };    		      
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
    //position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //tex coord attributes
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    //----------------------------------
    glEnableVertexAttribArray(0);
    //render loop
    stbi_set_flip_vertically_on_load(true);
    //load/create texture 
    int texWidth, texHeight, nrChannels;
    unsigned int texture;
    glGenTextures(1, &texture); //create a texture 'object'
    glBindTexture(GL_TEXTURE_2D, texture);
    //set texture warp/filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned char* data = stbi_load("textures/container.jpg", &texWidth, &texHeight, &nrChannels, 0);
    if(data){
	    //generate texture
      std::cout << texWidth << ", " << texHeight << "\n";
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
    }
  	else{ std::cerr << "Failed to load texture\n"; return -1; }
  unsigned int texture2;
  data = stbi_load("flare.jpg", &texWidth, &texHeight, &nrChannels, 0);
  glGenTextures(1,&texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
    //set texture warp/filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if(data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
	stbi_image_free(data);
  shaders.use();
	glUniform1i(glGetUniformLocation(shaders.ID, "texture"), 0);
  shaders.setInt("texture2",1);
  //transformations
  glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(0.25f,0.50f,0.0f));
  vec = trans * vec;
  trans = glm::scale(trans, glm::vec3(0.5f,1.0f,2.0f));

  //sedn translation matrix to the shader
  while(!glfwWindowShouldClose(window)){
        //input
        process(window,mapx, mapy);
        //render
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        //rotate
        trans = glm::rotate(trans, (float)glfwGetTime()/60.0f, glm::vec3(0.0f,0.0f,1.0f));

        unsigned int transLoc = glGetUniformLocation(shaders.ID, "transform");
        glUniformMatrix4fv(transLoc,1, GL_FALSE, glm::value_ptr(trans));



        shaders.setFloat("mapx",mapx);
        shaders.setFloat("mapy",mapy);

        glActiveTexture(GL_TEXTURE0);
      	glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO1);

//        shaders.use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

void process(GLFWwindow *window, float &mapx, float &mapy){
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
  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
    mapy += 0.5f;
  }
  else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
   mapy -= 0.5f;
    if(mapy < 0.0f)
    {
      mapy = 0.0f;
    }
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
    mapx -= 0.50f;
    if(mapx < 0.0f){
      mapx = 0.0f;
    }
  }
  else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
    mapx += 0.5f;
  }
}
