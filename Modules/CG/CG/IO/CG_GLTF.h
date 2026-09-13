#pragma once

#include "YK/Debugging/YK_Assert.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Types/Traits/YK_Concepts.h"

#include "CG/Libraries/TinyGLTF/tiny_gltf_v3.h"

#include <concepts>
#include <type_traits>

// Simple wrapper around TinyGLTF since it's effectively just a loader/parser
namespace CG_GLTF
{
#if !YAKU_RETAIL
    namespace Debug
    {
        template <typename DataType>
        constexpr bool ValidateType(YK_U32 p_type);

        template <typename DataType>
        requires(YK_NumericType<DataType> || std::same_as<DataType, YK_Byte>)
        constexpr bool ValidateType(YK_U32 p_type)
        {
            return p_type == TG3_TYPE_SCALAR;
        }

        template <YK_VectorType DataType>
        constexpr bool ValidateType(YK_U32 p_type)
        {
            return p_type >= TG3_TYPE_VEC2 && p_type <= TG3_TYPE_VEC4 && DataType::Count == p_type;
        }

        template <YK_MatrixType DataType>
        constexpr bool ValidateType(YK_U32 p_type)
        {
            return p_type >= TG3_TYPE_MAT2 && p_type <= TG3_TYPE_MAT4 && DataType::RowCount_V == DataType::ColumnCount_V
                   && DataType::RowCount_V == (p_type - 32); // TG3 offsets matrix types by 32
        }

        template <typename DataType>
        constexpr bool ValidateComponentType(YK_U32 p_componentType)
        {
            constexpr bool isUnsigned = std::is_unsigned_v<DataType>;
            constexpr bool isFloatingPoint = std::is_floating_point_v<DataType>;
            constexpr YK_SizeT size = sizeof(DataType);

            static_assert(!(isUnsigned && isFloatingPoint), "GLTF doesn't support unsigned floating point values!");

            switch (size)
            {
                case 1:
                    return p_componentType == (isUnsigned ? TG3_COMPONENT_TYPE_UNSIGNED_BYTE : TG3_COMPONENT_TYPE_BYTE);
                case 2:
                    return p_componentType
                           == (isUnsigned ? TG3_COMPONENT_TYPE_UNSIGNED_SHORT : TG3_COMPONENT_TYPE_SHORT);
                case 4:
                    {
                        if (isFloatingPoint)
                        {
                            return p_componentType == TG3_COMPONENT_TYPE_FLOAT;
                        }
                        return p_componentType
                               == (isUnsigned ? TG3_COMPONENT_TYPE_UNSIGNED_INT : TG3_COMPONENT_TYPE_INT);
                    }
                case 8: return isFloatingPoint && p_componentType == TG3_COMPONENT_TYPE_DOUBLE;
            }
            YK_ASSERT(false, "Provided type is unsupported by GLTF");
            return false;
        }

        template <typename DataType>
        requires(YK_VectorType<DataType> || YK_MatrixType<DataType>)
        constexpr bool ValidateComponentType(YK_U32 p_componentType)
        {
            return ValidateComponentType<typename DataType::DataType_T>(p_componentType);
        }

        template <typename DataType>
        bool ValidateDataType(tg3_model p_model, YK_U32 p_accessorIndex)
        {
            tg3_accessor accessor = p_model.accessors[p_accessorIndex];
            bool const valid =
              ValidateType<DataType>(accessor.type) && ValidateComponentType<DataType>(accessor.component_type);
            if (!valid)
            {
                YK_LOG_ERROR_PARAM("Type Validation Failed! Expected type: {} {}", accessor.type, accessor.component_type);
            }
            return valid;
        }

    } // namespace Debug
#endif // !YAKU_RETAIL

    struct Mesh
    {
        tg3_primitive const& m_primitive;
    };

    struct Skeleton
    {
        tg3_skin const& m_skin;
    };

    template <typename DataType>
    struct DataView
    {
        constexpr bool IsEmpty() const { return m_count == 0; }
        constexpr YK_SizeT SizeBytes() const { return m_count * sizeof(DataType); }
        constexpr YK_SizeT GetOffsetBytes() const { return IsEmpty() ? 0 : sizeof(DataType); }

        DataType const* m_buffer = nullptr;
        YK_SizeT m_count = 0;
    };

    class File
    {
    public:
        File(YK_FilePath const& p_filePath);
        ~File();

        File() = delete;
        File(File const&) = delete;
        File(File&&) = delete;

        File& operator=(File const&) = delete;
        File& operator=(File&&) = delete;

        bool CheckErrors() const;

        bool HasMesh() const { return m_model.meshes_count > 0; }
        bool HasSkeleton() const { return m_model.skins_count > 0; }
        bool HasAnimation() const { return m_model.animations_count > 0; }

        tg3_model const& GetModel() const { return m_model; }

        template <typename DataType>
        DataView<DataType> ViewData(YK_Int32 p_accessorIndex) const
        {
            YK_ASSERT(Debug::ValidateDataType<DataType>(m_model, p_accessorIndex),
                      "Data type does not match accessor's provided data!");
            YK_Byte const* buffer = nullptr;
            YK_SizeT count = GetBufferAndCount(static_cast<YK_U32>(p_accessorIndex), buffer);
            return DataView<DataType>{ .m_buffer = reinterpret_cast<DataType const*>(buffer), .m_count = count };
        }

    private:
        YK_SizeT GetBufferAndCount(YK_U32 p_accessorIndex, YK_Byte const*& p_outBuffer) const;

    private:
        tg3_model m_model;
        tg3_error_stack m_errorStack;
    };
} // namespace CG_GLTF