#include "ffmpeghelper.h"


FfmpegHelper::FfmpegHelper()
{
	opencvConfig();
	imageProvider = new ImageProvider();
	avdevice_register_all();
}

FfmpegHelper::~FfmpegHelper()
{
	isplay = false;
	qDebug() << "free FfmpegHelper";
}
string FfmpegHelper::doubleToString(double num, int precision)
{
	int nn = precision + 1;
	std::ostringstream out;
	out << std::setprecision(nn) << num;
	return out.str();
}

void FfmpegHelper::run()
{
	int ret = -1;
	while (isplay) {
		ret = av_read_frame(pFormatCtx_Video, &packet);
		if (ret != 0)
		{
			continue;
		}
		if (packet.stream_index != VideoInIndex)
		{
			av_packet_unref(&packet);
			continue;
		}
		else
		{
			ret = AVERROR(EAGAIN);
			ret = avcodec_send_packet(pCodecCtx_Video, &packet);
			if (ret != 0) {
				qDebug() << "avcodec_send_packet error";
				av_packet_unref(&packet);
				continue;
			}
			ret = avcodec_receive_frame(pCodecCtx_Video, pAVFrame);
			if (ret == 0) {

				Mat temp;
				temp = AVFrameToCVMat(pAVFrame,(AVPixelFormat)AV_PIX_FMT_BGR24);
				//Mat imgtemp = imread("sfz.jpg");
				//Mat temp;
				//resize(imgtemp, temp, Size(1280, 720), 1, 1);
				//灰度
				Mat _gray;
				cvtColor(temp, _gray, COLOR_BGR2GRAY);
				//归一
				Mat _threshold;
				threshold(_gray, _threshold, thresholdTOZERO, 255, THRESH_TOZERO);
				//高斯滤波
				Mat _gaussianBlur;
				GaussianBlur(_gray, _gaussianBlur, Size(3, 3), 5, 0);
				//边缘检测
				Mat _canny;
				Canny(_gaussianBlur, _canny, firstCannythreshold, secondCannythreshold);
				//算子
				Mat kernel = getStructuringElement(1, Size(3, 3));
				//膨胀
				dilate(_canny, _canny, kernel);
				//轮廓提取
				vector<vector<Point>> contours;
				vector<Vec4i> hierarchy;
				findContours(_canny, contours, hierarchy, 0, 2, Point());
				if (contours.size() == 0)
				{
					runLog(QString("未找到边缘点"));
					continue;
				}

				for (int i = 0; i < contours.size(); i++)
				{
					double arclength = arcLength(contours[i], true)*proportion;
					double area = contourArea(contours[i])*proportion*proportion;
					if (area < minArea || area>maxArea)
					{
						runLog(QString("过滤非预期面积：") + QString::fromStdString(std::to_string(area)));
						continue;
					}

					//绘制最小框选矩形
					cv::Rect rect = boundingRect(contours[i]);
					rectangle(temp, rect, Scalar(255, 0, 0));
					//上
					Point upPoint1 = Point(rect.x + rect.width / 2, rect.y);
					Point upPoint2 = Point(rect.x + rect.width / 2, 0);
                    string uplength = doubleToString(rect.y*proportion,3);
					putText(temp, "up:" + uplength + "mm", Point(rect.x + rect.width / 2 + 3, rect.y / 2), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2);
					line(temp, upPoint1, upPoint2, Scalar(0, 0, 0));
					//下
					Point downPoint1 = Point(rect.x + rect.width / 2, rect.y + rect.height);
					Point downPoint2 = Point(rect.x + rect.width / 2, pCodecCtx_Video->height);
					string downlength = doubleToString((pCodecCtx_Video->height - rect.y - rect.height)*proportion,3);
					putText(temp, "down:" + downlength + "mm", Point(rect.x + rect.width / 2 + 3, (pCodecCtx_Video->height + rect.y + rect.height) / 2), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2);
					line(temp, downPoint1, downPoint2, Scalar(0, 0, 0));
					//左
					Point leftPoint1 = Point(rect.x, rect.y + rect.height / 2);
					Point leftPoint2 = Point(0, rect.y + rect.height / 2);
					string leftlength = doubleToString(rect.x*proportion,3);
					putText(temp, "left:" + leftlength + "mm", Point(3, rect.y + rect.height / 2), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2);
					line(temp, leftPoint1, leftPoint2, Scalar(0, 0, 0));
					//右
					Point rightPoint1 = Point(rect.x + rect.width, rect.y + rect.height / 2);
					Point rightPoint2 = Point(pCodecCtx_Video->width, rect.y + rect.height / 2);
					string rightlength = doubleToString((pCodecCtx_Video->width - rect.x - rect.width)*proportion,3);
					putText(temp, "right:" + rightlength+ "mm", Point(rect.x + rect.width + 3, rect.y + rect.height / 2), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2);
					line(temp, rightPoint1, rightPoint2, Scalar(0, 0, 0));

					//绘制最小外接矩形
					cv::RotatedRect rotatedrect = minAreaRect(contours[i]);

					Point2f _points[4];
					rotatedrect.points(_points);
					putText(temp, doubleToString(area,3)+"(mm*mm)", rotatedrect.center, FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0), 2);
					

					for (int j = 0; j < 4; j++)
					{
						if (j == 3)
						{
							line(temp, _points[j], _points[0], Scalar(0, 0, 255));
							break;
						}
						line(temp, _points[j], _points[j + 1], Scalar(0, 0, 255));
					}
				}


				if (pAVFrameTemp == NULL)
					pAVFrameTemp = av_frame_alloc();
				pAVFrameTemp = CVMatToAVFrame(temp, (AVPixelFormat)pAVFrame->format);

				//转换像素
				sws_scale(pSwsCtx, (uint8_t const * const *)pAVFrameTemp->data, pAVFrameTemp->linesize, 0, pCodecCtx_Video->height, pAVFrameRGB->data, pAVFrameRGB->linesize);
				//构造QImage
				QImage img(pRgbBuffer, pCodecCtx_Video->width, pCodecCtx_Video->height, QImage::Format_RGB32);
				//更新UI图像
				imageProvider->setImage(img);
				emitImage();
			}
			else {
				qDebug() << "decode error";
			}
		}
		av_packet_unref(&packet);
	}
	av_packet_unref(&packet);
	av_frame_free(&pAVFrame);
	avformat_close_input(&pFormatCtx_Video);
	avcodec_free_context(&pCodecCtx_Video);
	av_freep(&pRgbBuffer);
}

Mat FfmpegHelper::AVFrameToCVMat(AVFrame *avFrame, AVPixelFormat dstFormat)
{
	//得到AVFrame信息
	int srcW = avFrame->width;
	int srcH = avFrame->height;

	//生成Mat对象
	cv::Mat mat;
	mat.create(cv::Size(srcW, srcH), CV_8UC3);

	int cvLinesizes[1];
	cvLinesizes[0] = mat.step1();

	SwsContext *swsCtx = sws_getContext(srcW, srcH, (AVPixelFormat)avFrame->format,
		srcW, srcH, dstFormat,
		SWS_BICUBIC, NULL, NULL, NULL);
	sws_scale(swsCtx, avFrame->data, avFrame->linesize, 0, srcH, &mat.data, cvLinesizes);
	sws_freeContext(swsCtx);

	return mat;

}

AVFrame *FfmpegHelper::CVMatToAVFrame(Mat &mat, AVPixelFormat dstFormat)
{
	int width = mat.cols;
	int height = mat.rows;

	int cvLinesizes[1];
	cvLinesizes[0] = mat.step1();

	SwsContext* conversion = sws_getContext(width, height, AVPixelFormat::AV_PIX_FMT_BGR24,
		width, height, (AVPixelFormat)frame->format,
		SWS_FAST_BILINEAR, NULL, NULL, NULL);
	sws_scale(conversion, &mat.data, cvLinesizes, 0, height, frame->data, frame->linesize);
	sws_freeContext(conversion);
	return frame;
}

void FfmpegHelper::opencvConfig()
{
	QString filepath = QCoreApplication::applicationDirPath() + "/setting.ini";
	settings = new QSettings(filepath, QSettings::IniFormat);
	settings->setIniCodec(QTextCodec::codecForName("utf-8"));
	settings->beginGroup("Opencv");

	QVariant temp = settings->value("firstCannythreshold");
	if (temp.isValid())
	{
		firstCannythreshold = temp.toInt();
	}
	else
	{
		settings->setValue("firstCannythreshold", firstCannythreshold);
	}


	temp = settings->value("firstCannythreshold");
	if (temp.isValid())
	{
		firstCannythreshold = temp.toInt();
	}
	else
	{
		settings->setValue("firstCannythreshold", firstCannythreshold);
	}

	temp = settings->value("secondCannythreshold");
	if (temp.isValid())
	{
		secondCannythreshold = temp.toInt();
	}
	else
	{
		settings->setValue("secondCannythreshold", secondCannythreshold);
	}

	temp = settings->value("proportion");
	if (temp.isValid())
	{
		proportion = temp.toDouble();
	}
	else
	{
		settings->setValue("proportion", proportion);
	}
	temp = settings->value("minArea");
	if (temp.isValid())
	{
		minArea = temp.toDouble();
	}
	else
	{
		settings->setValue("minArea", minArea);
	}
	temp = settings->value("maxArea");
	if (temp.isValid())
	{
		maxArea = temp.toDouble();
	}
	else
	{
		settings->setValue("maxArea", maxArea);
	}
	settings->endGroup();
}

void FfmpegHelper::saveOpencvConfig()
{
	settings->beginGroup("Opencv");
	settings->setValue("thresholdTOZERO", thresholdTOZERO);
	settings->setValue("firstCannythreshold", firstCannythreshold);
	settings->setValue("secondCannythreshold", secondCannythreshold);
	settings->setValue("proportion", proportion);
	settings->setValue("minArea", minArea);
	settings->setValue("maxArea", maxArea);
	settings->endGroup();
}

double FfmpegHelper::getMaxArea() const
{
	return maxArea;
}

void FfmpegHelper::setMaxArea(double value)
{
	maxArea = value;
}

double FfmpegHelper::getMinArea() const
{
	return minArea;
}

void FfmpegHelper::setMinArea(double value)
{
	minArea = value;
}




void FfmpegHelper::init(QString camera, QString resolution)
{
	pFormatCtx_Video = avformat_alloc_context();

	QString urlString = QString("video=") + camera;

	const AVInputFormat *inputFormat = av_find_input_format("dshow");

	AVDictionary* options = NULL;
	//减少缓冲
	av_dict_set(&options, "avioflags", "direct", 0);
	//分辨率
	av_dict_set(&options, "video_size", resolution.toStdString().c_str(), 0);
	//帧率
	av_dict_set(&options, "framerate", "25", 0);
	//格式
	av_dict_set(&options, "vcodec", "mjpeg", 0);
	//打开摄像头
	ret = avformat_open_input(&pFormatCtx_Video, urlString.toStdString().c_str(), inputFormat, &options);
	if (ret < 0)
	{
		qDebug() << "avformat_open_input failed" << ret;
		return;
	}
	else
	{
		qDebug() << "avformat_open_input success";
	}
	//分析视频流信息
	ret = avformat_find_stream_info(pFormatCtx_Video, NULL);
	if (ret < 0)
	{
		qDebug() << "avformat_find_stream_info failed";
		return;
	}
	else
	{
		qDebug() << "avformat_find_stream_info success";
	}

	//查找视频信息
	VideoInIndex = av_find_best_stream(pFormatCtx_Video, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);

	//查找解码器
	pCodecCtx_Video = avcodec_alloc_context3(NULL);//输入音频编解码器上下文
	avcodec_parameters_to_context(pCodecCtx_Video, pFormatCtx_Video->streams[VideoInIndex]->codecpar);
	const AVCodec* tmpCodec = avcodec_find_decoder(pFormatCtx_Video->streams[VideoInIndex]->codecpar->codec_id);
	if (tmpCodec == NULL)
	{
		qDebug() << "avcodec_find_decoder failed";
		return;
	}
	else
	{
		qDebug() << "avcodec_find_decoder success";
	}
	//打开解码器
	ret = avcodec_open2(pCodecCtx_Video, tmpCodec, NULL);
	if (ret < 0)
	{
		qDebug() << "avcodec_open2 failed";
		return;
	}
	else
	{
		qDebug() << "avcodec_open2 success";
	}

	//解码获取的未压缩的图像。
	pAVFrame = av_frame_alloc();
	//图像格式转换后，进行编码前的图像。
	pAVFrameRGB = av_frame_alloc();
	//图像格式转换上下文
	pSwsCtx = sws_getContext(pCodecCtx_Video->width, pCodecCtx_Video->height, pCodecCtx_Video->pix_fmt,
		pCodecCtx_Video->width, pCodecCtx_Video->height, AV_PIX_FMT_RGB32,
		SWS_BICUBIC, NULL, NULL, NULL);
	//初始化缓冲空间
	int  m_size = av_image_get_buffer_size(AVPixelFormat(AV_PIX_FMT_RGB32), pCodecCtx_Video->width, pCodecCtx_Video->height, 1);
	pRgbBuffer = (uint8_t *)(av_malloc(m_size));
	pAVFrameRGB->width = pCodecCtx_Video->width;
	pAVFrameRGB->height = pCodecCtx_Video->height;
	pAVFrameRGB->format = AV_PIX_FMT_BGR32;
	//pRgbBuffer和pAVFrameRGB图片数据空间绑定
	ret = av_image_fill_arrays(pAVFrameRGB->data, pAVFrameRGB->linesize, pRgbBuffer, AV_PIX_FMT_BGR32, pCodecCtx_Video->width, pCodecCtx_Video->height, 1);
	if (ret < 0)
	{
		qDebug() << "av_image_fill_arrays failed";
		return;
	}
	else
	{
		qDebug() << "av_image_fill_arrays success";
	}
	//为packet分配内存空间
	av_new_packet(&packet, pCodecCtx_Video->width * pCodecCtx_Video->height);

	frame = av_frame_alloc();
	frame->height = pCodecCtx_Video->height;
	frame->width = pCodecCtx_Video->width;
	frame->format = pCodecCtx_Video->pix_fmt;
	av_image_alloc(frame->data, frame->linesize, frame->width, frame->height, pCodecCtx_Video->pix_fmt, 1);

	isplay = true;
}

void FfmpegHelper::stop()
{
	isplay = false;
}

double FfmpegHelper::getProportion() const
{
	return proportion;
}

void FfmpegHelper::setProportion(double value)
{
	proportion = value;
}

int FfmpegHelper::getSecondCannythreshold() const
{
	return secondCannythreshold;
}

void FfmpegHelper::setSecondCannythreshold(int value)
{
	secondCannythreshold = value;
}

int FfmpegHelper::getFirstCannythreshold() const
{
	return firstCannythreshold;
}

void FfmpegHelper::setFirstCannythreshold(int value)
{
	firstCannythreshold = value;
}

int FfmpegHelper::getThresholdTOZERO() const
{
	return thresholdTOZERO;
}

void FfmpegHelper::setThresholdTOZERO(int value)
{
	thresholdTOZERO = value;
}
