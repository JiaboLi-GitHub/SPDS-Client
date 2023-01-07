#include "VideoSurface.h"
#include<qimage.h>

VideoSurface::VideoSurface(QObject* parent)
    : QAbstractVideoSurface(parent)
{

}


QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(
    QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> list;
    list << QVideoFrame::Format_RGB32;
    list << QVideoFrame::Format_ARGB32;
    list << QVideoFrame::Format_RGB24;
    list << QVideoFrame::Format_UYVY;
    list << QVideoFrame::Format_Y8;
    list << QVideoFrame::Format_YUYV;
    return list;
}

bool VideoSurface::present(const QVideoFrame& frame)
{
    // 处理捕获的帧
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);
        emit presentframe_(cloneFrame);

        return true;
    }
    return false;
}

