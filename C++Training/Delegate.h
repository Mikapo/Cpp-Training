#pragma once

#include <memory>
#include <unordered_map>

template <typename... argtypes>
class delegate
{
public:
    delegate() = default;
    ~delegate() { clear(); }

    template <typename object_type>
    bool bind_object(std::weak_ptr<object_type> object, void (object_type::*function)(argtypes...))
    {
        if (object.expired() || !function || function_handles.contains(object))
            return false;

        function_handles[object] = new function_handle<object_type>(object, function);
        return true;
    }
    template <typename object_type>
    bool unbind_object(std::weak_ptr<object_type> object)
    {
        if (!function_handles.contains(object))
            return false;

        delete function_handles[object];
        function_handles.erase(object);
    }
    void broadcast(argtypes... args)
    {
        for (auto& handle : function_handles)
        {
            if (handle.first.expired())
            {
                unbind_object(handle.first);
            }
            else
                handle.second->call(args...);
        }
    }
    void clear()
    {
        for (auto& handle : function_handles)
        {
            delete handle.second;
            function_handles.clear(handle.first);
        }
    }

private:
    struct function_handle_interface
    {
        virtual void call(argtypes... args) = 0;
    };

    template <typename object_type>
    struct function_handle : public function_handle_interface
    {

        function_handle(object_type* object, void (object_type::*function)(argtypes...))
        {
            this->object = object;
            this->function = function;
        }

        void call(argtypes... args) final { (object->*function)(args...); }

        object_type* object = nullptr;
        void (object_type::*function)(argtypes...);
    };

    std::unordered_map<std::weak_ptr<void*>, function_handle_interface*> function_handles;
};
