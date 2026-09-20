

#ifndef OPENCV_FLANN_OBJECT_FACTORY_H_
#define OPENCV_FLANN_OBJECT_FACTORY_H_

#include <map>

namespace cvflann
{

class CreatorNotFound
{
};

template<typename BaseClass,
         typename UniqueIdType,
         typename ObjectCreator = BaseClass* (*)()>
class ObjectFactory
{
    typedef ObjectFactory<BaseClass,UniqueIdType,ObjectCreator> ThisClass;
    typedef std::map<UniqueIdType, ObjectCreator> ObjectRegistry;


    ObjectFactory() {}

public:

    bool subscribe(UniqueIdType id, ObjectCreator creator)
    {
        if (object_registry.find(id) != object_registry.end()) return false;

        object_registry[id] = creator;
        return true;
    }

    bool unregister(UniqueIdType id)
    {
        return object_registry.erase(id) == 1;
    }

    ObjectCreator create(UniqueIdType id)
    {
        typename ObjectRegistry::const_iterator iter = object_registry.find(id);

        if (iter == object_registry.end()) {
            throw CreatorNotFound();
        }

        return iter->second;
    }

    static ThisClass& instance()
    {
        static ThisClass the_factory;
        return the_factory;
    }
private:
    ObjectRegistry object_registry;
};

}

#endif
