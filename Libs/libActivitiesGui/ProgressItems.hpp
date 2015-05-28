
#pragma once

//#include <libActivities/Activity.hpp>
//#include <libActivities/Step.hpp>
//#include <libActivities/Service.hpp>

#include <QVariant>

class ProgressItem
{
public:
    enum class Type { Base = 0, Service, Activity, Step };
    enum class DisplayRole { Progress = Qt::UserRole, ProgressInfo };

public:
    static const Type StaticType = Type::Base;

public:
    ProgressItem(ProgressItem* parent);

public:
    virtual int type() const;

    virtual QVariant data(int role) const;

private:
    ProgressItem*       mParent;
};

template<class T>
class ProgressItemObject : public ProgressItem
{
protected:
    ProgressItemObject(ProgressItem* parent, T* object)
        : ProgressItem(parent)
        , mObject(object)
    {}

public:
    const T* object() const {
        return mObject;
    }

    T* object() {
        return mObject;
    }

    QVariant data(int role) const {
//  TODO: return the requested data fields
//        if (mObject) {
//            switch (role) {
//            case Qt::DisplayRole:
//                return mObject->display();

//            case Qt::DecorationRole:
//                Heaven::IconRef iref = mObject->icon(true);
//                Heaven::Icon icon = iref.icon();
//                return icon.pixmap();
//            }

//            case ProgressItem::Progress:
//                return percent();

//            case ProgressItem::ProgressInfo:
//                return percent();

//            case Qt::ToolTipRole:
//                return mObject->isDefunct() ? mObject->errorText()
//                                            : mObject->description();
//      }

        return ProgressItem::data(role);
    }

protected:
    T* mObject;

private:
    quint8 percent() const
    {
        int max = mObject->maxProgress();
        int cur = mObject->curProgress();

        if (mObject->minProgress() < 0) {
            // substract a negative minimum
            max += mObject->minProgress();
            cur += mObject->minProgress();
        }

        int percent = max ? (cur / max) * 100 : 0;
        Q_ASSERT(percent >= 0);

        return static_cast<quint8>(percent);
    }
};

//class ProgressItemService
//        : public ProgressItemObject<Activities::Service>
//{
//public:
//    static const ProgressItem::Type StaticType = ProgressItem::Type::Service;

//public:
//    ProgressItemService(ProgressItem* parent, Activities::Service* service);
//};


//class ProgressItemActivity
//        : public ProgressItemObject<Activities::Activity>
//{
//public:
//    static const ProgressItem::Type StaticType = ProgressItem::Type::Activity;

//public:
//    ProgressItemActivity(ProgressItem* parent, Activities::Activity* activity);
//};

//class ProgressItemStep
//        : public ProgressItemObject<Activities::Step>
//{
//public:
//    static const ProgressItem::Type StaticType = ProgressItem::Type::Step;

//public:
//    ProgressItemStep(ProgressItem* parent, Activities::Step* step);
//};
