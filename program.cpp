#include "program.h"
#include <iostream>
#include <fstream>

#include <QDebug>
#include <QCoreApplication>

using namespace std;

Program::~Program(){
    glUseProgram(0);

    glDetachShader(bezier_program_, fragment_shader_);
    glDetachShader(bezier_program_, vertex_shader_);
    glDetachShader(bezier_program_, geometry_shader_);

    glDetachShader(program_, fragment_shader_);
    glDetachShader(program_, vertex_shader_);

    glDeleteShader(geometry_shader_);
    glDeleteShader(fragment_shader_);
    glDeleteShader(vertex_shader_);

    glDeleteProgram(program_);
    glDeleteProgram(bezier_program_);
}

void Program::Initialize(const char *vertex, const char *fragment, const char *geometry){
    initializeOpenGLFunctions();
    vertex_shader_=LoadAndCompileShaderOrDie(vertex,GL_VERTEX_SHADER);
    fragment_shader_=LoadAndCompileShaderOrDie(fragment,GL_FRAGMENT_SHADER);
    geometry_shader_=LoadAndCompileShaderOrDie(geometry,GL_GEOMETRY_SHADER);

    program_ = LinkProgramOrDie(vertex_shader_, fragment_shader_);
    bezier_program_ = LinkProgramOrDie(vertex_shader_, fragment_shader_, geometry_shader_);
}


GLuint Program::LoadAndCompileShaderOrDie(const char * source_file, GLenum type){
    long int file_size;
    char * shader_code;
    GLuint shader=glCreateShader(type);
    ifstream file (source_file, ios::in|ios::ate|ios::binary);
    if (file.is_open()) {
        file_size = file.tellg();
        shader_code = new char [static_cast<unsigned int> (file_size + 1)];
        file.seekg (0, ios::beg);
        file.read (shader_code, file_size);
        shader_code[file_size]='\0';
        file.close();

        glShaderSource(shader, 1, static_cast<GLchar**>(&shader_code), nullptr);
        glCompileShader(shader);
        delete[] shader_code;

    }
    else{ //file was not opened
        cerr<<"Could not open the file "<<source_file<<endl;
        QCoreApplication::exit(EXIT_FAILURE);
    }
    GLint  compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        switch(type){
        case GL_VERTEX_SHADER:
            cerr << "vertex ";
        break;
        case GL_FRAGMENT_SHADER:
            cerr << "fragment ";
        break;
        case GL_GEOMETRY_SHADER:
            cerr << "geometry ";
        break;
        }
        cerr << "shader is failed to compile ("<<source_file<<"):"<< endl;
        GLint  log_size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        char* log_msg = new char[static_cast<GLuint>(log_size)];
        glGetShaderInfoLog(shader, log_size, nullptr, log_msg);
        cerr << log_msg << endl;
        delete [] log_msg;
        QCoreApplication::exit(EXIT_FAILURE);
    }
    return shader;
}


GLuint Program::LinkProgramOrDie(GLuint vertex_shader, GLuint fragment_shader, GLuint geometry_shader){
    GLuint new_program = glCreateProgram();
    glAttachShader(new_program, vertex_shader);
    glAttachShader(new_program, fragment_shader);

    if(geometry_shader){
        glAttachShader(new_program, geometry_shader);
    }

    glLinkProgram(new_program);

    GLint  linked;
    glGetProgramiv(new_program, GL_LINK_STATUS, &linked);
    if ( !linked ) {
        cerr << "Shader program failed to link" << endl;
        GLint  log_size;
        glGetProgramiv(new_program, GL_INFO_LOG_LENGTH, &log_size);
        char* log_msg = new char[static_cast<GLuint>(log_size)];
        glGetProgramInfoLog(new_program, log_size, nullptr, log_msg);
        cerr << log_msg << endl;
        delete [] log_msg;
        QCoreApplication::exit(EXIT_FAILURE);
    }

    return new_program;
}

