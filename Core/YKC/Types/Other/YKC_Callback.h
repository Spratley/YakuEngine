#pragma once

#include "YKC/Types/Containers/YKC_FlatSet.h"

#include <type_traits>

template <typename ReturnType, typename... Parameters>
// Deny RValue parameters, if we have multiple callbacks and the first one consumes a parameter's values,
// the rest will operate on invalid data
requires(!std::is_rvalue_reference_v<Parameters> && ...)
class YK_Callback
{
public:
    using FunctionSignature = ReturnType (*)(Parameters...);

public:
    void Attach(FunctionSignature const& p_callback) { m_callbacks.Insert(p_callback); }
    void Detach(FunctionSignature const& p_callback) { m_callbacks.Remove(p_callback); }

    void Run(Parameters... p_parameters) const
    {
        for (FunctionSignature const& callback : m_callbacks)
        {
            callback(p_parameters...);
        }
    }

    void operator()(Parameters... p_parameters) const { Run(p_parameters...); }

private:
    YK_FlatSet<FunctionSignature> m_callbacks;
};