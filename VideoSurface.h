#pragma once
#include<qabstractvideosurface.h>
#include<qvideoframe.h>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;
    explicit VideoSurface(QObject* parent = 0);

private slots:
    bool present(const QVideoFrame& frame) override;

signals:
    void presentframe_(QVideoFrame& frame);

};
