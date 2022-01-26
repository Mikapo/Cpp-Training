#pragma once

#include <functional>
#include <optional>
#include <tuple>

namespace mikalib
{
    template <typename T, typename return_type, typename... argtypes>
    std::function<return_type(argtypes...)> compose(
        const std::function<T(argtypes...)>& f1, const std::function<return_type(T)>& f2)
    {
        return [f1, f2](const argtypes&... args) { return f2(f1(args...)); };
    }

    template <typename T, typename a, typename b, typename c>
    auto curry(T (*f)(a, b, c))
    {
        return [=](a arg1) { return [=](b arg2) { return [=](c arg3) { return f(arg1, arg2, arg3); }; }; };
    }

    template <typename return_type, typename... argtypes>
    class promise
    {
    public:
        promise(return_type (*f)(argtypes...), argtypes... args) : f(f), arguments(args...) {}

        return_type call()
        {
            if (!has_calulated)
            {
                result = std::apply(f, arguments);
                has_calulated = true;
            }
            return result;
        }
        return_type operator()() 
        { 
            return call();
        }

    private:
        union 
        {
            return_type result;
            std::tuple<argtypes...> arguments;
        };

        bool has_calulated = false;
        return_type (*f)(argtypes...);
    };

    template <typename return_type, typename... argtypes>
    promise<return_type, argtypes...> delay(return_type (*f)(argtypes...), argtypes... args)
    {
        return promise<return_type, argtypes...>(f, args...);
    }


} // namespace mikalib

template <typename T, typename return_type, typename... argtypes>
std::function<return_type(argtypes...)> operator>>(
    const std::function<T(argtypes...)>& first, const std::function<return_type(T)>& second)
{
    return mikalib::compose(first, second);
}
