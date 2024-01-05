#include "../include/memory_stats.h"
#include <cstdlib>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
void process(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void updateFPS(GLFWwindow* window, int target);
std::ostream& operator<<(std::ostream& stream, const glm::vec3 &v);
double previousTime = 0.0;
int frameCount = 0;

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
  glm::vec3 cameraTarget = glm::vec3(0.0f,0.0f, 0.0f);
  glm::vec3 cameraDirection = glm::normalize((cameraTarget - cameraPos));
  glm::vec3 worldUpAxis = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraXAxis = glm::normalize(glm::cross(worldUpAxis, cameraDirection));
  glm::vec3 cameraYAxis = glm::normalize(glm::cross(cameraDirection, cameraXAxis));
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  float cameraSpeed;
  glm::mat4 view;
float frameTime = 0.0f;
float lastFrame = 0.0f;
int main(){
    //init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
  
    int width = 1920, height = 1080;
    //vreate window
    GLFWwindow *window = glfwCreateWindow(width, height, "A window title", NULL, NULL);
    if(window == NULL){
        std::cerr << "Failed to create window object\n\r";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // load glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Could not Load GLAD\n\r";
        return -1;
    }
    Shader shaders("shaders/vertex.glsl", "shaders/fragment.glsl");
    //define vertices
    float mapx = 0.0f, mapy = 0.0f;
    //float tri_one[] = { // verts for triangle 1
			  // verts pos  	 //colours	 //tex coords
//			0.5f,0.5f,0.0f,     /*1.0f,0.0f,0.0f,*/ 1.0f,1.0f, //top right
//			0.5f,-0.5f,0.0f,    /*0.0f,1.0f,0.0f,*/ 1.0f,0.0f, // bottom right
//			-0.5f,-0.5f,0.0f,   /*0.0f,0.0f,1.0f,*/ 0.0f,0.0f, //bottom left
//			-0.5f,0.5f,0.0f,    /*1.0f,1.0f,0.0f,*/ 0.0f,1.0f// top left
//    };

    float cube[] = {
    //front face
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
    //back face
     -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f, 0.0f, 0.0f,   
};
    //define indices
    unsigned int indices[] = {
    //front face
    0,1,2,
    0,2,3,
    //back face
    4,5,6,
    4,6,7,
    //Top face
    4,5,1,
    4,1,0,
    //bottom face
    7,6,2,
    7,2,3,
    //left dace
    1,5,6,
    1,6,2,
    //right face
    0,4,7,
    0,7,3
  };
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_STATIC_DRAW);
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

  //sedn translation matrix to the shader
glm::vec3 cubePositions[] = {
glm::vec3( 0.0f, 0.0f,
0.0f),
glm::vec3( 2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3( 2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f, 3.0f, -7.5f),
glm::vec3( 1.3f, -2.0f, -2.5f),
glm::vec3( 1.5f, 2.0f, -2.5f),
glm::vec3( 1.5f, 0.2f, -1.5f),
glm::vec3(-1.3f, 1.0f, -1.5f)
};
  long long mem = memory_used() / 1024;
  std::cout << "Memory Used: " << mem << " kilo bytes" << std::endl;
  std::cout << cameraDirection << "\n" << cameraXAxis << "\n" << cameraYAxis << std::endl;
  while(!glfwWindowShouldClose(window)){
        //input
        float currentFrame =  (float)glfwGetTime();
        frameTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cameraSpeed = 2.0f * frameTime;
        process(window);
        //render
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        

        glActiveTexture(GL_TEXTURE0);
      	glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO1);



     




//        shaders.setFloat("mapx",mapx);
  //      shaders.setFloat("mapy",mapy);




        //A model matrix
       for(unsigned int i = 0; i < 10; i++)
        {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cubePositions[i]);
          if(i % 3 == 0 && i % 2 == 0 && i != 0){
            model = glm::rotate(model, (float)glfwGetTime()*5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
          }
           else if(i % 3 == 0 && i != 0){
            model = glm::rotate(model, (float)glfwGetTime()*2.5f, glm::vec3(1.0f,0.3f,0.1f));

          }
          else{
            float angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f,0.3f,0.1f));
          }
          int modelLoc = glGetUniformLocation(shaders.ID, "modelMatrix");
          glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
          glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT, 0);

        }
        //view

//        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraYAxis);
        // projection matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(60.0f), (float)(width/(float)height), 0.1f, 100.0f);
      
        // mode loc

        //view loc
        int viewLoc = glGetUniformLocation(shaders.ID, "viewMatrix");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        //perspective lov
        int projectionLoc = glGetUniformLocation(shaders.ID, "projectionMatrix");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);


        glBindVertexArray(VAO1);
//        glDrawArrays(GL_TRIANGLES,0,36);
//        shaders.use();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapInterval(1.0/144.0);
        updateFPS(window,0);
        //rotate
/*        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(mapx, mapy, 0.0f));
        trans = glm::scale(trans, glm::vec3(1.0f,1.0f,0.0f));

        transLoc = glGetUniformLocation(shaders.ID, "transform");
        glUniformMatrix4fv(transLoc,1, GL_FALSE, &trans[0][0]);/
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

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
    glLineWidth(3.0f);
  	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   
    }
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
    	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

  //ganlde camera input keys
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    cameraPos += cameraSpeed * cameraFront;
  }
  else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    cameraPos -= cameraSpeed * cameraFront;
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraYAxis)) * cameraSpeed;
  }
  else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraYAxis)) * cameraSpeed; 
  }
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraYAxis);
}
void updateFPS(GLFWwindow* window, int target){
  double currentTime = glfwGetTime();
  double deltaTime = currentTime - previousTime;
  frameCount++;
  double targetDeltaTime = 1.0/(double)target;
  if(targetDeltaTime < deltaTime){
    double sleepTime = targetDeltaTime - deltaTime;
    glfwWaitEventsTimeout(targetDeltaTime);

  }
  if(deltaTime >= 1.0){
    const char *ftitle = (std::string("FPS: ")+std::to_string(frameCount)).c_str();
    glfwSetWindowTitle(window, ftitle);
    frameCount = 0;
    previousTime = currentTime;
  }

}

std::ostream& operator<<(std::ostream& stream, const glm::vec3 &v){
  stream << v.x << ", " << v.y << ", " << v.z;
  return stream;
}
