#pragma once
#include <memory>
#include <vector>

template <class T>
class IDataMapper
{
	typedef std::shared_ptr<T> WTPtr;
public:
	virtual WTPtr find(long id) = 0;
    virtual std::vector<WTPtr> findAll() = 0;
    virtual void insert(WTPtr item) = 0;
    virtual void update(WTPtr item) = 0;
    virtual void remove(WTPtr item) = 0;
};

