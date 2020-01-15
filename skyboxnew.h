#ifndef SKYBOXNEW_H
#define SKYBOXNEW_H
#include "transformational.h"
#include <QVector>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

class QImage;
class QOpenGLShaderProgram;
class QOpenGLFunctions;
class QOpenGLTexture;
//class Material;

class SkyBoxNew : public Transformational
{
public:
    SkyBoxNew(float width, const QVector<QImage> &texture);
    ~SkyBoxNew();
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);
private:
    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *m_texture;

    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;

   // QVector<QOpenGLTexture *> m_textures;

   // QQuaternion m_rotate;
   // QVector3D m_translate;
   // float m_scale;
   // QMatrix4x4 m_globalTransform;

    //Material *m_material;

};

#endif // SKYBOXNEW_H
