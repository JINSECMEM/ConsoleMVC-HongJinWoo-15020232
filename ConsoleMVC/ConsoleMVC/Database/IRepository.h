#pragma once
#include <vector>
#include <string>

template<typename T, typename KeyType = std::string>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void           Save(const T& entity)          = 0;
    virtual T              FindById(const KeyType& id)    = 0;
    virtual std::vector<T> FindAll()                      = 0;
    virtual void           Update(const T& entity)        = 0;
    virtual void           Delete(const KeyType& id)      = 0;
};
