#ifndef RESOLUTIONMODELLIST_H
#define RESOLUTIONMODELLIST_H

#include <QObject>
#include <QAbstractListModel>
#include "resolutionmodel.h"

class ResolutionModelList : public QAbstractListModel
{
    Q_OBJECT
private:
	enum DataRoles {
		Resolution = 0
	};
public:

    ResolutionModelList();

	virtual QVariant data(const QModelIndex &index, int role = DataRoles::Resolution) const;
	virtual QHash<int, QByteArray> roleNames() const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex())  const;

	QList<ResolutionModel*> *Resolutions;
    void setResolutions(QList<QString> &resolutions);

signals:
    void resolutionsChanged();
};

#endif // RESOLUTIONMODELLIST_H
