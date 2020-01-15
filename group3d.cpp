#include "group3d.h"

Group3D::Group3D()
{
    m_scale = 1.0f;
}

void Group3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for(auto o: m_objects)
        o->setGlobalTransform(localMatrix);

}

void Group3D::translate(const QVector3D &t)
{
    m_translate += t;
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for(auto o: m_objects)
        o->setGlobalTransform(localMatrix);
}

void Group3D::scale(const float &s)
{
     m_scale *= s;
     QMatrix4x4 localMatrix;
     localMatrix.setToIdentity();
     localMatrix.translate(m_translate);
     localMatrix.rotate(m_rotate);
     localMatrix.scale(m_scale);
     localMatrix = m_globalTransform * localMatrix;

     for(auto o: m_objects)
         o->setGlobalTransform(localMatrix);
}

void Group3D::setGlobalTransform(const QMatrix4x4 &g)
{
    m_globalTransform = g;
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    for(auto o: m_objects)
        o->setGlobalTransform(localMatrix);
}

void Group3D::addObject(Transformational *obj)
{
    m_objects.append(obj);
    QMatrix4x4 localMatrix;
    localMatrix.setToIdentity();
    localMatrix.translate(m_translate);
    localMatrix.rotate(m_rotate);
    localMatrix.scale(m_scale);
    localMatrix = m_globalTransform * localMatrix;

    m_objects.last()->setGlobalTransform(localMatrix);
}

void Group3D::delObject(Transformational *obj)
{
    //m_objects.removeOne(obj);
    m_objects.removeAll(obj);
}

void Group3D::delObject(const int &index)
{
    m_objects.remove(index);
}

void Group3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    for(auto o: m_objects)
        o->draw(program, functions);
}
