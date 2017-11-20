#include"PCTWorkItem.h"

PCTWorkItem::PCTWorkItem(int priority)
    :mPriority(priority)
{
    //
}

bool PCTWorkItem::operator<(const PCTWorkItem& item) const
{
    return mPriority < item.mPriority;
}

bool PCTWorkItem::operator<=(const PCTWorkItem& item) const
{
    return mPriority <= item.mPriority;
}

bool PCTWorkItem::operator>(const PCTWorkItem& item) const
{
    return mPriority > item.mPriority;
}

bool PCTWorkItem::operator>=(const PCTWorkItem& item) const
{
    return mPriority >= item.mPriority;
}
