#include "widget.h"
#include <QOpenGLContext>
#include <QMouseEvent>
#include <QtMath>
#include "objectengine3d.h"
#include <QKeyEvent>
#include "group3d.h"
#include "camera3d.h"
#include "skybox.h"
#include "skyboxnew.h"
#include "material.h"
#include "light.h"



Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    //m_z = -5.0f;
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
    m_fbHeight = 1024;
    m_fbWidth = 1024;
    angleObject = 0.0f;
    angleGroup1 = 0.0f;
    angleGroup1 = 0.0f;
    angleMain = 0.0f;

    m_projectionLightMatrix.setToIdentity();
    m_projectionLightMatrix.ortho(-40, 40, -40, 40, -40, 40);

    m_light[0] = new Light(Light::Spot);
    m_light[0]->setPosition(QVector4D(10.0f, 10.0f, 10.0f, 1.0f));
    m_light[0]->setDirection(QVector4D(-1.0f, -1.0f, -1.0f, 0.0));
    m_light[0]->setDiffuseColor(QVector3D(1.0f, 0.0f, 0.0f));
    m_light[0]->setCutoff(10.0f / 180.0f * M_PI );

    m_light[1] = new Light(Light::Spot);
    m_light[1]->setPosition(QVector4D(-10.0f, 10.0f, 10.0f, 1.0f));
    m_light[1]->setDirection(QVector4D(1.0f, -1.0f, -1.0f, 0.0));
    m_light[1]->setDiffuseColor(QVector3D(0.0f, 1.0f, 0.0f));
    m_light[1]->setCutoff(12.0f / 180.0f * M_PI );

    m_light[2] = new Light(Light::Directional);
    m_light[2]->setPosition(QVector4D(10.0f, 10.0f, -10.0f, 1.0f));
    m_light[2]->setDirection(QVector4D(-1.0f, -1.0f, 1.0f, 0.0));
    m_light[2]->setDiffuseColor(QVector3D(0.0f, 0.0f, 1.0f));
    m_light[2]->setCutoff(8.0f / 180.0f * M_PI );


//    m_lightRotateX = 30; // угол вращения света в градусах
//    m_lightRotateY = 40;

//    m_shadowLightMatrix.setToIdentity();
//    m_shadowLightMatrix.rotate(m_lightRotateX, 1.0, 0.0, 0.0);
//    m_shadowLightMatrix.rotate(m_lightRotateY, 0.0, 1.0, 0.0);

//    // чтобы освещение совпадало с тенями надо наоборот вращать второй вектор света
//    m_lightMatrix.setToIdentity();
//    m_lightMatrix.rotate(-m_lightRotateY, 0.0, 1.0, 0.0);
//    m_lightMatrix.rotate(-m_lightRotateX, 1.0, 0.0, 0.0);


}

Widget::~Widget()
{
    delete m_camera;
    for(auto o: m_objects)
        delete o;

    for(auto o: m_groups)
        delete o;

  //  for(auto o: m_transformObjects)
  //      delete o;
}

// вызывается один раз в самом начале
void Widget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); // включает буфер глубины
    glEnable(GL_CULL_FACE);  // отсечение задних граней



    initShaders();

    QImage diffuseMap(":/stone2.png");
    QImage normalMap(":/normal_3.jpg");


//    initCube(2.0f);
//    m_objects[0]->translate(QVector3D(-0.2f, 0.0f, 0.0f));

//    initCube(1.0f);
//    m_objects[1]->translate(QVector3D(0.8f, 0.0f, 0.0f));

    //float step = 2.0f;

    float x = 0.0, y = 0.0, z = 0.0;

    m_groups.append(new Group3D);
    //m_groups[0]->addObject(m_camera);
//    for(float x = -step; x <= step; x += step){
//        for(float y = -step; y <= step; y += step){
//            for(float z = -step; z <= step; z += step){
                m_objects.append(new ObjectEngine3D);
                m_objects.last()->loadObjectFromFile(":/monkey1.obj");

                //initCube(1.0f, 1.0f, 1.0f, &diffuseMap, &normalMap);
                m_objects.last()/*[m_objects.size() - 1 ]*/->translate(QVector3D(x, y, z));
                m_groups.last()/*[m_groups.size() - 1 ]*/->addObject(m_objects.last()/*[m_objects.size() - 1 ]*/);
//            }
//        }
//    }
    m_groups.last()/*[0]*/->translate(QVector3D(-4.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);
//    for(float x = -step; x <= step; x += step){
//        for(float y = -step; y <= step; y += step){
//            for(float z = -step; z <= step; z += step){
              m_objects.append(new ObjectEngine3D);
              m_objects.last()->loadObjectFromFile(":/monkey1.obj");
              m_objects.last()->translate(QVector3D(x, y, z));
              m_groups.last()->addObject(m_objects.last()/*[m_objects.size() - 1 ]*/);

//                initCube(1.0f, 1.0f, 1.0f, &diffuseMap, &normalMap);
//                m_objects[m_objects.size() - 1 ]->translate(QVector3D(x, y, z));
//                m_groups[m_groups.size() - 1 ]->addObject(m_objects[m_objects.size() - 1 ]);
//            }
//        }
//    }
    m_groups.last()/*[1]*/->translate(QVector3D(4.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);
    m_groups.last()/*[2]*/->addObject(m_groups[0]);
    m_groups.last()/*[2]*/->addObject(m_groups[1]);

    m_transformObjects.append(m_groups.last()/*[2]*/);

    //m_objects.append(new ObjectEngine3D);
   // m_objects.last()->loadObjectFromFile(":/monkey2.obj");
    //loadObj(":/monkey1.obj");
    initCube(2.0f, 2.0f, 2.0f, &diffuseMap, &normalMap);
    m_transformObjects.append(m_objects.last());

    QImage tmp(":/mars.jpg");
    initCube(40.0f, 2.0f, 40.0f, &tmp);
    m_objects.last()->translate(QVector3D(0.0, -2.0, 0.0));
    m_transformObjects.append(m_objects.last());

  //  m_groups[0]->addObject(m_camera);

    QVector<QImage> images;
    //for (int j = 0; j < 6; ++j)
        images.append(QImage(":/skybox/skybox/front.jpg")/*.arg(j + 1))*/);
        images.append(QImage(":/skybox/skybox/back.jpg"));
        images.append(QImage(":/skybox/skybox/top.jpg").mirrored());
        images.append(QImage(":/skybox/skybox/bottom.jpg").mirrored());
        images.append(QImage(":/skybox/skybox/left.jpg"));
        images.append(QImage(":/skybox/skybox/right.jpg"));
    //m_skybox = new SkyBox(100, images);
   // m_skybox = new SkyBox(100, QImage(":/skybox.jpg"));
    m_skybox2 = new SkyBoxNew(100, images);

    m_depthBuffer = new QOpenGLFramebufferObject(m_fbWidth, m_fbHeight, QOpenGLFramebufferObject::Depth);

    m_timer.start(30, this);
}

// при изменении размера окна
void Widget::resizeGL(int w, int h)
{
    float aspect = w/(static_cast<float>(h));
    m_projectionMatrix.setToIdentity(); // создать единичную матрицу (единицы на главной диагонали)
    m_projectionMatrix.perspective(45, aspect,0.01f, 1000.0f);  //(угол усеченного конуса камеры в градусах, aspectratio, передняя и дальняя плоскости отсечения)

}

// вызывается при перерисовке содержимого
void Widget::paintGL()
{
    // Отрисовка во фрейм буффер
    m_depthBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0,0,m_fbWidth,m_fbHeight);

    m_programDepth.bind();
    m_programDepth.setUniformValue("u_projectionLightMatrix", m_projectionLightMatrix);
    m_programDepth.setUniformValue("u_shadowLightMatrix", m_light[0]->getLightMatrix());

    for(auto *s3d: m_transformObjects){
        s3d->draw(&m_programDepth, context()->functions());
    }
    m_programDepth.release();
    m_depthBuffer->release();

    // теперь надо получить указатель на текстуру
    GLuint texture = m_depthBuffer->texture();

    // забиндить текстуру но средствами OpenGl
    glActiveTexture(GL_TEXTURE4); // на какой слот биндить в данном случае на 4
    glBindTexture(GL_TEXTURE_2D, texture);


    // Отрисовка на экран - второй этап
    // вначале очищаем буфер цвета и буфер глубины
    glViewport(0,0,width(),height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //m_programSkyBox.bind();
   // m_programSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_programSkyBox2.bind();
    m_programSkyBox2.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_programSkyBox);
    //m_skybox->draw(&m_programSkyBox, context()->functions());
    m_skybox2->draw(&m_programSkyBox2, context()->functions());

   // m_programSkyBox.release();
    m_programSkyBox2.release();

    m_program.bind();
    m_program.setUniformValue("u_shadowMap", GL_TEXTURE4 - GL_TEXTURE0);
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
   // m_program.setUniformValue("u_viewMatrix", viewMatrix);
   // m_program.setUniformValue("u_lightDirection", QVector4D(0.0, 0.0, -1.0, 0.0)); // т.к. камера смотрит на ось z источник света должен в противоположную сторону от камеры светить
    m_program.setUniformValue("u_projectionLightMatrix", m_projectionLightMatrix);
    m_program.setUniformValue("u_shadowLightMatrix", m_light[0]->getLightMatrix());
   // m_program.setUniformValue("u_lightMatrix", m_lightMatrix);
    m_program.setUniformValue("u_lightPower", 1.0f);
    for(int i = 0; i < 3; ++i){
        m_program.setUniformValue(QString("u_lightProperty[%1].ambienceColor").arg(i).toLatin1().data(), m_light[i]->getAmbienceColor());
        m_program.setUniformValue(QString("u_lightProperty[%1].diffuseColor").arg(i).toLatin1().data(), m_light[i]->getDiffuseColor());
        m_program.setUniformValue(QString("u_lightProperty[%1].specularColor").arg(i).toLatin1().data(), m_light[i]->getSpecularColor());
        m_program.setUniformValue(QString("u_lightProperty[%1].position").arg(i).toLatin1().data(), m_light[i]->getPosition());
        m_program.setUniformValue(QString("u_lightProperty[%1].direction").arg(i).toLatin1().data(), m_light[i]->getDirection());
        m_program.setUniformValue(QString("u_lightProperty[%1].cutoff").arg(i).toLatin1().data(), m_light[i]->getCutoff());
        m_program.setUniformValue(QString("u_lightProperty[%1].type").arg(i).toLatin1().data(), m_light[i]->getType());
    }
    m_program.setUniformValue("u_countLights",3);
    m_program.setUniformValue("u_indexLightForShadow", 0);

    m_camera->draw(&m_program);
    for(auto *s3d: m_transformObjects){
    //for(int i = 0; i < m_objects.size(); ++i){
      //  m_objects[i]->draw(&m_program, context()->functions());
        s3d->draw(&m_program, context()->functions());
    }
    m_program.release();
}

void Widget::initShaders()
{
    // компиляция шейдеров
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
        close();
    //
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
        close();
    // Слинковать все шейдеры в один
    if(!m_program.link())
        close();
    // Bind shader pipeline for use
//    if (!m_program.bind())
//           close();

    // компиляция шейдеров
    if(!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vsh"))
        close();
    //
    if(!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.fsh"))
        close();
    // Слинковать все шейдеры в один
    if(!m_programSkyBox.link())
        close();
    // Bind shader pipeline for use
//    if (!m_programSkyBox.bind())
//           close();

    // компиляция шейдеров
    if(!m_programSkyBox2.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox2.vsh"))
        close();
    //
    if(!m_programSkyBox2.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox2.fsh"))
        close();
    // Слинковать все шейдеры в один
    if(!m_programSkyBox2.link())
        close();
    // Bind shader pipeline for use
//    if (!m_programSkyBox2.bind())
//           close();

    if(!m_programDepth.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/depth.vsh"))
        close();

    if(!m_programDepth.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/depth.fsh"))
        close();

    if(!m_programDepth.link())
        close();


}

void Widget::initCube(float width, float height, float depth, QImage *diffuseMap, QImage *normalMap)
{
    float with_div_2 = width / 2.0f;
    float height_div_2 = height / 2.0f;
    float depth_div_2 = depth / 2.0f;


    QVector<VertexData> vertexes;
    // добавляем точки куба в массив
    //  первая грань если смотрим в положит направлении оси z к токе 0,0 (все z положительные)
    //                                         x          y           z        textures               normal
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));  // правая нижняя вершина

    // вторая грань если смотрим в положит направлении оси x к токе 0,0 (все x положительные)
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, depth_div_2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, -depth_div_2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, -depth_div_2), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, depth_div_2), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));  // правая нижняя вершина

    // третья грань если смотрим в положит направлении оси y к токе 0,0(все y положительные)
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));  // правая нижняя вершина

    // 4 грань если смотрим в отрицат направлении оси z к токе 0,0 (все z отрицат)
    vertexes.append(VertexData(QVector3D(with_div_2, height_div_2, -depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, -depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, -depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));  // правая нижняя вершина

    // 5 грань если смотрим в отрицат направлении оси x к токе 0,0 (все x отрицат)
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));  // правая нижняя вершина

    // 6 грань если смотрим в отрицат направлении оси y к токе 0,0(все y отрицат)
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, depth_div_2), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));  // правая верхняя вершина
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, depth_div_2), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-with_div_2, -height_div_2, -depth_div_2), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(with_div_2, -height_div_2, -depth_div_2), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));  // правая нижняя вершина

    // массив индексов. Вершин у нас 24
    QVector<GLuint> indexes;
    for(uint i = 0; i < 36; ++i)
    {
        indexes.append(i);
//        indexes.append(i+1);
//        indexes.append(i+2);
//        indexes.append(i+2);
//        indexes.append(i+1);
//        indexes.append(i+3);
    }

//    QPixmap pixmap;
//    pixmap.load(":/cube1.png");

//    QFile file("cube123.png");
//    file.open(QIODevice::WriteOnly);
//    pixmap.save(&file, "PNG");

    QImage img(":/cube1.png");

    Material *newMtl = new Material;
    //newMtl->setDiffuseMap(":/stone2.png");
    //newMtl->setNormalMap(":/normal_3.jpg");
    if(diffuseMap)
        newMtl->setDiffuseMap(*diffuseMap);
    if(normalMap)
        newMtl->setNormalMap(*normalMap);
    newMtl->setShinnes(96);
    newMtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));


    ObjectEngine3D *newObj = new ObjectEngine3D;
    newObj->calculateTBN(vertexes);
    newObj->addObject(new SimpleObject3D(vertexes, indexes, newMtl));
    m_objects.append(newObj);
    //m_objects.last()->init(vertexes, indexes, QImage(":/cube1.png"));// = new SimpleObject3D(vertexes, indexes, img); //(vertexes, indexes, QImage(":/cube1.png"));

}

//void Widget::loadObj(const QString &path)
//{
//    QFile objFile(path);
//    if(!objFile.exists())
//    {
//        qDebug() << "File not found";
//        return;
//    }
//    objFile.open(QIODevice::ReadOnly);

//    QTextStream input(&objFile);
//    QVector<QVector3D> coords;
//    QVector<QVector2D> texCoords;
//    QVector<QVector3D> normals;

//    QVector<VertexData> vertexes;
//    QVector<GLuint> indexes;

//    while (!input.atEnd()) {
//        QString str = input.readLine();
//        QStringList list = str.split(" ");

//        if (list.at(0) == "#"){
//            qDebug() << "This is comment: " << str;
//            continue;
//        }
//        else if (list.at(0) == "mtllib"){
//            // Обработать файл с материалами
//            qDebug() << "File with material: " << str;
//            continue;
//        }
//        else if (list.at(0) == "o"){
//            continue;
//        }
//        else if (list.at(0) ==  "v"){
//            coords.append(QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat()));
//            continue;
//        }
//        else if (list.at(0) ==  "vt"){
//            texCoords.append(QVector2D(list.at(1).toFloat(), list.at(2).toFloat()));
//            continue;
//        }
//        else if (list.at(0) ==  "vn"){
//            normals.append(QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat()));
//            continue;
//        }
//        else if (list.at(0) ==  "f"){ // полигоны
//            for(int i = 1; i <= 3; ++i)
//            {
//                QStringList vert = list.at(i).split("/");
//                vertexes.append(VertexData(coords.at(vert.at(0).toLong() - 1), texCoords.at(vert.at(1).toLong() - 1), normals.at(vert.at(2).toLong() - 1) ));
//                indexes.append(indexes.size());
//            }
//            continue;
//        }

//    }
//    objFile.close();

//    m_objects.append(new SimpleObject3D(vertexes, indexes, QImage(":/stone2.png")));
//}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        m_mousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton) return;

    QVector2D diffPos = QVector2D(event->localPos()) - m_mousePosition;
    m_mousePosition = QVector2D(event->localPos());

    float angel = diffPos.length() / 2.0f; // угол поворота

    // Точка вокруг которой осуществлять поворот - ось вращения
    QVector3D axis = QVector3D(diffPos.y(), diffPos.x(), 0.0);

    //m_rotation = QQuaternion::fromAxisAndAngle(axis, angel) * m_rotation; // умножаем на уже совершенные повороты
    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angel));

    update();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0){
        //m_z += 0.25f;
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    }
    else if(event->delta() < 0)
    {
        //m_z -= 0.25f;
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }
    update();
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    int i = 0;
    int lastIndex = m_objects.size() - 2;
    for(auto o: m_objects){
        if(i >= lastIndex)
            break;
        if(i % 2 == 0){
            o->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleObject)));
            o->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qCos(angleObject)));
        }
        else{
            o->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, qSin(angleObject)));
            o->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleObject)));
        }
        i++;
    }

    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, qSin(angleGroup1)));
    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qSin(angleGroup1)));

    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qCos(angleGroup2)));
    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qCos(angleGroup2)));

    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, qSin(angleMain)));
    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, -qCos(angleMain)));

    angleObject += M_PI / 180.0f;
    angleGroup1 += M_PI / 360.0f;
    angleGroup1 -= M_PI / 360.0f;
    angleMain += M_PI / 720.0f;

    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        m_groups[0]->delObject(m_camera);
        m_groups[1]->addObject(m_camera);
        break;
    case Qt::Key_Right:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->addObject(m_camera);
        break;
    case Qt::Key_Down:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->delObject(m_camera);

        break;
    case Qt::Key_Up:
        m_groups[1]->delObject(m_camera);
        m_groups[0]->delObject(m_camera);
        QMatrix4x4 tmp;
        tmp.setToIdentity();
        m_camera->setGlobalTransform(tmp);
        break;
    }
    update();
}
