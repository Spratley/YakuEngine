#pragma once

// YKC_Singleton does not automatically get created and depends on the end-user to construct an instance
// Consider your alternatives before turning to a singleton, global unprotected access is dangerous
template <typename ChildType>
class YKC_Singleton
{
public:
    static ChildType* GetInstance() { return s_instance; }

protected:
    inline static ChildType* s_instance = nullptr;
};

// YKC_JITSingleton automatically creates the instance when it is first requested, be prepared for overhead
// ChildType also needs to be trivially constructable, so be prepared to defer any initialization to after it is created
// Consider your alternatives before turning to a singleton, global unprotected access is dangerous
template <typename ChildType>
class YKC_JITSingleton
{
public:
    static ChildType& GetInstance()
    {
        if (!s_instance)
        {
            s_instance = new ChildType();
        }
    }

private:
    inline static ChildType* s_instance = nullptr;
};