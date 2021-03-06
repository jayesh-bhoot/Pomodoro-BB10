#include "CircularTimer.h"

#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Color>
#include <bb/cascades/ImplicitAnimationController>
#include <bb/cascades/FontSize>

#include "math.h"

#include <QSettings>
#include <QDebug>

const int CircularTimer::INITIAL_ANGLE = 270;
const int CircularTimer::SECOND_HAND_MOVEMENT_ANGLE = 6;
const int CircularTimer::DEFAULT_DURATION = 25;
const float CircularTimer::DEFAULT_DIMENSION = 400;

CircularTimer::CircularTimer(Container *parent) :
        CustomControl(parent),
        m_dimension(DEFAULT_DIMENSION),
        mDuration(DEFAULT_DURATION),
        m_startTime(0, 0, 0),
        m_endTime(0, DEFAULT_DURATION, 0),
        m_updateTimer(new QTimer(this))
{
    m_secondHand = Image(QUrl("asset:///images/handle_inactive.png"));
    m_secondHandle = ImageView::create().image(m_secondHand)
                                        .horizontal(HorizontalAlignment::Right)
                                        .vertical(VerticalAlignment::Center);
    m_secondHandleContainer = Container::create().layout(new DockLayout()).add(m_secondHandle);
    m_secondDial = ImageView::create().image(QUrl("asset:///images/slider_track.png"))
                                      .horizontal(HorizontalAlignment::Center)
                                      .vertical(VerticalAlignment::Center);

    m_minuteHand = Image(QUrl("asset:///images/handle_inactive.png"));
    m_minuteHandle = ImageView::create().image(m_minuteHand)
                                        .horizontal(HorizontalAlignment::Right)
                                        .vertical(VerticalAlignment::Center);
    m_minuteHandleContainer = Container::create().layout(new DockLayout()).add(m_minuteHandle);
    m_minuteDial = ImageView::create().image(QUrl("asset:///images/slider_track.png"))
                                      .horizontal(HorizontalAlignment::Center)
                                      .vertical(VerticalAlignment::Center);
    m_minuteHandContainer = Container::create().layout(new DockLayout())
                                               .horizontal(HorizontalAlignment::Center)
                                               .vertical(VerticalAlignment::Center)
                                               .add(m_minuteDial)
                                               .add(m_minuteHandleContainer);

    m_digitalTime = Label::create().horizontal(HorizontalAlignment::Center).vertical(
            VerticalAlignment::Center);

    m_rootContainer = Container::create().layout(new DockLayout())
                                         .add(m_secondDial).add(m_secondHandleContainer)
                                         .add(m_minuteHandContainer)
                                         .add(m_digitalTime)
                                         .horizontal(HorizontalAlignment::Fill);

    setRoot(m_rootContainer);

    bool ok;
    Q_UNUSED(ok);
    ok = QObject::connect(this, SIGNAL(dimensionChanged(float)), this, SLOT(onDimensionChanged(float)));
    Q_ASSERT(ok);
    ok = QObject::connect(this, SIGNAL(durationChanged(int)), this, SLOT(onDurationChanged(int)));
    Q_ASSERT(ok);

    setDimension(m_dimension);
    setDuration(mDuration);

    ok = QObject::connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateHands()));
    Q_ASSERT(ok);
}

void CircularTimer::onDimensionChanged(float dimension)
{
    m_rootContainer->setPreferredSize(m_dimension, m_dimension);
    m_secondDial->setPreferredSize(m_dimension * 0.85, m_dimension * 0.85);
    m_secondHandle->setPreferredSize(m_dimension * 0.2, m_dimension * 0.2);
    m_secondHandleContainer->setPreferredSize(m_dimension, m_dimension * 0.2);
    m_secondHandleContainer->setTranslationY((m_dimension - 0.2 * m_dimension) / 2);
    m_secondHandleContainer->setRotationZ(INITIAL_ANGLE);

    float secondDialDimension = m_dimension * 0.70;
    m_minuteHandContainer->setPreferredSize(secondDialDimension, secondDialDimension);
    m_minuteDial->setPreferredSize(secondDialDimension * 0.85, secondDialDimension * 0.85);
    m_minuteHandle->setPreferredSize(secondDialDimension * 0.2, secondDialDimension * 0.2);
    m_minuteHandleContainer->setPreferredSize(secondDialDimension, secondDialDimension * 0.2);
    m_minuteHandleContainer->setTranslationY((secondDialDimension - 0.2 * secondDialDimension) / 2);
    m_minuteHandleContainer->setRotationZ(INITIAL_ANGLE);
}

void CircularTimer::updateHands()
{
    int elapsedSecs = m_startTime.secsTo(QTime::currentTime());
    QTime elapsedTime = QTime(0, 0, 0).addSecs(elapsedSecs);
    int secondsLeft = elapsedTime.secsTo(m_endTime);
    if (secondsLeft >= 0) {
        QTime timeLeft = QTime(0, 0, 0).addSecs(secondsLeft);
        m_digitalTime->setText(timeLeft.toString("mm.ss"));

        m_secondHandleContainer->setRotationZ(
                m_secondHandleContainer->rotationZ() + SECOND_HAND_MOVEMENT_ANGLE);

        double minuteMovement = elapsedTime.minute() * mMinuteHandMovementAngle + 270;
        if (minuteMovement > (360 + 270))
            m_minuteHandleContainer->setRotationZ(360 + 270);
        else
            m_minuteHandleContainer->setRotationZ(minuteMovement);
    } else {
        timeout();
    }
}

int CircularTimer::duration() const
{
    return mDuration;
}

void CircularTimer::setDuration(int duration)
{
    mDuration = duration;
    emit durationChanged(duration);
}

void CircularTimer::start()
{
    resetHandsWithoutAnimation();
    m_startTime.start();
    m_updateTimer->start(1000);
    emit timerStarted();
}

void CircularTimer::stop()
{
    m_updateTimer->stop();
    m_digitalTime->setText(m_endTime.toString("mm.ss"));
    resetHandsWithAnimation();
    emit timerStopped();
}

void CircularTimer::timeout()
{
    m_updateTimer->stop();
    m_digitalTime->setText(m_endTime.toString("mm.ss"));
    emit timerCompleted();
}

void CircularTimer::onDurationChanged(int duration)
{
    m_startTime.setHMS(0, 0, 0);
    m_endTime = m_startTime.addSecs(duration * 60);
    qDebug() << "endTime: " << m_endTime;
    if (duration < 60)
        m_digitalTime->setText(m_endTime.toString("mm.ss"));
    else
        m_digitalTime->setText("60.00");
    mMinuteHandMovementAngle = 360 / (double) duration;
}

QString CircularTimer::timeLeft() const
{
    if (isActive()) {
        int elapsedSecs = m_startTime.secsTo(QTime::currentTime());
        QTime elapsedTime = QTime(0, 0, 0).addSecs(elapsedSecs);
        int secondsLeft = elapsedTime.secsTo(m_endTime);
        QTime timeLeft = QTime(0, 0, 0).addSecs(secondsLeft);
        return timeLeft.toString("mm.ss");
    }
    if (m_endTime.hour() <= 0)
        return m_endTime.toString("mm.ss");
    else
        return "60.00";
}

bool CircularTimer::isActive() const
{
    return m_updateTimer->isActive();
}

void CircularTimer::resetHandsWithoutAnimation()
{
    if (m_secondHandleContainer->rotationZ() != INITIAL_ANGLE) {
        ImplicitAnimationController secondController = ImplicitAnimationController::create(
                m_secondHandleContainer).enabled(false);
        m_secondHandleContainer->setRotationZ(INITIAL_ANGLE);
    }
    if (m_minuteHandleContainer->rotationZ() != INITIAL_ANGLE) {
        ImplicitAnimationController minuteController = ImplicitAnimationController::create(
                m_minuteHandleContainer).enabled(false);
        m_minuteHandleContainer->setRotationZ(INITIAL_ANGLE);
    }
}

void CircularTimer::resetHandsWithAnimation()
{
    if (m_secondHandleContainer->rotationZ() != INITIAL_ANGLE) {
        m_secondHandleContainer->setRotationZ(INITIAL_ANGLE);
    }
    if (m_minuteHandleContainer->rotationZ() != INITIAL_ANGLE) {
        m_minuteHandleContainer->setRotationZ(INITIAL_ANGLE);
    }
}


float CircularTimer::dimension() const
{
    return m_dimension;
}

void CircularTimer::setDimension(float dimension)
{
    m_dimension = dimension;
    emit dimensionChanged(m_dimension);
}

int CircularTimer::digitalTimerSize() const
{
    return m_digitalTime->textStyle()->fontSize();
}

void CircularTimer::setDigitalTimerSize(int size)
{
    m_digitalTime->textStyle()->setFontSize(static_cast<FontSize::Type>(size));
}
