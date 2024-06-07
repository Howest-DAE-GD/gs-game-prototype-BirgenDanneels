#pragma once
#include "utils.h"
#include <vector>
#include "Texture.h"

class Box
{
private:

	float m_Food;
	bool m_IsDead;

	bool m_IsJumping;
	bool m_IsMoving;

	bool m_OnGround;
	bool m_OnPlatform;
	bool m_ControllingFriend;
	bool m_InSwitchZone;

	Point2f m_Position;
	Vector2f m_WalkAndJumpVelocity;

	Vector2f m_Velocity;
	Vector2f m_TransVector;

	Rectf m_Hitbox;
	Rectf m_TransformedHitbox;

	Texture* m_pPressE;
	Texture* m_GotFoodText;
	Texture* m_PlayerHealthText;
	bool m_PickedUpFood;
	float m_PickedUpFoodTextTimer;

	void ResetJump();

	void UpdateMovement(float elapsedSec);

	void ApplyGravity(float elapsedSec);
	void ApplyFriction(float elapsedSec);

	void CheckGround(const std::vector<std::vector<Point2f>>& SVGground, float elapsedSec);
	bool CheckXCollision(const std::vector<Point2f>& object);
	bool CheckYCollision(const std::vector<Point2f>& object, float elapsedSec);

	void DoorCollision(const Rectf rect);

	void CheckDeath(const std::vector<std::vector<Point2f>>& SVGDeath);
	bool CheckXDeath(const std::vector<Point2f>& object);
	bool CheckYDeath(const std::vector<Point2f>& object);

	void ClipToEdge(const utils::HitInfo& hitInfo, bool isXAxes);
	void ClipToEdge(const utils::HitInfo& hitInfo1, const utils::HitInfo& hitInfo2, bool isXAxes);

	void DecreaseFood(float elapsedSec);

public:

	explicit Box(const Point2f& startPos, const Vector2f& walkVelocity);
	Box(Box&) = delete;
	~Box();

	void MoveLeft(float elapsedSec);
	void MoveRight(float elapsedSec);

	void Jump(float elapsedSec);

	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& SVGground, const std::vector<std::vector<Point2f>>& SVGDeath, const Rectf& Door);
	void Draw() const;
	void DrawFoodBar() const;

	void AddFood(int value);

	Rectf GetHitbox() const;
	Point2f GetPos() const;
	bool GetIsDead() const;

	void Reset();
	void SetControllingFriend(bool yes);
	void SetInSwitchZone(bool inZone);
};

