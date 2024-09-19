#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInputs(GLFWwindow *window);

int main(){
    // init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // end init
    // get shader files
    std::ifstream vertexFile("vertex.glsl");
    std::ifstream fragmentFile("fragment.glsl");
    std::string str, vertexSource = "", fragmentSource = "";
    if(vertexFile.is_open()){
    	while(std::getline(vertexFile, str)){
	    	vertexSource += '\n';
		    vertexSource += str;
    	}
	    vertexFile.close();
    }
    if(fragmentFile.is_open()){
	    str = "";
	    while(std::getline(fragmentFile, str)){
		    fragmentSource += '\n';
		    fragmentSource += str;
    	}
	    fragmentFile.close();
    }
    const char *vertexShaderSource = vertexSource.c_str(), *fragmentShaderSource = fragmentSource.c_str();
    //Create Window Object
    GLFWwindow* window = glfwCreateWindow(800,600, "",NULL,NULL);
    //check if window was created
    if(window == NULL){
    	std::cerr << "Failed to creaet window\n\r";
    	glfwTerminate();
        return -1;
    }
    //make the context of our window the main context on the current thread
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //load glad(it manages all of gl function pointers) before using any gl functions
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	    std::cerr << "!!!Could not Load GLAD!!!\n\r";
	    return -1;
    }

	//render a triangle
    float vertices[] = {
            0.5f,0.5f,0.0f, /*top right*/
		    0.5f,-0.5f,0.0f, /*bottom right*/
			-0.5f,-0.5f,0.0f, /*bottom left*/
    		-0.5f,0.5f,0.0f /*top left*/
        };

    //create a vertex buffer object with a specified id to send vertex data to the gpu
    //
    // from here whenever we call GL_ARRAY_BUFFER will use the currently bound buffer which is vbo
    // the we copy the data from vbo to the buffer's memory(this is GL_ARRAY_BUFFER)
    //create indices, the order at which to draw our verticies
    float indices[] = {
            0,1,3,
   		    1,2,3
        };
    //create a VBO,EBO and VAO
    unsigned int vbo, ebo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    //bind the vao first then bind and set vertex buffer(s)
    glBindVertexArray(vao);
    //VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //"config" vao by setting the vertex attribute pointer
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //bind vao
    glBindVertexArray(0);
    //create a vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check for shader compile errors
    int success;
    char infolog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
	glGetShaderInfoLog(vertexShader,512, NULL, infolog);
    	std::cout << "Failed to compile vertex shader successfully\n\r" << infolog << "\n\r";
    }
    //create fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
    	glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
	std::cout << "Failed to compile fragment shader successfully\n\r" << infolog << "\n\r";
    }
    // create a shader program to link all our compiled shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    //attch our shader to the shader program and then link them together
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
    	glGetProgramInfoLog(shaderProgram,512, NULL, infolog);
	std::cerr << "Could not link shader program\n" << infolog << "\n\r";
    }

    //set vertex attributes for openGL to interpret our data(vertex && fragment) correctly
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //render loop
    while(!glfwWindowShouldClose(window)){
    	//inpout
	processInputs(window);
	//render
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	//
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//processEvents
	glfwSwapBuffers(window);
	glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	glViewport(0,0,width,height);
}

void processInputs(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}
