#include "videocontrols.h"
#include "ui_videocontrols.h"

VideoControls::VideoControls(FloatingWidgetContainer *parent) :
    OverlayWidget(parent),
    ui(new Ui::VideoControls)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_NoMousePropagation, true);
    hide();
    ui->pauseButton->setIconPath(":res/icons/buttons/videocontrols/play24.png");
    ui->prevFrameButton->setIconPath(":res/icons/buttons/videocontrols/skip-backwards24.png");
    ui->nextFrameButton->setIconPath(":res/icons/buttons/videocontrols/skip-forward24.png");
    ui->muteButton->setIconPath(":/res/icons/buttons/videocontrols/mute-on24.png");
    ui->muteButton->setAction("toggleMute");

    lastPosition = -1;

    readSettings();
    connect(settings, &Settings::settingsChanged, this, &VideoControls::readSettings);

    connect(ui->pauseButton, &IconButton::clicked, this, &VideoControls::pause);
    connect(ui->seekBar, &VideoSlider::sliderMovedX, this, &VideoControls::seek);
    connect(ui->prevFrameButton, &IconButton::clicked, this, &VideoControls::prevFrame);
    connect(ui->nextFrameButton, &IconButton::clicked, this, &VideoControls::nextFrame);

    if(parent)
        setContainerSize(parent->size());
}

void VideoControls::readSettings() {
    if(settings->panelEnabled() && settings->panelPosition() == PanelHPosition::PANEL_BOTTOM)
        setPosition(FloatingWidgetPosition::TOP);
    else
        setPosition(FloatingWidgetPosition::BOTTOM);
}

VideoControls::~VideoControls() {
    delete ui;
}

void VideoControls::setMode(PlaybackMode _mode) {
    mode = _mode;
    ui->muteButton->setVisible( (mode == PLAYBACK_VIDEO) );
}

void VideoControls::setPlaybackDuration(int duration) {
    QString durationStr;
    if(mode == PLAYBACK_VIDEO) {
        int _time = duration;
        int hours = _time / 3600;
        _time -= hours * 3600;
        int minutes = _time / 60;
        int seconds = _time - minutes * 60;
        durationStr = QString("%1").arg(minutes, 2, 10, QChar('0')) + ":" +
                      QString("%1").arg(seconds, 2, 10, QChar('0'));
        if(hours)
            durationStr.prepend(QString("%1").arg(hours, 2, 10, QChar('0')) + ":");
    } else {
        durationStr = QString::number(duration);
    }
    ui->seekBar->setRange(0, duration - 1);
    ui->durationLabel->setText(durationStr);
    ui->positionLabel->setText(durationStr);
    recalculateGeometry();
    ui->positionLabel->setText("");
}

void VideoControls::setPlaybackPosition(int position) {
    if(position == lastPosition)
        return;
    QString positionStr;
    if(mode == PLAYBACK_VIDEO) {
        int _time = position;
        int hours = _time / 3600;
        _time -= hours * 3600;
        int minutes = _time / 60;
        int seconds = _time - minutes * 60;
        positionStr = QString("%1").arg(minutes, 2, 10, QChar('0')) + ":" +
                      QString("%1").arg(seconds, 2, 10, QChar('0'));
        if(hours)
            positionStr.prepend(QString("%1").arg(hours, 2, 10, QChar('0')) + ":");
    } else {
        positionStr = QString::number(position + 1);
    }
    ui->positionLabel->setText(positionStr);
    ui->seekBar->blockSignals(true);
    ui->seekBar->setValue(position);
    ui->seekBar->blockSignals(false);
    lastPosition = position;
}

void VideoControls::onPlaybackPaused(bool mode) {
    if(mode)
        ui->pauseButton->setIconPath(":res/icons/buttons/videocontrols/play24.png");
    else
        ui->pauseButton->setIconPath(":res/icons/buttons/videocontrols/pause24.png");
}

void VideoControls::onVideoMuted(bool mode) {
    if(mode)
        ui->muteButton->setIconPath(":res/icons/buttons/videocontrols/mute-on24.png");
    else
        ui->muteButton->setIconPath(":res/icons/buttons/videocontrols/mute-off24.png");
}
