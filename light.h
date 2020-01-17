#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>


class Light
{
public:
    enum Type {
        Directional = 0,
        Point = 1,
        Spot = 2
    };

    Light(const Type &type = Directional);
    void setAmbienceColor(const QVector3D &ambienceColor);
    const QVector3D &getAmbienceColor() const;

    void setDiffuseColor(const QVector3D &diffuseColor);
    const QVector3D &getDiffuseColor() const;

    void setSpecularColor(const QVector3D &specularColor);
    const QVector3D &getSpecularColor() const;

    void setPosition(const QVector4D &position);
    const QVector4D &getPosition() const;

    void setDirection(const QVector4D &direction);
    const QVector4D &getDirection() const;

    void setCutoff(const float &cutoff);
    const float &getCutoff() const;

    void setType(const Type &type);
    const Type &getType() const;

    const QMatrix4x4 &getLightMatrix() const;

private:
    // Параметры света а налогичны параметрам для материала
   QVector3D m_ambienceColor; // свет который много много раз отразился от разных поверхностей и непонятно откуда пришел этот свет - просто светло как в сумерки например на объектах гдк нет явного попадания лучей света
   QVector3D m_diffuseColor;  // это непосредственно цвет падающих лучей источника света
   QVector3D m_specularColor; // это цет блика от падающих прямых лучей

   QVector4D m_position;
   QVector4D m_direction;
   float m_cutoff; // параметр половина угла в вершине конуса если свет в виде конуса (прожекторный свет)
   Type m_type;
   QMatrix4x4 m_lightMatrix;

};

#endif // LIGHT_H
