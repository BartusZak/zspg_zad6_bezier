#ifndef BEZIER_H
#define BEZIER_H

#include "glfunctions.h"
#include "program.h"

#include<QtGui/QOpenGLVertexArrayObject>


class Bezier: protected GLFunctions{
    public:
        void Initialize();
        void Draw(Program & );
        ~Bezier();
        Bezier(void);
        bool AddPoint(GLfloat*v);

    private:

        GLuint max_n_;
        GLint n_;


        GLuint vao_;

        GLuint vertices_;
};

#endif // BEZIER_H
