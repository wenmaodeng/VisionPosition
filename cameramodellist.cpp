#include "cameramodellist.h"

CameraModelList::CameraModelList()
{
	QCameraInfos = QCameraInfo::availableCameras();
	resolutionModelList = new ResolutionModelList();
	if (QCameraInfos.size() > 0)
		updataResolutions(0);
}

CameraModelList::~CameraModelList()
{
	qDebug() << "free CameraModelList";
}

QVariant CameraModelList::data(const QModelIndex &index, int role) const
{
	switch (role) {
	case  DataRoles::DeviceName:
		return QCameraInfos[index.row()].deviceName();
	case  DataRoles::Description:
		return QCameraInfos[index.row()].description();
	default: break;
	}
	return QVariant();
}

QHash<int, QByteArray> CameraModelList::roleNames() const
{
	QHash <int, QByteArray> temp;
	temp.insert(DataRoles::DeviceName, "deviceName");
	temp.insert(DataRoles::Description, "description");
	return temp;
}

int CameraModelList::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return QCameraInfos.length();
}

void CameraModelList::updataResolutions(int cameraIndex)
{
	if (cameraIndex > QCameraInfos.size() - 1)
		return;
	QCameraInfo cameraInfo = QCameraInfos.at(cameraIndex);
	QCamera *camera = new QCamera(cameraInfo.deviceName().toUtf8(), this);
	camera->start();
	QList<QSize> sizes = camera->supportedViewfinderResolutions();
	camera->stop();

	QList<QString> _resolutions;
	for (int i = 0; i < sizes.count(); i++)
	{
		QString temp = QString::number(sizes[i].width()) + "x" + QString::number(sizes[i].height());
		if (!_resolutions.contains(temp))
			_resolutions.append(temp);
	}

	resolutionModelList->setResolutions(_resolutions);
}
