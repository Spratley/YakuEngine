// This is kinda a placeholder until I can make a better platform wrapper.
// Uses CRTP, ideally there should be validation that DerivedPlatformType derives from YK_PlatformCoreBase,
// but what can you do, eh?

template <typename DerivedPlatformType>
class YKC_PlatformCoreBase
{
public:
    static DerivedPlatformType& GetInstance() { return *s_instance; }

    YKC_PlatformCoreBase() 
    {
        if (s_instance)
        {
            YK_LOG_ERROR("Re-creating platform core instance! Something is VERY wrong!");
            return;
        }
        s_instance = static_cast<DerivedPlatformType*>(this);
    }

private:
    inline static DerivedPlatformType* s_instance = nullptr;
};