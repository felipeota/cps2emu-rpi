#ifndef HALOBJECT_H
#define HALOBJECT_H

#include <QObject>
#include <QString>
#include <QDebug>


/* Structure required by the PropertyModified signal */
struct Property {
    QString name;
    bool added;
    bool removed;
};



class HalObject : public QObject
{
    Q_OBJECT
public:
    HalObject(QObject * parent = 0);
    virtual ~HalObject() {}

public slots:
    void listen(int num, QList<Property> prop);

};

#endif // HALOBJECT_H
