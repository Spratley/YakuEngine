#pragma once

// TODO: AGAIN again implement ID pooling however that happens
class CG_GLRenderTarget
{
public:
    CG_GLRenderTarget(YK_Vector2i p_size);
    ~CG_GLRenderTarget();

    struct Binding
    {
        Binding(YK_Vector2i p_restoreSize) : m_restoreSize(p_restoreSize) {}
        ~Binding();

    private:
        YK_Vector2i m_restoreSize;
    };

    [[nodiscard]] Binding Bind() const;

    YK_U32 GetColorBufferID() const { return m_colorBufferID; }

private:
    YK_Vector2i m_size;
    YK_U32 m_frameBufferID;

    // Should these also be replaced with GL texture objects?
    // Also this should be more generic, either template parameters or dynamic with AddColorTarget etc
    YK_U32 m_colorBufferID;
    YK_U32 m_depthBufferID;
};