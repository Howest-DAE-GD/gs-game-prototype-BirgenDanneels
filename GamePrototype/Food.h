#pragma once
class Food
{

private:

	Point2f m_Position; //center of food
	Rectf m_Hitbox;

	float m_Radius;
	int m_Value;
	bool m_IsPickedUp;

public:

	explicit Food(Point2f pos, float radius);

	void Draw() const;

	int PickUp();

	bool GetPickedUp() const;
	Rectf GetHitbox() const;

	void Reset();

};

