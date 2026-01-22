//
// Created by niel on 1/7/26.
//

#include "renderer.h"
void Engine::Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
void Engine::ShaderProgram::use()
{
    glUseProgram(ID);
}

void Engine::ShaderProgram::addShader(Shader* shader) {
    shaders.push_back(shader);
}

void Engine::ShaderProgram::compile() {
    for (auto & shader : shaders) {
        glAttachShader(ID,shader->shader);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
}

void Engine::ShaderProgram::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
Engine::Shader::Shader(const char* shaderPath,const int shaderType,std::string name)
{
    this->shaderType = shaderType;
    this->name = name;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string shaderCode;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    // open files
    shaderFile.open(shaderPath);
    if (!shaderFile.is_open()) {
        perror("ifstream open failed");                     // OS error
        std::cout << "errno = " << errno << std::endl;      // numeric errno
        std::cout << "shaderPath = [" << shaderPath << "]" << std::endl;
        exit(1);                                           // stop immediately
    }
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << shaderFile.rdbuf();
    // close file handlers
    shaderFile.close();
    // convert stream into string
    shaderCode   = vShaderStream.str();
    const char* vShaderCode = shaderCode.c_str();
    // 2. compile shaders
    // vertex shader
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &vShaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader,name + " shader");
}

Engine::Renderer::Renderer(ShaderProgram *shaderProgram) {
    this->shaderProgram = shaderProgram;
    glEnable(GL_DEPTH_TEST);
}
void Engine::Renderer::render(unsigned int start, unsigned int count,GLenum type,Texture* tex,glm::mat4 model) {
    glm::mat4 proj =glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    shaderProgram->setUniformMatrix4fv("projection", proj);
    shaderProgram->setUniformMatrix4fv("view", view);
    shaderProgram->setUniformMatrix4fv("model",model);
    shaderProgram->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->ID);
    glBindVertexArray(VAO);
    glDrawElements(type, count, GL_UNSIGNED_INT, (void*)(start * sizeof(unsigned int)));
}
void Engine::Renderer::renderUDVA(unsigned int start, unsigned int count,GLenum type,Texture* tex,glm::mat4 model) {
    glm::mat4 proj =glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    shaderProgram->setUniformMatrix4fv("projection", proj);
    shaderProgram->setUniformMatrix4fv("view", view);
    shaderProgram->setUniformMatrix4fv("model",model);
    shaderProgram->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->ID);
    glBindVertexArray(VAO);
    glDrawArrays(type,start, count);
}

void Engine::Renderer::setVAtributes(int layout, unsigned int size, GLenum type, GLboolean normalize, unsigned int stride, unsigned int offset) {
    glBindVertexArray(VAO);
    glVertexAttribPointer(layout, size, type, normalize, stride, reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
    glEnableVertexAttribArray(layout);
}

void Engine::Renderer::loadData(float *vertex, int vertexCount, unsigned int* indices, int indexCount) {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);

    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);
}

Engine::Texture::Texture(std::string filePath, ShaderProgram* shaderProgram) {
    data = stbi_load(filePath.c_str(),&width,&height,&nrChannels,0);
    if (!data) {
        std::cout << "Error loading texture file " << filePath << std::endl;
        std::cout << "ERROR::STBI::LOADING_FAILED\n" << stbi_failure_reason();
        return;
    }
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(shaderProgram->ID, "texture1"), 0);

}

Engine::Texture::~Texture() {
    stbi_image_free(data);
}

Engine::Sprite2D::Sprite2D(std::string filePath) {
    shaderProgram = Engine::Engine::Instance().defaultShaderProgram;
    renderer = Engine::Engine::Instance().SpriteRenderer;
    texture = new Texture(filePath,shaderProgram);
    vertexData = generateTextureVertices(texture->width,texture->height).data();
    renderer->loadData(vertexData,20,indices,6);
    renderer->setVAtributes(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),0);
    renderer->setVAtributes(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),3 * sizeof(float));
    this->renderer = renderer;
    position = glm::vec3(0.0f,0.0f,0.0f);
    scale = glm::vec3(1.0f,1.0f,1.0f);
    rotation = 0.0f;
    REGISTER_UPDATE(Render);
}

void Engine::Sprite2D::Render(double deltaTime) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    renderer->render(0,6,GL_TRIANGLES,texture,model);
}
