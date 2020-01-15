#include "materiallibrary.h"
#include "material.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDebug>

MaterialLibrary::MaterialLibrary()
{

}

void MaterialLibrary::addMaterial(Material *material)
{
    if(!material)
        return;

    for(int i=0; i < m_materials.size(); ++i)
    {
        if(m_materials.at(i) == material)
            return;
    }
    m_materials.append(material);
}

Material *MaterialLibrary::getMaterial(quint32 index)
{
    if(index < static_cast<quint32>(m_materials.size()))
        return m_materials.at(static_cast<int>(index));
    else {
        return nullptr;
    }

}

Material *MaterialLibrary::getMaterial(const QString &mtlName)
{
    for(int i=0; i < m_materials.size(); ++i)
    {
        if(m_materials.at(i)->mtlName() == mtlName)
            return m_materials.at(i);
    }
    return nullptr;
}

quint32 MaterialLibrary::getCountMaterials()
{
    return  static_cast<quint32>(m_materials.size());
}

void MaterialLibrary::loadMaterialsFromFile(const QString &filename)
{
    QFile m_mtlFile(filename);

    if(!m_mtlFile.exists())
    {
        qDebug() << "File not found";
        return;
    }
    if(!m_mtlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't open mtl file: " << filename;
        return;
    }

    for(auto mat:m_materials)
    {
        delete mat;
    }
    m_materials.clear();

    Material *newMtl = nullptr;
    QFileInfo fileInfo(filename);

    QTextStream m_inputStream(&m_mtlFile);

    while (!m_inputStream.atEnd()) {
        QString str = m_inputStream.readLine();
        QStringList list = str.split(" ");

        if (list.at(0) == "#"){
            qDebug() << "This is comment: " << str;
            continue;
        }
        else if (list.at(0) == "newmtl"){
            addMaterial(newMtl);
            newMtl = new Material;
            newMtl->setName(list.at(1));
            continue;
        }
        else if (list.at(0) == "Ns"){
            // размер пятна блика
            newMtl->setShinnes(list.at(1).toDouble());
            continue;
        }
        else if (list.at(0) ==  "Ka"){
            // амбиенс цвета
            newMtl->setAmbienceColor(QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(2).toFloat() ));
            continue;
        }
        else if (list.at(0) ==  "Kd"){
            newMtl->setDiffuseColor(QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(2).toFloat() ));
            continue;
        }
        else if (list.at(0) ==  "Ks"){
            newMtl->setSpecularColor(QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(2).toFloat() ));
            continue;
        }
        else if (list.at(0) ==  "map_Kd"){
            newMtl->setDiffuseMap(QString("%1%2%3").arg(fileInfo.absolutePath()).arg(QDir::separator()).arg(list.at(1)));
            continue;
        }else if (list.at(0) ==  "map_Bump"){
            newMtl->setNormalMap(QString("%1%2%3").arg(fileInfo.absolutePath()).arg(QDir::separator()).arg(list.at(1)));
            continue;
        }


    }

    addMaterial(newMtl);

    m_mtlFile.close();
}
