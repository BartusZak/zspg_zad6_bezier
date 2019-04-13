#include "bezier.h"

#include <QDebug>

Bezier::Bezier(void){
    max_n_=4;
    n_=0;
}

Bezier::~Bezier(){
    glBindVertexArray(vao_);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vertices_);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
}

void Bezier::Initialize(){
    initializeOpenGLFunctions();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertices_);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_);
    glBufferData(GL_ARRAY_BUFFER, max_n_*2*sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}



void Bezier::Draw(Program &prog){
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
    glBindVertexArray(vao_);

    glUseProgram(prog);

    glDrawArrays(GL_LINE_STRIP, 0, n_);
    glDrawArrays(GL_POINTS, 0, n_);

    if(n_ == static_cast<GLint>(max_n_)){
        glUseProgram(prog.BezierProgram());
        glDrawArrays(GL_LINES_ADJACENCY, 0, n_);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

bool Bezier::AddPoint(GLfloat *v){
    if(n_ == static_cast<GLint>(max_n_)) return false;

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLuint>(n_++)*2*sizeof(float), 2*sizeof(float), &v[0]);
    glBindVertexArray(0);
    return true;
}

