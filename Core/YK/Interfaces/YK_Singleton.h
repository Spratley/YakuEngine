#pragma once

// YK_Singleton does not automatically get created and depends on the end-user to construct an instance
// Consider your alternatives before turning to a singleton, global unprotected access is dangerous
template <typename ChildType>
class YK_Singleton
{
public:
    static ChildType* GetInstance() { return s_instance; }

protected:
    inline static ChildType* s_instance = nullptr;
};