#include "garbage_collector.h"

#include "garbage_collected_object.h"
#include <vcruntime_exception.h>

garbage_collector::~garbage_collector() { clear(); }

garbage_collector* garbage_collector::get()
{
    static garbage_collector instance;
    return &instance;
}

void garbage_collector::add_object(garbage_collected_object* object, garbage_collected_object* owner)
{
    if (!object || !owner)
        return;

    if (!owner)
    {
        objects[object] = tracked_object(object, &root);
        root.children[object] = &objects[object];
    }
    else
    {
        if (objects.contains(owner))
        {
            objects[object] = tracked_object(object, &objects[owner]);
            objects[owner].children[object] = &objects[object];
        }
        else
            std::_Xruntime_error("owner is not tracked by garbage_collector");
    }

     for (auto tracked_pointer : object->get_tracked_pointers())
        add_pointer(tracked_pointer, object);
}

void garbage_collector::remove_object(garbage_collected_object* object)
{
    if (objects.contains(object))
    {
        tracked_object& object_to_remove = objects[object];
        object_to_remove.parent->children.erase(object);

        for (auto& children : object_to_remove.children)
               children.second->parent = nullptr;

        objects.erase(object);
    }
}

void garbage_collector::add_pointer(garbage_collected_object** pointer, garbage_collected_object* owner)
{
    if (objects.contains(owner))
    {
        objects[owner].tracked_pointers.insert(pointer);
    }
}
void garbage_collector::garbage_collect()
{
    check_object(&root);
    std::vector<garbage_collected_object*> marked_for_deletion;
    for (auto& object : objects)
    {
        if (object.second.marked)
            objects[object.first].marked = false;
        else
        {
            if (object.second.object)
            {
                marked_for_deletion.push_back(object.second.object);
            }
        }
    }

    for (auto object : marked_for_deletion)
        delete object;
}

void garbage_collector::clear()
{
    root.children.clear();
    garbage_collect();
}

void garbage_collector::check_object(tracked_object* object)
{
    if (!object || object->marked)
        return;

    object->marked = true;
    for (std::pair children : object->children)
        check_object(children.second);

    for (auto tracked_pointer : object->tracked_pointers)
        if (tracked_pointer && objects.contains(*tracked_pointer))
            check_object(&objects[*tracked_pointer]);
}
