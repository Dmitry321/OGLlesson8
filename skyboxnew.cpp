#include "skyboxnew.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImage>
#include <QOpenGLFunctions>

//#include "material.h"
//#include "simpleobject3d.h"

SkyBoxNew::SkyBoxNew(float width, const QVector<QImage> &textures) :
    m_texture(nullptr), //QOpenGLTexture::TargetCubeMap)
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    // если будет вызыватся повторно надо сначало удалить буферы
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();


    float with_div_2 = width / 2.0f;
    QVector<QVector3D> vertexes;
    // добавляем точки куба в массив
    //  первая грань если смотрим в положит направлении оси z к токе 0,0 (все z положительные)
    //                                         x          y           z        textures               normal
    // Первая грань это позади нас когда мы внутри skybox (ближняя к наблюдателю)
    vertexes.append(QVector3D(-with_div_2, with_div_2, with_div_2));
    vertexes.append(QVector3D(-with_div_2, -with_div_2, with_div_2));
    vertexes.append(QVector3D(with_div_2, with_div_2, with_div_2));  // правая верхняя вершина
    vertexes.append(QVector3D(with_div_2, -with_div_2, with_div_2));  // правая нижняя вершина

    // справа от нас
    vertexes.append(QVector3D(with_div_2, with_div_2, with_div_2));
    vertexes.append(QVector3D(with_div_2, -with_div_2, with_div_2));
    vertexes.append(QVector3D(with_div_2, with_div_2, -with_div_2));  // правая верхняя вершина
    vertexes.append(QVector3D(with_div_2, -with_div_2, -with_div_2));  // правая нижняя вершина

    // Над нами сверху
    vertexes.append(QVector3D(with_div_2, with_div_2, with_div_2));
    vertexes.append(QVector3D(with_div_2, with_div_2, -with_div_2));
    vertexes.append(QVector3D(-with_div_2, with_div_2, with_div_2));  // правая верхняя вершина
    vertexes.append(QVector3D(-with_div_2, with_div_2, -with_div_2));  // правая нижняя вершина

    // Перед нами
    vertexes.append(QVector3D(with_div_2, with_div_2, -with_div_2));
    vertexes.append(QVector3D(with_div_2, -with_div_2, -with_div_2));
    vertexes.append(QVector3D(-with_div_2, with_div_2, -with_div_2));  // правая верхняя вершина
    vertexes.append(QVector3D(-with_div_2, -with_div_2, -with_div_2));  // правая нижняя вершина

    // слева от нас
    vertexes.append(QVector3D(-with_div_2, with_div_2, with_div_2));
    vertexes.append(QVector3D(-with_div_2, with_div_2, -with_div_2));
    vertexes.append(QVector3D(-with_div_2, -with_div_2, with_div_2));  // правая верхняя вершина
    vertexes.append(QVector3D(-with_div_2, -with_div_2, -with_div_2));  // правая нижняя вершина

    // снизу под нами
    vertexes.append(QVector3D(-with_div_2, -with_div_2, with_div_2));
    vertexes.append(QVector3D(-with_div_2, -with_div_2, -with_div_2));
    vertexes.append(QVector3D(with_div_2, -with_div_2, with_div_2));  // правая верхняя вершина
    vertexes.append(QVector3D(with_div_2, -with_div_2, -with_div_2));  // правая нижняя вершина

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


    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexes.constData(), static_cast<int>(static_cast<ulong>(vertexes.size()) * sizeof (QVector3D)) ); // выделение в видеопамяти нужное кол-во байт и скопирует указанные данные в видеопамять
    m_vertexBuffer.release(); // можно пока освободить

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), static_cast<int>(static_cast<ulong>(indexes.size()) * sizeof(GLuint)));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    m_texture->setSize(textures.at(0).width(), textures.at(0).height(), textures.at(0).depth());
    m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_texture->allocateStorage();

    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         textures.at(0).convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         textures.at(1).convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         textures.at(2).convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         textures.at(3).convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

    m_texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         textures.at(4).convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);

    m_texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         textures.at(5).convertToFormat(QImage::Format_RGBA8888).constBits(), Q_NULLPTR);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

}

SkyBoxNew::~SkyBoxNew()
{
    delete m_texture;
    delete m_program;
}

void SkyBoxNew::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
//    modelMatrix.translate(m_translate);
//    modelMatrix.rotate(m_rotate);
//    modelMatrix.scale(m_scale);
//    modelMatrix = m_globalTransform * modelMatrix;


    m_texture->bind(0);
    program->setUniformValue("u_modelMatrix", modelMatrix);
    program->setUniformValue("u_texture", 0);

    m_vertexBuffer.bind();


    // указываем шейдеру аттрибуты
    int vertLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertLoc);
    program->setAttributeBuffer(vertLoc,GL_FLOAT, 0, 3, sizeof (QVector3D));


    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr); //первый аргумент это примитивы которые рисовать, второй количество элементов для отрисовки далее тип и массив для отрисовки но так как мы используем буфер то здесь просто 0

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();

    program->setUniformValue("uTexture", 0);
}

void SkyBoxNew::rotate(const QQuaternion &r)
{
    (void)r;
}

void SkyBoxNew::translate(const QVector3D &t)
{
    (void)t;
}

void SkyBoxNew::scale(const float &s)
{
    (void)s;
}

void SkyBoxNew::setGlobalTransform(const QMatrix4x4 &g)
{
    (void)&g;
}
