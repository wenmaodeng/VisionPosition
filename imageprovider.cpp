#include "imageprovider.h"

ImageProvider::ImageProvider(): QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return image;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(image);
}

void ImageProvider::setImage(const QImage &_image)
{
    image=_image;
}
