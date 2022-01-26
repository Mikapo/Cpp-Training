#pragma once

#include <unordered_map>
#include <unordered_set>

class garbage_collected_object;

class garbage_collector
{
public:
   
    garbage_collector() = default;
    ~garbage_collector();

    static garbage_collector* get();
    void add_pointer(garbage_collected_object** pointer, garbage_collected_object* owner);
    void garbage_collect();
    void clear();

private:
    struct tracked_object
    {
        tracked_object() = default;
        tracked_object(garbage_collected_object* object, tracked_object* parent)
            : object(object), parent(parent) {}
       

        garbage_collected_object* object = nullptr;
        tracked_object* parent = nullptr;
        std::unordered_map<garbage_collected_object*, tracked_object*> children;
        std::unordered_set<garbage_collected_object**> tracked_pointers;
        bool marked = false;
    };

    void check_object(tracked_object* object);
    void add_object(garbage_collected_object* object, garbage_collected_object* owner = nullptr);
    void remove_object(garbage_collected_object* object);

    tracked_object root = tracked_object();
    std::unordered_map<garbage_collected_object*, tracked_object> objects;
    friend garbage_collected_object;
};

