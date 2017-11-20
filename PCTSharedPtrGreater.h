#include<memory>

#ifndef PCT_SHAREDPTRGREATER_H_
#define PCT_SHAREDPTRGREATER_H_

template <typename T>
struct PCTSharedPtrGreater {
    bool operator()(const std::shared_ptr<T>& shared1, const std::shared_ptr<T>& shared2)
    {
        return (*shared1) > (*shared2);
    }
};

#endif
