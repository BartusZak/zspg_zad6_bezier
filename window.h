#ifndef WINDOW_H
#define WINDOW_H


#include "program.h"
#include "bezier.h"
#include "glfunctions.h"

#include <QtGui/QWindow>
#include <QtGui/QScreen>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGui/QOpenGLDebugLogger>

class Window : public QWindow, protected GLFunctions{
    Q_OBJECT
public:
    explicit Window(QWindow* parent=nullptr);
    virtual ~Window();
    virtual void render(QPainter *painter);

public slots:
    void render();

protected:
    void exposeEvent(QExposeEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void keyPressEvent(QKeyEvent *event);

private:
    QOpenGLContext *context_;
    QOpenGLPaintDevice* device_;
    QOpenGLDebugLogger* logger_;

    void Initialize();

    Program program_;
    Bezier bezier_;
private slots:
    void onMessageLogged( QOpenGLDebugMessage message );
};

#endif // WINDOW_H
