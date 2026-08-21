#pragma once

class CG_Mesh;
class CG_Material;

struct CG_MeshComponent
{
    CG_Mesh const* m_mesh;
};

struct CG_RendererComponent
{
    CG_Material* m_material;
};