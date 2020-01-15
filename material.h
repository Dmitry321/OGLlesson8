#ifndef MATERIAL_H
#define MATERIAL_H
#include <QString>
#include <QVector3D>
#include <QImage>

class Material
{
public:
    Material();
    void setName(const QString &mtlName);
    const QString &mtlName() const;
    void setDiffuseColor(const QVector3D &diffuseColor);
    const QVector3D &diffuseColor() const;
    void setAmbienceColor(const QVector3D &ambienceColor);
    const QVector3D &ambienceColor() const;
    void setSpecularColor(const QVector3D &specularColor);
    const QVector3D &specularColor() const;
    void setShinnes(const float &shinnes);
    const float &shinnes() const;
    void setDiffuseMap(const QString &filename);
    void setDiffuseMap(const QImage &imageFile);
    const QImage &diffuseMap() const;
    bool isUsingDiffuseMap() const;

    void setNormalMap(const QString &filename);
    void setNormalMap(const QImage &imageFile);
    const QImage &normalMap() const;
    bool isUsingNormalMap() const;


private:
    QString m_mtlName;
    QVector3D m_diffuseColor;
    QVector3D m_ambienceColor;
    QVector3D m_specularColor;
    float m_shinnes; // размер блика чем больше число тем блик меньше
    QImage m_diffuseMap;
    QImage m_normalMap;
    bool m_isUsingDiffuseMap;
    bool m_isUsingNormalMap;


};

#endif // MATERIAL_H
