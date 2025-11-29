#pragma once

namespace YK_Matrix
{
    template <typename DataType>
    [[nodiscard]] inline YK_Matrix_R_C<DataType, 4, 4> Perspective(DataType p_fov, DataType p_aspectRatio,
                                                                   DataType p_nearPlane, DataType p_farPlane)
    {
        YK_Matrix_R_C<DataType, 4, 4> result(static_cast<DataType>(0));
        float const tanHalfFOV = std::tan((p_fov * (3.1415926f / 180.0f)) / static_cast<DataType>(2));
        result[0][0] = static_cast<DataType>(1) / (p_aspectRatio * tanHalfFOV);
        result[1][1] = static_cast<DataType>(1) / tanHalfFOV;
        result[2][2] = -(p_farPlane + p_nearPlane) / (p_farPlane - p_nearPlane);
        result[2][3] = -static_cast<DataType>(1);
        result[3][2] = -(static_cast<DataType>(2) * p_farPlane * p_nearPlane) / (p_farPlane - p_nearPlane);
        return result;
    }

    template <typename DataType>
    [[nodiscard]] constexpr inline YK_Matrix_R_C<DataType, 4, 4> Orthographic(DataType p_width, DataType p_height,
                                                                              DataType p_depth)
    {
        // Solution provided by
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/orthographic-projection-matrix.html
        YK_Matrix_R_C<DataType, 4, 4> result(static_cast<DataType>(0));
        result[0][0] = static_cast<DataType>(2) / p_width;
        result[1][1] = static_cast<DataType>(2) / p_height;
        result[2][2] = static_cast<DataType>(2) / p_depth;
        result[3][2] = static_cast<DataType>(-1);
        result[3][3] = static_cast<DataType>(1);
        return result;
    }
} // namespace YK_Matrix