#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);
    QPixmap requestPixmap(const QString & id, QSize * size, const QSize & requestedSize);
    void setImage(const QImage &_image);

private:
    QImage image;
};

#endif // IMAGEPROVIDER_H
