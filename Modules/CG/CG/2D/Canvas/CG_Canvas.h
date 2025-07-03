#pragma once

class CG_Canvas
{
public:
	void Render() const;

	YK_U8 GetPriority() const { return m_priority; }

	void Scroll(YK_Vector2f const& p_scroll);
	void Spin(float p_angleDegrees);

	void SetPosition(YK_Vector2f const& p_position);
	void SetRotation(float p_angleDegrees);

private:
	YK_U8 m_priority;
	YK_Matrix44 m_transform;
};