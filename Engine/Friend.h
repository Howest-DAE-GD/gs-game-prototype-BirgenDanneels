#pragma once
#include "utils.h"
#include <vector>

class Friend
{
private:

	bool m_IsJumping;
	bool m_IsMoving;
	bool m_IsDead;

	bool m_OnGround;

	Point2f m_Position;
	Vector2f m_WalkAndJumpVelocity;

	Vector2f m_Velocity;
	Vector2f m_TransVector;

	Rectf m_Hitbox;
	Rectf m_TransformedHitbox;

	void ResetJump();

	void UpdateMovement(float elapsedSec);

	void ApplyGravity(float elapsedSec);
	void ApplyFriction(float elapsedSec);

	void CheckGround(const std::vector<std::vector<Point2f>>& SVGground, float elapsedSec);
	bool CheckXCollision(const std::vector<Point2f>& object);
	bool CheckYCollision(const std::vector<Point2f>& object, float elapsedSec);

	void CheckDeath(const std::vector<std::vector<Point2f>>& SVGDeath);
	bool CheckXDeath(const std::vector<Point2f>& object);
	bool CheckYDeath(const std::vector<Point2f>& object);

	void ClipToEdge(const utils::HitInfo& hitInfo, bool isXAxes);
	void ClipToEdge(const utils::HitInfo& hitInfo1, const utils::HitInfo& hitInfo2, bool isXAxes);

public:

	explicit Friend(const Point2f& startPos, const Vector2f& walkVelocity);
	Friend(Friend&) = delete;
	~Friend();

	void MoveLeft(float elapsedSec);
	void MoveRight(float elapsedSec);

	void Jump(float elapsedSec);

	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& SVGground, const std::vector<std::vector<Point2f>>& SVGDeath);
	void Draw() const;

	Rectf GetHitbox() const;
	Point2f GetPos() const;

	void Reset();
	bool GetIsDead() const;
};

