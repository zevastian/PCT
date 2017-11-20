#include"PCTNetworkItem.h"

PCTNetworkItem::PCTNetworkItem(int priority)
    :mPriority(priority)
{
    //
}

bool PCTNetworkItem::operator<(const PCTNetworkItem& item) const
{
    return mPriority < item.mPriority;
}

bool PCTNetworkItem::operator<=(const PCTNetworkItem& item) const
{
    return mPriority <= item.mPriority;
}

bool PCTNetworkItem::operator>(const PCTNetworkItem& item) const
{
    return mPriority > item.mPriority;
}

bool PCTNetworkItem::operator>=(const PCTNetworkItem& item) const
{
    return mPriority >= item.mPriority;
}
