#ifndef SIMPLEOBJECT3D_H
#define SIMPLEOBJECT3D_H
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include "transformational.h"

//#include <QOpenGLTexture>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class Material;

struct VertexData
{
    VertexData(){
    }
    VertexData(QVector3D p, QVector2D t, QVector3D n):
        position(p), textCoord(t), normal(n)
    {

    }
    QVector3D position;  // позиция точки
    QVector2D textCoord; // текстурные координаты
    QVector3D normal; // нормаль к данной точке
    QVector3D tangent; // для карты нормалей
    QVector3D bitangent;

};

class SimpleObject3D : public Transformational
{
public:
    SimpleObject3D();
    //SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, const QImage &texture);
    SimpleObject3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material *material);
    //
    ~SimpleObject3D();

    //void init(const QVector<VertexData> &vertData, const QVector<GLushort> &indexes, const QImage &texture);
    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes, Material *material);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    //void translate(const QVector3D &translateVector);

    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    //QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_diffuseMap;
    QOpenGLTexture *m_normalMap;
    //QVector<QOpenGLTexture *> m_textures;

    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    Material *m_material;
};

#endif // SIMPLEOBJECT3D_H
