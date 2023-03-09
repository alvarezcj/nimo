#include "Shader.h"
#include "glad/glad.h"
#include "core/Log.h"

#include <fstream>
#include <sstream>

nimo::Shader::Shader(const std::string& filename)
{
    NIMO_DEBUG("nimo::Shader::Shader({})", filename);
    try 
    {
        std::ifstream vShaderFile;
        // open files
        vShaderFile.open(filename);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        // convert stream into string
        std::string code = vShaderStream.str();
        std::string vertexCode =  code.substr(code.find("@VERTEX", 0) + 7, code.find("@VERTEXEND", 0) - code.find("@VERTEX", 0) - 7);
        std::string fragmentCode = code.substr(code.find("@FRAGMENT", 0) + 9, code.find("@FRAGMENTEND", 0) - code.find("@FRAGMENT", 0) - 9);
                const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    catch (std::ifstream::failure& e)
    {
        NIMO_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: {}", filename);
    }
}
nimo::Shader::~Shader()
{
    NIMO_DEBUG("nimo::Shader::~Shader");
    glDeleteProgram(ID);
}
void nimo::Shader::use()
{
    glUseProgram(ID);
}
void nimo::Shader::set(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void nimo::Shader::set(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void nimo::Shader::set(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
void nimo::Shader::set(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void nimo::Shader::set(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void nimo::Shader::set(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void nimo::Shader::set(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
}
void nimo::Shader::set(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}
void nimo::Shader::set(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
}
void nimo::Shader::set(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void nimo::Shader::set(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void nimo::Shader::set(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void nimo::Shader::checkCompileErrors(GLuint shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            NIMO_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {}\n {}", type , infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            NIMO_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {}\n {}", type , infoLog);
        }
    }
}