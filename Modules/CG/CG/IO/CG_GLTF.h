#pragma once

#include "YK/IO/File/YK_FilePath.h"

#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"

// Simple wrapper around TinyGLTF since it's effectively just a loader/parser
struct CG_GLTF_Mesh
{
    tg3_primitive const& m_primitive;
};

struct CG_GLTF_Skeleton
{
    tg3_skin const& m_skin;
};

class CG_GLTF
{
public:
    CG_GLTF(YK_FilePath const& p_filePath);
    ~CG_GLTF();

    CG_GLTF() = delete;
    CG_GLTF(CG_GLTF const&) = delete;
    CG_GLTF(CG_GLTF&&) = delete;

    CG_GLTF& operator=(CG_GLTF const&) = delete;
    CG_GLTF& operator=(CG_GLTF&&) = delete;

    bool CheckErrors() const;

    bool HasMesh() const { return m_model.meshes_count > 0; }
    bool HasSkeleton() const { return m_model.skins_count > 0; }
    bool HasAnimation() const { return m_model.animations_count > 0; }

    // TODO: Add more than this
    tg3_model const& GetModel() const { return m_model; }

private:
    tg3_model m_model;
    tg3_error_stack m_errorStack;
};