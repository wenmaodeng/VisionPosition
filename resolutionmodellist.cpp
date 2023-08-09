#include "resolutionmodellist.h"

ResolutionModelList::ResolutionModelList()
{
	Resolutions = new QList<ResolutionModel*>({});
}

QVariant ResolutionModelList::data(const QModelIndex &index, int role) const
{
	switch (role) {
	case  DataRoles::Resolution:
		return Resolutions->at(index.row())->getResolution();
	default: break;
	}
	return QVariant();
}

QHash<int, QByteArray> ResolutionModelList::roleNames() const
{
	QHash <int, QByteArray> temp;
	temp.insert(DataRoles::Resolution, "resolution");
	return temp;
}

int ResolutionModelList::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return  Resolutions->size();
}

void ResolutionModelList::setResolutions(QList<QString> &resolutions)
{
	beginRemoveRows(QModelIndex(), 0, Resolutions->size() - 1);
	Resolutions->clear();
	endRemoveRows();
	
	for (int i = 0; i < resolutions.size(); i++)
	{
		ResolutionModel *resolutionModel;
		resolutionModel = new ResolutionModel(resolutions.at(i));
		beginInsertRows(QModelIndex(), i, i);
		Resolutions->insert(i, resolutionModel);
		endInsertRows();
	}
}
