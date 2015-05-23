
#pragma once

//#include <libActivities/Activity.hpp>
//#include <libActivities/Step.hpp>
//#include <libActivities/Service.hpp>

#include <QVariant>

class ProgressItem
{
public:
    enum class Type { Base = 0, Service, Activity, Step };

public:
    static const Type StaticType = Type::Base;

public:
    ProgressItem(ProgressItem* parent);

public:
    virtual int type() const;

    virtual QVariant data(int role) const;
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
//                return mObject->description();

//            case Qt::DecorationRole:
//                Heaven::IconRef iref = mObject->icon(true);
//                Heaven::Icon icon = iref.icon();
//                return icon.pixmap();
//            }
//        }

        return ProgressItem::data(role);
    }

protected:
    T* mObject;
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
