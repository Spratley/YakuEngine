#pragma once

class CG_Mesh;
class CG_Texture;

struct CG_MeshComponent
{
    CG_Mesh* m_mesh;
};

struct CG_RendererComponent
{
    // TODO: This is where a material would be referenced instead
    CG_Texture* m_texture;
};