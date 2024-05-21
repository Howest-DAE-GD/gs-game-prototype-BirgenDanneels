#pragma once

class SwitchZone
{
private:
	Rectf m_Zone;
	bool m_PlayerIsInZone;

public:
	explicit SwitchZone(const Rectf& zoneRect);
	~SwitchZone() noexcept;

	void CheckPlayerInZone(const Rectf player);
	void Draw()const;

	bool GetPlayerInZone() const;

};

