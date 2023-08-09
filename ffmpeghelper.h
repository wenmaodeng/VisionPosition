#ifndef FFMPEGHELPER_H
#define FFMPEGHELPER_H

#include <QObject>
#include <QtDebug>
#include <QThread>
#include <QImage>
#include <QSettings>
#include <QtCore/QCoreApplication>
#include <QTextCodec>
#include <string>

#include "opencv2/opencv.hpp"
#include "imageprovider.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

using namespace cv;
using namespace std;
class FfmpegHelper : public QThread
{
    Q_OBJECT
public:
    FfmpegHelper();
    ~FfmpegHelper();

    Q_INVOKABLE void init(QString camera, QString resolution);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void saveOpencvConfig();

    Q_PROPERTY(double thresholdTOZERO READ getThresholdTOZERO WRITE setThresholdTOZERO NOTIFY thresholdTOZEROChanged)
    Q_PROPERTY(int firstCannythreshold READ getFirstCannythreshold WRITE setFirstCannythreshold NOTIFY firstCannythresholdChanged)
    Q_PROPERTY(int secondCannythreshold READ getSecondCannythreshold WRITE setSecondCannythreshold NOTIFY secondCannythresholdChanged)
    Q_PROPERTY(double proportion READ getProportion WRITE setProportion NOTIFY proportionChanged)
    Q_PROPERTY(double proportion READ getProportion WRITE setProportion NOTIFY proportionChanged)
    Q_PROPERTY(double minArea READ getMinArea WRITE setMinArea NOTIFY minAreaChanged)
    Q_PROPERTY(double maxArea READ getMaxArea WRITE setMaxArea NOTIFY maxAreaChanged)

    ImageProvider *imageProvider;
    int thresholdTOZERO=120;
    int firstCannythreshold = 60;
    int secondCannythreshold = 120;
    double proportion = 1;
    double minArea=0;
    double maxArea=20000;

    int getFirstCannythreshold() const;
    void setFirstCannythreshold(int value);

    int getSecondCannythreshold() const;
    void setSecondCannythreshold(int value);

    double getProportion() const;
    void setProportion(double value);

    int getThresholdTOZERO() const;
    void setThresholdTOZERO(int value);

    double getMinArea() const;
    void setMinArea(double value);

    double getMaxArea() const;
    void setMaxArea(double value);

signals:
    void emitImage();
    void runLog(QString log);
    void thresholdTOZEROChanged();
    void firstCannythresholdChanged();
    void secondCannythresholdChanged();
    void proportionChanged();
    void minAreaChanged();
    void maxAreaChanged();

protected:
    void run();

private:
    Mat AVFrameToCVMat(AVFrame *aVFrame, AVPixelFormat dstFormat);
    AVFrame *CVMatToAVFrame(cv::Mat &mat, AVPixelFormat dstFormat);
    //输入视频流地址
    QString inurl;
    //输入视频格式上下文
    AVFormatContext* pFormatCtx_Video = NULL;
    //输入视频编解码器上下文
    AVCodecContext* pCodecCtx_Video;
    //输入视频流索引
    int VideoInIndex = 0;
    //读取视频源获取的帧
    AVFrame *pAVFrame;
    //写入到输出视频流的帧
    AVFrame *pAVFrameRGB;
    //写入到输出视频流的帧的缓存
    uint8_t *pRgbBuffer;
    //图片转换
    SwsContext *pSwsCtx;
    //读取的帧包
    AVPacket packet;
    //图像格式转换中转
    AVFrame *frame = NULL;
    AVFrame *pAVFrameTemp = NULL;
    bool isplay = false;
    int ret = -1;
    QSettings *settings;
    void opencvConfig();
	string doubleToString(double num, int precision);
};

#endif // FFMPEGHELPER_H
