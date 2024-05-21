#include "pch.h"
#include "Button.h"
#include "utils.h"
#include <iostream>

Button::Button(Rectf hitbox, Rectf door)
	:m_IsPushed{ false }
{
	m_HitboxButton = hitbox;
	m_HitboxDoor = door;
}

void Button::Draw() const
{
	Rectf button{m_HitboxButton};
	if (m_IsPushed)button.height /= 4;

	utils::SetColor(Color4f{ 0.6f, 0.1f, 0.7f, 1.f });
	utils::FillRect(button);

	if (m_IsPushed)return;
	utils::FillRect(m_HitboxDoor);
}

bool Button::PressButton(Rectf playerHitbox)
{
	if (!m_IsPushed && utils::IsOverlapping(m_HitboxButton, playerHitbox))
	{
		m_IsPushed = true;

		std::cout << "You have pushed the button!"; // TODO: iets op het scherm gooien van text ipv console

		return true;
	}
	return false;
}

Rectf Button::GetDoorHitbox() const
{
	if (!m_IsPushed)return m_HitboxDoor;
	else Rectf{ 0,0,0,0 };
}

bool Button::GetIsPushed() const
{
	return m_IsPushed;
}

void Button::Reset()
{
	m_IsPushed = false;
}
