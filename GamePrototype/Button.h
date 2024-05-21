#pragma once
class Button
{
private:
	Rectf m_HitboxButton;
	Rectf m_HitboxDoor;
	bool m_IsPushed;

public:

	explicit Button(Rectf hitbox, Rectf door);

	void Draw() const;
	bool PressButton(Rectf playerHitbox);

	Rectf GetDoorHitbox() const;
	bool GetIsPushed() const;

	void Reset();
};

