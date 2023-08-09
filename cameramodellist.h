#ifndef CAMERAMODELLIST_H
#define CAMERAMODELLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QCameraInfo>
#include <QCamera>
#include <QtDebug>
#include "resolutionmodellist.h"

class CameraModelList : public QAbstractListModel
{
    Q_OBJECT
private:
	enum DataRoles {
		DeviceName = 0,
		Description
	};
public:
    CameraModelList();
    ~CameraModelList();
    virtual QVariant data(const QModelIndex &index, int role = DataRoles::DeviceName) const ;
    virtual QHash<int, QByteArray> roleNames() const ;
    virtual int rowCount(const QModelIndex &parent = QModelIndex())  const ;

    Q_INVOKABLE void updataResolutions(int cameraIndex);
    
	QList<QCameraInfo> QCameraInfos;
    ResolutionModelList* resolutionModelList;

signals:
};

#endif // CAMERAMODELLIST_H
