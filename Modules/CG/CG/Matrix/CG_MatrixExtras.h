#pragma once

namespace YK_Matrix
{
	template <typename DataType>
	inline YK_Matrix_R_C<DataType, 4, 4> Perspective(DataType p_fov, DataType p_aspectRatio, DataType p_nearPlane, DataType p_farPlane)
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
	YK_Matrix_R_C<DataType, 4, 4> Orthographic(DataType)
	{
        YK_Matrix_R_C<DataType, 4, 4> result(static_cast<DataType>(0));
	}
}