#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
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
    //create shaders and compile
    int success;
    char infolog[512];
    const char *vertexSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main(){\n"
	    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\n\r";
    const char *fragmentSource1 = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
	    "FragColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);\n"
        "};\n\r";
    const char *fragmentSource2 = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
	    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "};\n\r";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512, NULL, infolog);
        std::cout << "ERROR COMPILE VERTEX SHADER\n" << infolog << std::endl;
    }
    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource1, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        std::cerr << "FAILED TO COMPILE FRAGMENT SHADER\n" << infolog << std::endl;
    }
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentSource2, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infolog);
        std::cerr << "FAILED TO COMPILE FRAGMENT SHADER\n" << infolog << std::endl;
    }
    unsigned int shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader);
    glLinkProgram(shaderProgram1);
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infolog);
        std::cerr << "FAILED TO LINK SHADERS TO PROGRAM\n" << infolog << std::endl;
    }
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infolog);
        std::cerr << "FAILED TO LINK SHADERS TO PROGRAM\n" << infolog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    //deinfe vertices
/*    float vertices[] = {0.5, 0.5, 0.0,
                        0.5,-0.5,0.0,
                        -0.5,-0.5,0.0,
                        -0.5,0.5,0.0};*/
    float tri_one[] = { // verts for triangle 1
			-0.9,0.0,0.0,
			-0.1,0.0,0.0,
			-0.1,0.5,0.0};
    float tri_two[] = { // verts for tri 2
			0.9,0.0,0.0,
			0.1,0.0,0.0,
			0.1,0.5,0.0};
    		      
    //define indices
    unsigned int indices[] = {0,1, 3,
                              1,2,3};

    //creaet buffers objects
    unsigned int VBO1,VBO2,VAO1, VAO2,EBO;
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
   // glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindVertexArray(0);
    //----------------------------------
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_two), &tri_two, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //render loop
    while(!glfwWindowShouldClose(window)){
        //input
        process(window);
        //render
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 //       glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram2);
	    glBindVertexArray(VAO2);
	    glDrawArrays(GL_TRIANGLES, 0, 3);
 
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
