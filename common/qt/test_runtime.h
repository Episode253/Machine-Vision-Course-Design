#ifndef TEST_RUNTIME_H
#define TEST_RUNTIME_H

#include <QCoreApplication>
#include <QTimer>


inline void scheduleAutomatedExit()
{
    bool valid = false;
    const int timeoutMs = qEnvironmentVariableIntValue("VISUAL_LAB_AUTO_EXIT_MS", &valid);
    if (valid && timeoutMs >= 0)
        QTimer::singleShot(timeoutMs, QCoreApplication::instance(), &QCoreApplication::quit);
}

#endif
