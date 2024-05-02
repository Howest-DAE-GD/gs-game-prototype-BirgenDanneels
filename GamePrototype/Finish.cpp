#include "pch.h"
#include "Finish.h"
#include "utils.h"
#include <iostream>

Finish::Finish(Point2f pos, float radius)
	:m_Position{ pos },
	m_Radius{ radius },
	m_IsFinished{false}
{
	m_Hitbox = Rectf{ pos.x - radius, pos.y - radius, radius * 2, radius * 2 };
}

void Finish::Draw() const
{

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::FillEllipse(m_Position.x, m_Position.y, m_Radius, m_Radius);

	utils::SetColor(Color4f{ 0.3f, 0.7f, 0.f, 1.f });
	utils::DrawRect(m_Hitbox);
}

bool Finish::HitFinish(Rectf playerHitbox)
{
	if (utils::IsOverlapping(m_Hitbox, playerHitbox))
	{
		m_IsFinished = true;

		std::cout << "You have reached the finish!"; // TODO: iets op het scherm gooien van text ipv console

		return true;
	}
	return false;
}

Rectf Finish::GetHitbox() const
{
	return m_Hitbox;
}

bool Finish::GetFinished() const
{
	return m_IsFinished;
}
