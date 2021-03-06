#ifndef HSQML_OBJECT_H
#define HSQML_OBJECT_H

#include <QtCore/QObject>
#include <QtCore/QAtomicInt>
#include <QtCore/QAtomicPointer>

class HsQMLClass;
class HsQMLObject;

class HsQMLObjectProxy
{
public:
    HsQMLObjectProxy(HsStablePtr, HsQMLClass*);
    virtual ~HsQMLObjectProxy();
    HsStablePtr haskell() const;
    HsQMLClass* klass() const;
    HsQMLObject* object();
    HsQMLObject* maybeObject();
    void clearObject();
    enum RefSrc {Handle, Object};
    void ref(RefSrc);
    void deref(RefSrc);

private:
    HsStablePtr mHaskell;
    HsQMLClass* mKlass;
    QAtomicPointer<HsQMLObject> mObject;
    QAtomicInt mRefCount;
};

class HsQMLObject : public QObject
{
public:
    HsQMLObject(HsQMLObjectProxy*);
    virtual ~HsQMLObject();
    virtual const QMetaObject* metaObject() const;
    virtual void* qt_metacast(const char*);
    virtual int qt_metacall(QMetaObject::Call, int, void**);
    HsQMLObjectProxy* proxy() const;

private:
    HsQMLObjectProxy* mProxy;
    HsStablePtr mHaskell;
    HsQMLClass* mKlass;
};

#endif /*HSQML_OBJECT_H*/
