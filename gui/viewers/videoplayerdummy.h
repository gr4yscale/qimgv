#ifndef VIDEOPLAYERDUMMY_H
#define VIDEOPLAYERDUMMY_H

#include "gui/viewers/videoplayer.h"

class VideoPlayerDummy : public VideoPlayer
{
public:
    VideoPlayerDummy(QWidget *parent = nullptr);
    virtual bool openMedia(Clip *clip);
    virtual void seek(int pos);
    virtual void pauseResume();
    virtual void setPaused(bool mode);
    virtual void setMuted(bool);
};

#endif // VIDEOPLAYERDUMMY_H
