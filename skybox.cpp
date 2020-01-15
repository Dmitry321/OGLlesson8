#include "skybox.h"
#include <QImage>
#include "simpleobject3d.h"
#include "material.h"

SkyBox::SkyBox(float width, const QImage &texture)
{
    float with_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    // добавляем точки куба в массив
    //  первая грань если смотрим в положит направлении оси z к токе 0,0 (все z положительные)
    //                                         x          y           z        textures               normal
    // Первая грань это позади нас когда мы внутри skybox (ближняя к наблюдателю)
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, with_div_2), QVector2D(1.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, with_div_2), QVector2D(1.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, with_div_2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, with_div_2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));  // правая нижняя вершина

    // справа от нас
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, with_div_2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, with_div_2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, -with_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, -with_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));  // правая нижняя вершина

    // Над нами сверху
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, with_div_2), QVector2D(2.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, -with_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, with_div_2), QVector2D(1.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, -with_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));  // правая нижняя вершина

    // Перед нами
    vertexes.append(VertexData(QVector3D(with_div_2, with_div_2, -with_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, -with_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, -with_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, -with_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));  // правая нижняя вершина

    // слева от нас
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, with_div_2), QVector2D(0.0f, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, with_div_2, -with_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, with_div_2), QVector2D(0.0, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, -with_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));  // правая нижняя вершина

    // снизу под нами
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, with_div_2), QVector2D(1.0f/4.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -with_div_2, -with_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, with_div_2), QVector2D(2.0f/4.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -with_div_2, -with_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 1.0, 0.0)));  // правая нижняя вершина

    // массив индексов. Вершин у нас 24
    QVector<GLuint> indexes;
    for(uint i = 0; i < 24; i += 4)
    {
        // порядок обхода треугольников против часовой стрелки если мы внутри куба то индексы последний с предпоследним надо поменять местами у каждого тругольника
        indexes.append(i+0);
        indexes.append(i+2);
        indexes.append(i+1);

        indexes.append(i+2);
        indexes.append(i+3);
        indexes.append(i+1);
    }
    Material *newMtl = new Material;
    newMtl->setDiffuseMap(texture);
    newMtl->setShinnes(96);
    newMtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));

    m_cube = new SimpleObject3D(vertexes, indexes, newMtl);

}

SkyBox::~SkyBox()
{
    delete m_cube;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    m_cube->draw(program, functions);
}

void SkyBox::rotate(const QQuaternion &r)
{
    (void)r;
}

void SkyBox::translate(const QVector3D &t)
{
    (void)t;
}

void SkyBox::scale(const float &s)
{
    (void)s; // чтобы убрать предупреждения об используемой переменной
}

void SkyBox::setGlobalTransform(const QMatrix4x4 &g)
{
    (void)&g;
}
