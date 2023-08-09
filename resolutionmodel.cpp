#include "resolutionmodel.h"

ResolutionModel::ResolutionModel(QString resolution)
{
	Resolution = resolution;
}

QString ResolutionModel::getResolution() const
{
	return Resolution;
}

void ResolutionModel::setResolution(const QString &value)
{
	Resolution = value;
}
