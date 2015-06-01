#include "ProgressItems.hpp"


ProgressItem::ProgressItem(ProgressItem* parent)
    : mParent(parent)
{
}

int ProgressItem::type() const
{
    return static_cast<int>(StaticType);
}

QVariant ProgressItem::data(int role) const
{
    Q_UNUSED(role);
    return QVariant();
}


//ProgressItemService::ProgressItemService(ProgressItem* parent,
//                                         Activities::Service* service)
//    : ProgressItemObject(parent, object)
//{

//}


//ProgressItemActivity::ProgressItemActivity(ProgressItem* parent,
//                                           Activities::Activity* activity)
//    : ProgressItemObject(parent, activity)
//{

//}


//ProgressItemStep::ProgressItemStep(ProgressItem* parent, Activities::Step* step)
//{

//}
