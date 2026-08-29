#pragma once

#include "YK/Types/Containers/YK_FlatSet.h"

#include <type_traits>

template <typename... Parameters>
// Deny RValue Parameters, if the first callback consumes the object then the rest operate on invalid data
requires(!std::is_rvalue_reference_v<Parameters> && ...)
class YK_Callback
{
    using GlobalSignature = void (*)(Parameters...);
    template <GlobalSignature Function>
    static void InvokeGlobal(void*, Parameters... p_parameters)
    {
        Function(std::forward<Parameters>(p_parameters)...);
    }

    template <typename Type, auto Function>
    static void InvokeMember(void* m_object, Parameters... p_parameters)
    {
        (static_cast<Type*>(m_object)->*Function)(std::forward<Parameters>(p_parameters)...);
    }

    struct Callback
    {
        using ThunkSignature = void (*)(void*, Parameters...);
        Callback(void* p_instance, ThunkSignature p_thunk)
            : m_instance(p_instance)
            , m_thunk(p_thunk)
        {}

        Callback(Callback&& p_other)
            : m_instance(p_other.m_instance)
            , m_thunk(p_other.m_thunk)
        {}

        auto operator<=>(Callback const&) const = default;

        void* m_instance = nullptr;
        ThunkSignature m_thunk = nullptr;
    };

public:
    // Global Functions
    template <GlobalSignature Function>
    void Attach()
    {
        m_callbacks.Insert(Callback{ nullptr, &InvokeGlobal<Function> });
    }
    template <GlobalSignature Function>
    void Detach()
    {
        m_callbacks.Remove(Callback{ nullptr, &InvokeGlobal<Function> });
    }

    // Member Functions
    template <typename Type, auto Function>
    void Attach(Type* p_object)
    {
        m_callbacks.Insert(Callback{ static_cast<void*>(p_object), &InvokeMember<Type, Function> });
    }

    template <typename Type, auto Function>
    void Detach(Type* p_object)
    {
        m_callbacks.Remove(Callback{static_cast<void*>(p_object), &InvokeMember<Type, Function>});
    }

    template <typename... CallParameters>
    requires(sizeof...(Parameters) == sizeof...(CallParameters)
             && (std::is_invocable_v<GlobalSignature, CallParameters &&> && ...))
    void operator()(CallParameters&&... p_parameters) const
    {
        for (Callback const& callback : m_callbacks)
        {
            callback.m_thunk(callback.m_instance, std::forward<CallParameters>(p_parameters)...);
        }
    }

private:
    YK_FlatSet<Callback> m_callbacks;
};