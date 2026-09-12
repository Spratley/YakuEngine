#include "PCH/CG_PCH.h"
#include "CG_GLTF.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"

CG_GLTF::CG_GLTF(YK_FilePath const& p_filePath)
{
    YK_ASSERT(p_filePath.Extension() == "gltf" || p_filePath.Extension() == "glb",
              "Non GLTF file provided to CG_GLTF!");

    tg3_parse_options options;

    tg3_parse_options_init(&options);
    tg3_error_stack_init(&m_errorStack);

    tg3_error_code result =
      tg3_parse_file(&m_model, &m_errorStack, p_filePath.CString(), static_cast<YK_U32>(p_filePath.Length()), &options);
    if (result != TG3_OK)
    {
        CheckErrors();
        return;
    }

    YK_ASSERT(m_model.meshes_count <= 1, "YakuEn doesn't support multi-mesh files yet!");
    if (m_model.meshes_count == 1)
    {
        YK_ASSERT(m_model.meshes[0].primitives_count == 1, "YakuEn doesn't support multi-mesh files yet!");
    }
    YK_ASSERT(m_model.skins_count <= 1, "YakuEn doesn't support multi-skeleton files yet!");
}

CG_GLTF::~CG_GLTF()
{
    tg3_model_free(&m_model);
    tg3_error_stack_free(&m_errorStack);
}

bool CG_GLTF::CheckErrors() const
{
#if !YAKU_RETAIL
    for (auto i : YK_CountTo(m_errorStack.count))
    {
        tg3_error_entry const& error = m_errorStack.entries[i];
        YK_LOG_ERROR_PARAM("GLTF Error [{}]: {}",
                           static_cast<YK_U32>(error.severity),
                           error.message ? error.message : "No Message");
    }
#endif // !YAKU_RETAIL
    return m_errorStack.has_error;
}