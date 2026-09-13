#include "PCH/CG_PCH.h"
#include "CG_GLTF.h"

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/IO/Logging/YK_Logger.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"

namespace CG_GLTF
{
    File::File(YK_FilePath const& p_filePath)
    {
        YK_ASSERT(p_filePath.Extension() == "gltf" || p_filePath.Extension() == "glb",
                  "Non GLTF file provided to CG_GLTF!");

        tg3_parse_options options;

        tg3_parse_options_init(&options);
        tg3_error_stack_init(&m_errorStack);

        tg3_error_code result = tg3_parse_file(&m_model,
                                               &m_errorStack,
                                               p_filePath.CString(),
                                               static_cast<YK_U32>(p_filePath.Length()),
                                               &options);
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

    File::~File()
    {
        tg3_model_free(&m_model);
        tg3_error_stack_free(&m_errorStack);
    }

    bool File::CheckErrors() const
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

    YK_SizeT File::GetBufferAndCount(YK_U32 p_accessorIndex, YK_Byte const*& p_outBuffer) const
    {
        if (p_accessorIndex >= m_model.accessors_count)
        {
            YK_LOG_ERROR_PARAM("Invalid accessor requested! File is likely corrupt.\nAccessor Index: {}",
                               p_accessorIndex);
            p_outBuffer = nullptr;
            return 0;
        }
        tg3_accessor accessor = m_model.accessors[p_accessorIndex];
        tg3_buffer_view bufferView = m_model.buffer_views[accessor.buffer_view];
        tg3_buffer buffer = m_model.buffers[bufferView.buffer];

        YK_ASSERT(accessor.sparse.count == 0, "YakuEn doesn't support sparse GLTF data!");
        YK_ASSERT(bufferView.byte_stride == 0, "YakuEn doesn't support non-continuous GLTF data!");

        p_outBuffer =
          reinterpret_cast<YK_Byte const*>(buffer.data.data + accessor.byte_offset + bufferView.byte_offset);
        return accessor.count;
    }

} // namespace CG_GLTF