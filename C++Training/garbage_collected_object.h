#pragma once

#include "garbage_collector.h"

class garbage_collected_object
{
public:
    virtual ~garbage_collected_object() { garbage_collector::get()->remove_object(this); }

protected:
    garbage_collected_object(garbage_collected_object* owner) { garbage_collector::get()->add_object(this, owner); }

private:
    virtual std::vector<garbage_collected_object**> get_tracked_pointers() = 0;
    friend garbage_collector;
};
