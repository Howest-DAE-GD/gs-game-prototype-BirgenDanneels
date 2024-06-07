#include "pch.h"
#include "Food.h"
#include "utils.h"

Food::Food(Point2f pos, float radius)
	:m_Position{pos},
	m_Radius{radius},
	m_IsPickedUp{false},
	m_Value{50}
{
	m_Hitbox = Rectf{pos.x - radius, pos.y - radius, radius * 2, radius * 2};
}

void Food::Draw() const
{
	if (m_IsPickedUp)return;

	utils::SetColor(Color4f{ 0.467f, 0.62f, 0.796f, 1.f });
	utils::FillEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);

}

int Food::PickUp()
{
	m_IsPickedUp = true;

	// TODO: iets op het scherm gooien van text dat je +X food hebt gekregen

	return m_Value;
}

bool Food::GetPickedUp() const
{
	return m_IsPickedUp;
}

Rectf Food::GetHitbox() const
{
	return m_Hitbox;
}

void Food::Reset()
{
	m_IsPickedUp = false;
}
