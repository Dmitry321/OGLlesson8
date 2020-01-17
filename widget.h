#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QVector>
#include <QBasicTimer>
#include <QOpenGLFramebufferObject>
//#include "simpleobject3d.h"

class ObjectEngine3D; //SimpleObject3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;
class SkyBoxNew;
class QOpenGLFramebufferObject;
class Light;

//struct VertexData
//{
//    VertexData(){
//    }
//    VertexData(QVector3D p, QVector2D t, QVector3D n):
//        position(p), textCoord(t), normal(n)
//    {

//    }
//    QVector3D position;  // позиция точки
//    QVector2D textCoord; // текстурные координаты
//    QVector3D normal; // нормаль к данной точке

//};

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:  // вызываются только внутри этого класса
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initCube(float width, float height, float dept, QImage *diffuseMap = 0, QImage *normalMap = 0);
   // void loadObj(const QString &path);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_projectionLightMatrix;
//    QMatrix4x4 m_lightMatrix;

//    float m_lightRotateX;
//    float m_lightRotateY;

//    QMatrix4x4 m_shadowLightMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programDepth;
    QOpenGLShaderProgram m_programSkyBox;
    QOpenGLShaderProgram m_programSkyBox2;
    //QOpenGLTexture *m_texture;
    //QOpenGLBuffer m_arrayBuffer;
    //QOpenGLBuffer m_indexBuffer;
    QVector2D m_mousePosition;
    // Объект дял поворота содержит вектор и угол поворота
    //QQuaternion m_rotation;
    QVector<Group3D *> m_groups;
    QVector<ObjectEngine3D *> m_objects;
    QVector<Transformational *> m_transformObjects;
    QBasicTimer m_timer;

    //float m_z; // сдвиг по оси Z для камеры
    float angleObject;
    float angleGroup1;
    float angleGroup2;
    float angleMain;

    Camera3D * m_camera;
    SkyBox *m_skybox;
    SkyBoxNew *m_skybox2;
    // буффер для хранения расстояний от точек до источника света
    QOpenGLFramebufferObject *m_depthBuffer;
    quint32 m_fbHeight;
    quint32 m_fbWidth;

    Light *m_light[3];


};

#endif // WIDGET_H
