//
// Created by niel on 1/7/26.
//

#include "renderer.h"
#include "stb_image.h"
#include "scene.h"
#include "phisycs.h"
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
    shaderProgram->setUniformMatrix4fv("model",model);
    shaderProgram->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->ID);
    glBindVertexArray(VAO);
    glDrawElements(type, count, GL_UNSIGNED_INT, (void*)(start * sizeof(unsigned int)));
}
void Engine::Renderer::renderUDVA(unsigned int start, unsigned int count,GLenum type,Texture* tex,glm::mat4 model) {
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
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleMode);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width,height,0,format,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(shaderProgram->ID, "texture1"), 0);

}

Engine::Texture::Texture(std::string filePath, ShaderProgram *shaderProgram, GLenum sampleModePassed) {
    sampleMode = sampleModePassed;
    data = stbi_load(filePath.c_str(),&width,&height,&nrChannels,0);
    if (!data) {
        std::cout << "Error loading texture file " << filePath << std::endl;
        std::cout << "ERROR::STBI::LOADING_FAILED\n" << stbi_failure_reason();
        return;
    }
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleMode);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width,height,0,format,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(shaderProgram->ID, "texture1"), 0);
}

Engine::Texture::~Texture() {
    stbi_image_free(data);
}

Engine::DebugRenderer2D::DebugRenderer2D(ShaderProgram* shaderProgram) : shaderProgram(shaderProgram) {}

Engine::DebugRenderer2D::~DebugRenderer2D() {
    // Avoid GL deletes after the context is gone (happens during shutdown).
    if (glfwGetCurrentContext() == nullptr) {
        VBO = EBO = VAO = 0;
        return;
    }
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    VBO = EBO = VAO = 0;
}

void Engine::DebugRenderer2D::ensureBuffers() {
    if (VAO != 0) return;
    const std::vector<float> verts = generateTextureVertices(1.0f, 1.0f);
    const std::vector<unsigned int> idx = generateTextureIndices();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * idx.size(), idx.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Engine::DebugRenderer2D::drawHitboxes(const std::vector<WorldHitbox2D>& boxes, const Camera& camera, const glm::vec4& color) {
    if (boxes.empty() || !shaderProgram) return;
    ensureBuffers();

    shaderProgram->use();
    shaderProgram->setUniformMatrix4fv("projection", camera.getProjectionMatrix());
    shaderProgram->setUniformMatrix4fv("view", camera.getViewMatrix());
    shaderProgram->setVec2("uFlip", glm::vec2(0.0f));
    shaderProgram->setBool("debugOverride", true);
    shaderProgram->setVec4("debugColor", color);

    GLboolean depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    if (depthWasEnabled) {
        glDisable(GL_DEPTH_TEST);
    }
    GLint prevPolygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, prevPolygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(VAO);
    for (const auto& hb : boxes) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(hb.center, 0.0f));
        model = glm::scale(model, glm::vec3(hb.halfExtents.x * 2.0f, hb.halfExtents.y * 2.0f, 1.0f));
        shaderProgram->setUniformMatrix4fv("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    glPolygonMode(GL_FRONT_AND_BACK, prevPolygonMode[0]);
    shaderProgram->setBool("debugOverride", false);
    if (depthWasEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
}

Engine::Sprite2D::Sprite2D(std::string filePath,GLenum sampleMode) {
    shaderProgram = Engine::Engine::Instance().defaultShaderProgram;
    renderer = Engine::Engine::Instance().SpriteRenderer;
    texture = new Texture(filePath,shaderProgram,sampleMode);
    position = glm::vec3(0.0f,0.0f,0.0f);
    scale = glm::vec3(1.0f,1.0f,1.0f);
    rotation = 0.0f;
    setTexture(texture);
    REGISTER_UPDATE(Render);
}

void Engine::Sprite2D::Render(double deltaTime) {
    if (!texture) {
        std::cout << "Sprite2D::Render skipped: texture not set" << std::endl;
        return;
    }
    Scene* scene = Engine::Instance().getCurrentScene();
    Camera* camera = scene->getCamera();

    if (!camera) {
        std::cout << "ERROR: No camera found in scene" << std::endl;
        return;
    }

    glm::mat4 proj = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    shaderProgram->setUniformMatrix4fv("projection", proj);
    shaderProgram->setUniformMatrix4fv("view", view);
    shaderProgram->setUniformMatrix4fv("model", model);
    shaderProgram->setVec2("uFlip", glm::vec2(flipX ? 1.0f : 0.0f, flipY ? 1.0f : 0.0f));

    // Respect texture alpha while still writing depth so nearer sprites occlude farther ones
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Temporarily disable depth testing for 2D sprite pass to avoid being hidden by tile depth writes
    GLboolean depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    if (depthWasEnabled) {
        glDisable(GL_DEPTH_TEST);
    }

    shaderProgram->setBool("debugOverride", false);
    renderer->render(0, 6, GL_TRIANGLES, texture, model);

    if (depthWasEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
}

Engine::Camera::Camera(ShaderProgram* shaderProgram) : shaderProgram(shaderProgram) {
    position = glm::vec3(0.0f);
    zoom = 1.0f;
}

glm::mat4 Engine::Camera::getViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, -position);
    return view;
}

glm::mat4 Engine::Camera::getProjectionMatrix() const {
    float width = static_cast<float>(WindowStartupConfig::width);
    float height = static_cast<float>(WindowStartupConfig::height);
    float halfWidth = (width / 2.0f) / zoom;
    float halfHeight = (height / 2.0f) / zoom;

    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
}

void Engine::Camera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
}

void Engine::Camera::setZoom(float newZoom) {
    zoom = newZoom > 0.0f ? newZoom : 0.1f;
}

Engine::Sprite2D::Sprite2D(GLenum sampleMode) {
    shaderProgram = Engine::Engine::Instance().defaultShaderProgram;
    renderer = Engine::Engine::Instance().SpriteRenderer;
    texture = nullptr;
    position = glm::vec3(0.0f,0.0f,0.0f);
    scale = glm::vec3(1.0f,1.0f,1.0f);
    rotation = 0.0f;
    REGISTER_UPDATE(Render);
}

void Engine::Sprite2D::setTexture(Texture* tex) {
    texture = tex;
    if (!renderer) {
        renderer = Engine::Engine::Instance().SpriteRenderer;
    }
    if (!texture) {
        return;
    }
    vertexData = generateTextureVertices(texture->width,texture->height);
    renderer->loadData(vertexData.data(),20,indices,6);
    renderer->setVAtributes(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),0);
    renderer->setVAtributes(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),3 * sizeof(float));
}
