//
//  Shader.hpp
//  opengldemo
//
//  Created by 郭海 on 2017/12/22.
//  Copyright © 2017年 gh. All rights reserved.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>


class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        //顶点 片段 字符
        std::string vertexCode, fragmentCode;
        //顶点 片段 文件流
        std::ifstream vShaderFile, fShaderFile;
        // 保证ifstream对象可以抛出异常：
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            //打开渲染文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStram;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStram << fShaderFile.rdbuf();
            //
            vShaderFile.close();
            fShaderFile.close();
            
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStram.str();
        } catch (std::ifstream::failure e) {
            std::cout<<"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"<<e.std::exception::what()<<std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        
        
        //创建顶点着色器
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileError(vertex, "VERTEX");
        
        //片段着色器
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment,1,&fShaderCode,nullptr);
        glCompileShader(fragment);
        checkCompileError(fragment, "FRAGMENT");
        
        ID=glCreateProgram();
        glAttachShader(ID,vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileError(ID, "PROGRAM");
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
    }
    
    void use()
    {
        glUseProgram(ID);
    }
    //设置uniform的bool值
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID,name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setMat4(const std::string &name, glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
private:
    void checkCompileError(unsigned int shader,std::string type)
    {
        int success;
        char infoLog[1024];
        if(type != "PROGRAM")
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
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            
        }
    }
};
#endif
