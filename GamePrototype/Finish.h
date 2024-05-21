#pragma once
class Finish
{
private:

	Point2f m_Position; //center of finish
	Rectf m_Hitbox;

	float m_Radius;
	bool m_IsFinished; //has to e put somewhere else maybe

public:

	explicit Finish(Point2f pos, float radius);

	void Draw() const;
	bool HitFinish(Rectf playerHitbox);

	Rectf GetHitbox() const;
	bool GetFinished() const;

	void Reset();
};

