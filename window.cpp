#include "window.h"
#include <QCoreApplication>
#include <Qt>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QPoint>


#include<QDebug>


static const char* kVertexShaderFile="Shader.vertex.glsl";
static const char* kFragmentShaderFile="Shader.fragment.glsl";
static const char* kGeometryShaderFile="Shader.geometry.glsl";

Window::Window( QWindow* parent)
    : QWindow( parent )
{
   setSurfaceType( OpenGLSurface );

   QSurfaceFormat format;
   format.setDepthBufferSize( 24 );
   format.setMajorVersion( 4);
   format.setMinorVersion( 3 );
   format.setSamples(4);
   format.setProfile( QSurfaceFormat::CoreProfile );
   format.setOption(QSurfaceFormat::DebugContext );
   setFormat(format);

    device_=nullptr;

    context_ = new QOpenGLContext;
    context_->setFormat( requestedFormat() );
    context_->create();
    context_->makeCurrent( this );

}

Window::~Window(){


    delete logger_;
    delete context_;
    delete device_;

}

void Window::onMessageLogged( QOpenGLDebugMessage message )
{
    qDebug() << message;
}

void Window::Initialize(){
    context_->makeCurrent( this );
    device_ = new QOpenGLPaintDevice;
    initializeOpenGLFunctions();

    logger_ = new QOpenGLDebugLogger(this);
    logger_->initialize();
    connect(logger_,  SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
            this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ),
            Qt::DirectConnection );
    logger_->startLogging();


    program_.Initialize(kVertexShaderFile, kFragmentShaderFile, kGeometryShaderFile);
    bezier_.Initialize();


    glViewport(0, 0, width(), height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


}


void Window::render(){
   if (!isExposed()) return;

   context_->makeCurrent( this );

   if(!device_)  Initialize();

   glEnable(GL_PROGRAM_POINT_SIZE);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   bezier_.Draw(program_);


   QPainter painter(device_);
   render(&painter);
   context_->swapBuffers(this);

}

void Window::render(QPainter *painter)
{
    Q_UNUSED(painter);
}


void Window::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    if(!device_)  Initialize();
    glViewport(0, 0, width(), height());
    if (isExposed()) render();
}


void Window::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}

void Window::keyPressEvent(QKeyEvent *event){
    if(Qt::Key_Escape == event->key() ) QCoreApplication::exit();
}



void Window::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton){
        GLfloat v[2];
        v[0]=(2.0f*event->pos().x())/static_cast<GLfloat>(width())-1.0f;
        v[1]=-(2.0f*event->pos().y())/static_cast<GLfloat>(height())+1.0f;
        if(bezier_.AddPoint(v)) QCoreApplication::postEvent(this, new QEvent(QEvent::Expose));
    }
}


