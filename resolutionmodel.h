#ifndef RESOLUTIONMODEL_H
#define RESOLUTIONMODEL_H

#include <QObject>

class ResolutionModel : public QObject
{
    Q_OBJECT
public:
    ResolutionModel(QString resolution);

    QString getResolution() const;
    void setResolution(const QString &value);

signals:
private:
    QString Resolution;

};

#endif // RESOLUTIONMODEL_H
