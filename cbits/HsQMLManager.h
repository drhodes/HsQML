#ifndef HSQML_MANAGER_H
#define HSQML_MANAGER_H

#include <QtCore/QMutex>
#include <QtCore/QVector>
#include <QtCore/QUrl>
#include <QtGui/QApplication>

#include "hsqml.h"
#include "HsQMLEngine.h"

class HsQMLManager : public QObject
{
    Q_OBJECT

public:
    HsQMLManager(
        int& argc,
        char** argv,
        void (*)(HsFunPtr),
        void (*)(HsStablePtr));
    ~HsQMLManager();
    void run();
    void freeFun(HsFunPtr);
    void freeStable(HsStablePtr);
    Q_SLOT void createEngine(HsQMLEngineConfig);

private:
    HsQMLManager(const HsQMLManager&);
    HsQMLManager& operator=(const HsQMLManager&);

    QApplication mApp;
    QVector<HsQMLEngine*> mEngines;
    void (*mFreeFun)(HsFunPtr);
    void (*mFreeStable)(HsStablePtr);
};

extern QMutex gMutex;
extern HsQMLManager* gManager;
extern int gLogLevel;

#endif /*HSQML_MANAGER_H*/
