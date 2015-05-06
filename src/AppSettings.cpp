#include "AppSettings.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDebug>


const QString AppSettings::POMODORO_DURATION("pomodoroDuration");
const QString AppSettings::SHORT_BREAK_DURATION("shortBreakDuration");
const QString AppSettings::LONG_BREAK_DURATION("longBreakDuration");
const QString AppSettings::POMODOROS_BEFORE_LONG_BREAK("pomodorosBeforeLongBreak");
const QString AppSettings::WORK_TIMEOUT_SOUND("workTimeoutSound");
const QString AppSettings::SHORT_BREAK_TIMEOUT_SOUND("shortBreakTimeoutSound");
const QString AppSettings::LONG_BREAK_TIMEOUT_SOUND("longBreakTimeoutSound");


AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName("CodeZombies");
    QCoreApplication::setApplicationName("PomodoroTimer");

    mPomodoroDuration = QSettings().value(POMODORO_DURATION, 25).toInt();
    mShortBreakDuration = QSettings().value(SHORT_BREAK_DURATION, 5).toInt();
    mLongBreakDuration = QSettings().value(LONG_BREAK_DURATION, 15).toInt();
    mPomodorosBeforeLongBreak = QSettings().value(POMODOROS_BEFORE_LONG_BREAK, 4).toInt();
    mWorkTimeoutSound = QSettings().value(WORK_TIMEOUT_SOUND, "asset:///sounds/ship-bell.mp3").toString();
    mShortBreakTimeoutSound = QSettings().value(SHORT_BREAK_TIMEOUT_SOUND, "asset:///sounds/temple-bell.mp3").toString();
    mLongBreakTimeoutSound = QSettings().value(LONG_BREAK_TIMEOUT_SOUND, "asset:///sounds/zen-temple-bell.mp3").toString();
}

AppSettings::~AppSettings()
{

}

int AppSettings::pomodoroDuration() const
{
    return mPomodoroDuration;
}

int AppSettings::shortBreakDuration() const
{
    return mShortBreakDuration;
}

int AppSettings::longBreakDuration() const
{
    return mLongBreakDuration;
}

int AppSettings::pomodorosBeforeLongBreak() const
{
    return mPomodorosBeforeLongBreak;
}

QString AppSettings::workTimeoutSound() const
{
    return mWorkTimeoutSound;
}

QString AppSettings::shortBreakTimeoutSound() const
{
    return mShortBreakTimeoutSound;
}

QString AppSettings::longBreakTimeoutSound() const
{
    return mLongBreakTimeoutSound;
}


void AppSettings::setPomodoroDuration(int duration)
{
    if (duration != mPomodoroDuration) {
        QSettings().setValue(POMODORO_DURATION, duration);
        mPomodoroDuration = duration;
    }
}

void AppSettings::setShortBreakDuration(int duration)
{
    if (duration != mShortBreakDuration) {
        QSettings().setValue(SHORT_BREAK_DURATION, duration);
        mShortBreakDuration = duration;
    }
}

void AppSettings::setLongBreakDuration(int duration)
{
    if (duration != mLongBreakDuration) {
        QSettings().setValue(LONG_BREAK_DURATION, duration);
        mLongBreakDuration = duration;
    }
}

void AppSettings::setPomodorosBeforeLongBreak(int count)
{
    if(count != mPomodorosBeforeLongBreak) {
        QSettings().setValue(POMODOROS_BEFORE_LONG_BREAK, count);
        mPomodorosBeforeLongBreak = count;
    }
}

void AppSettings::setWorkTimeoutSound(QString url)
{
    if(url != mWorkTimeoutSound) {
        QSettings().setValue(WORK_TIMEOUT_SOUND, url);
        mWorkTimeoutSound = url;
    }
}

void AppSettings::setShortBreakTimeoutSound(QString url)
{
    if(url != mShortBreakTimeoutSound) {
        QSettings().setValue(SHORT_BREAK_TIMEOUT_SOUND, url);
        mShortBreakTimeoutSound = url;
    }
}

void AppSettings::setLongBreakTimeoutSound(QString url)
{
    if(url != mLongBreakTimeoutSound) {
        QSettings().setValue(LONG_BREAK_TIMEOUT_SOUND, url);
        mLongBreakTimeoutSound = url;
    }
}
