#ifndef PROGRAM_H
#define PROGRAM_H


#include "glfunctions.h"

class Program:protected GLFunctions{
public:
    ~Program();
    void Initialize(const char * vertex, const char* fragment, const char * geometry);

    operator GLuint(){return program_;}
    GLuint BezierProgram(){return bezier_program_;}

private:

    GLuint LoadAndCompileShaderOrDie(const char* source_file, GLenum type);
    GLuint LinkProgramOrDie(GLuint vertex_shader, GLuint fragment_shader, GLuint geometry_shader=0);

    GLuint program_;
    GLuint bezier_program_;

    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint geometry_shader_;

};

#endif // PROGRAM_H
