#include "pch.h"
#include "SwitchZone.h"
#include "utils.h"

SwitchZone::SwitchZone(const Rectf& zoneRect)
	:m_Zone{zoneRect},
	m_PlayerIsInZone{false}
{
}

SwitchZone::~SwitchZone() noexcept
{
}

void SwitchZone::CheckPlayerInZone(const Rectf player)
{
	if (player.left >= m_Zone.left && (player.left + player.width) <= (m_Zone.left + m_Zone.width)
		&& player.bottom >= m_Zone.bottom && player.bottom <= m_Zone.bottom + m_Zone.height)
	{
		m_PlayerIsInZone = true;
	}
	else m_PlayerIsInZone = false;
}

void SwitchZone::Draw() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
	utils::DrawRect(m_Zone);
}

bool SwitchZone::GetPlayerInZone() const
{
	return m_PlayerIsInZone;
}
