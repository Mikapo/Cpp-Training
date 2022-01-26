#pragma once

#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#define ASSERT_SINGLETON_CLASS(TYPE)                                                                                   \
    static_assert(std::is_base_of<singleton, TYPE>::value, "T not derived from Singleton")

class singleton
{
public:
    virtual ~singleton(){};
};

class singleton_manager
{
public:
    ~singleton_manager() { destruct(); }

    // constructs singleton_manager
    // you can not use this manager without contructing it first
    static bool construct()
    {
        if (!manager)
        {
            manager = new singleton_manager();
            return true;
        }
        else
            return false;
    }

    // destructs this manager and all singletons managed by this manager
    static bool destruct()
    {
        if (!manager)
            return false;

        for (std::pair<std::string, singleton*> item : manager->singletons)
        {
            delete item.second;
        }

        delete manager;
        manager = nullptr;
        return true;
    }

    // creates singleton with spesific class
    //  all classes created must inherit from singleton class
    //  if you want to create multiple singletons of same class then use id to identify them from each other. Default id
    //  is 0 after id you can also pass contruction arguments into this function
    template <typename T, typename... argtypes>
    static T* create(int id = 0, argtypes&&... construction_args)
    {
        if (!manager)
            return nullptr;

        ASSERT_SINGLETON_CLASS(T);

        std::string key = typeid(T).name();
        key.append(std::to_string(id));

        if (!manager->singletons.contains(key))
        {
            T* singleton_instance = new T(std::forward<argtypes>(construction_args)...);
            manager->singletons[key] = singleton_instance;
            return singleton_instance;
        }
        else
            return dynamic_cast<T*>(manager->singletons[key]);
    }

    // destroys spesific singleton
    template <typename T>
    static bool destroy(int id = 0)
    {
        if (!manager)
            return false;

        std::string key = typeid(T).name();
        key.append(std::to_string(id));
        if (manager->singletons.contains(key))
        {
            delete manager->singletons[key];
            manager->singletons.erase(key);
            return true;
        }
        else
            return false;
    }

    // returns pointer to spesific singleton
    template <typename T>
    static T* get(int id = 0)
    {
        if (!manager)
            return nullptr;

        ASSERT_SINGLETON_CLASS(T);

        std::string key = typeid(T).name();
        key.append(std::to_string(id));
        if (manager->singletons.contains(key))
        {
            return dynamic_cast<T*>(manager->singletons[key]);
        }
        else
        {
            return nullptr;
        }
    }

private:
    singleton_manager(){};

    std::unordered_map<std::string, singleton*> singletons;
    friend class singleton;
    static singleton_manager* manager;
};
