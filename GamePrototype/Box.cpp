#include "pch.h"
#include "Box.h"
#include <iostream>

void Box::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& SVGground, const std::vector<std::vector<Point2f>>& SVGDeath, const Rectf& Door)
{
	m_PickedUpFoodTextTimer += elapsedSec;
	if (m_IsDead) return;

	if (m_PickedUpFoodTextTimer > 2.f)
	{
		m_PickedUpFoodTextTimer = 0;
		m_PickedUpFood = false;
	}

	DoorCollision(Door);
	DecreaseFood(elapsedSec);

	ApplyGravity(elapsedSec);
	if (!m_IsMoving)ApplyFriction(elapsedSec);

	m_TransVector.x = m_Velocity.x * elapsedSec;
	m_TransVector.y = m_Velocity.y * elapsedSec;

 	CheckGround(SVGground, elapsedSec);
	m_TransVector.x = m_Velocity.x * elapsedSec;
	m_TransVector.y = m_Velocity.y * elapsedSec;
	CheckDeath(SVGDeath);
	UpdateMovement(elapsedSec);

	m_IsMoving = false;
}
void Box::Draw() const
{
	if (m_IsDead) return;

	utils::SetColor(Color4f{ 0.588f, 0.435f, 0.839f, 1.f });
	utils::FillRect(m_TransformedHitbox);

	const Rectf foodBar{ m_TransformedHitbox.left, m_TransformedHitbox.bottom + m_TransformedHitbox.height + 10.f, m_Food / 100.f * m_Hitbox.width, 10.f };
	utils::SetColor(Color4f{ 1.f, 0.75f, 0.75f, 1.f });
	utils::FillRect(foodBar);

	if (m_InSwitchZone)
	{
		const Rectf dstnRectE{ foodBar.left, foodBar.bottom + foodBar.height + 10.f, m_TransformedHitbox.width, m_pPressE->GetHeight() / m_pPressE->GetWidth() * m_TransformedHitbox.width };
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
		utils::FillRect(dstnRectE);
		m_pPressE->Draw(dstnRectE);
	}

	if (m_PickedUpFood)
	{
		const Rectf dstnRectFood{ foodBar.left + m_TransformedHitbox.width * 1.25f, foodBar.bottom + foodBar.height * 3.f, m_TransformedHitbox.width, m_GotFoodText->GetHeight() / m_GotFoodText->GetWidth() * m_TransformedHitbox.width };
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
		utils::FillRect(dstnRectFood);
		m_GotFoodText->Draw(dstnRectFood);
	}

}

void Box::DrawFoodBar() const
{
	if (m_ControllingFriend)
	{
		float scale{ 5.f };
		const Rectf foodBar{ 0, 0, (m_Food / 100.f * m_Hitbox.width) * scale, 10.f * scale};
		const Rectf foodBar100{ 0, 0, m_Hitbox.width * scale, 10.f * scale };

		const Rectf TextRect{ 0,0,  m_PlayerHealthText->GetWidth() / m_PlayerHealthText->GetHeight() * foodBar.height,foodBar.height };
		const Rectf TextDstnRect{ ((scale* m_Hitbox.width) - TextRect.width) / 2.f, 0, TextRect.width, TextRect.height};
		//const Rectf TextSrcRect{ 0,0,  m_PlayerHealthText->GetWidth() * (m_Food / 100.f), m_PlayerHealthText->GetHeight()};

		utils::SetColor(Color4f{ 1.f, 0.75f, 0.75f, 1.f });
		utils::FillRect(foodBar);
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
		utils::DrawRect(foodBar100, scale);
		m_PlayerHealthText->Draw(TextDstnRect);
	}
}

void Box::AddFood(int value)
{
	m_Food += value;
	if (m_Food > 100) m_Food = 100;

	m_PickedUpFood = true;
	m_PickedUpFoodTextTimer = 0;
}

Rectf Box::GetHitbox() const
{
	return m_TransformedHitbox;
}
Point2f Box::GetPos() const
{
	return Point2f{m_TransformedHitbox.left, m_TransformedHitbox.bottom};
}
bool Box::GetIsDead() const
{
	return m_IsDead;
}

void Box::Reset()
{
	m_Position.x = m_Hitbox.left;
	m_Position.y = m_Hitbox.bottom;

	m_IsDead = false;
	m_ControllingFriend = false;
	m_Food = 100.f;
}

void Box::SetControllingFriend(bool yes)
{
	m_ControllingFriend = !yes;
}

void Box::SetInSwitchZone(bool inZone)
{
	m_InSwitchZone = inZone;
}

void Box::MoveLeft(float elapsedSec)
{
	const float speedIncrement{ 2000.f * elapsedSec };

	if (m_Velocity.x <= 0)
	{
		if (m_Velocity.x > -m_WalkAndJumpVelocity.x)
		{
			m_Velocity.x -= speedIncrement;
			if (m_Velocity.x < -m_WalkAndJumpVelocity.x)m_Velocity.x = -m_WalkAndJumpVelocity.x;
		}
		m_IsMoving = !m_IsMoving;
	}

}
void Box::MoveRight(float elapsedSec)
{
	const float speedIncrement{ 2000.f * elapsedSec };

	if (m_Velocity.x >= 0)
	{
		if (m_Velocity.x < m_WalkAndJumpVelocity.x)
		{
			m_Velocity.x += speedIncrement;
			if (m_Velocity.x > m_WalkAndJumpVelocity.x)m_Velocity.x = m_WalkAndJumpVelocity.x;
		}

		m_IsMoving = !m_IsMoving;
	}
}

void Box::Jump(float elapsedSec)
{
	const float maxJumpHoldTime{ 0.2f };

	if (m_IsJumping == false && (m_OnGround || m_OnPlatform))
	{
		m_IsJumping = true;
		m_OnGround = false;
		m_OnPlatform = false;
		m_Velocity.y = m_WalkAndJumpVelocity.y;
	}
}
void Box::ResetJump()
{
	m_IsJumping = false;
}

void Box::UpdateMovement(float elapsedSec)
{
	m_TransVector.x = m_Velocity.x * elapsedSec;
	m_TransVector.y = m_Velocity.y * elapsedSec;


	m_Position.x += m_TransVector.x;
	m_Position.y += m_TransVector.y;
	m_TransformedHitbox.left = m_Position.x;
	m_TransformedHitbox.bottom = m_Position.y;

	//std::cout << "x:" << m_Velocity.x << "           y:" << m_Velocity.y << "\n";
}

void Box::ApplyGravity(float elapsedSec)
{
	const float gravity{ 3000.f * elapsedSec };
	if (!m_OnGround && !m_OnPlatform)
	{
		m_Velocity.y -= gravity;
	}
}
void Box::ApplyFriction(float elapsedSec)
{
	if (m_Velocity.x > 0)
	{
		m_Velocity.x -= m_WalkAndJumpVelocity.x * 6.f * elapsedSec;
		if (m_Velocity.x <= 0)m_Velocity.x = 0;
	}

	if (m_Velocity.x < 0)
	{
		m_Velocity.x += m_WalkAndJumpVelocity.x * 6.f * elapsedSec;
		if (m_Velocity.x >= 0)m_Velocity.x = 0;
	}
}

void Box::CheckGround(const std::vector<std::vector<Point2f>>& SVGground, float elapsedSec)
{
	bool yHit{ false };

	for (std::vector<Point2f> ground : SVGground)
	{
		CheckXCollision(ground);
		if (CheckYCollision(ground, elapsedSec)) yHit = true;
	}
	if (yHit)m_OnGround = true;
}
bool Box::CheckXCollision(const std::vector<Point2f>& object)
{
	//collision next to avatar
	if (m_TransVector.x == 0)return false;

	utils::HitInfo hitAvatarMiddleX{};
	utils::HitInfo hitAvatarBottomX{};

	if (m_TransVector.x > 0)
	{
		const Point2f rightMiddleX{ m_TransformedHitbox.left + m_TransformedHitbox.width - 5.f, m_TransformedHitbox.bottom + m_TransformedHitbox.height / 2 };
		const Point2f rightMiddleXEnd{ m_TransformedHitbox.left + m_TransformedHitbox.width + m_TransVector.x, rightMiddleX.y };

		const Point2f rightBottomX{ m_TransformedHitbox.left + m_TransformedHitbox.width - 5.f, m_TransformedHitbox.bottom + 5.f };
		const Point2f rightBottomXEnd{ m_TransformedHitbox.left + m_TransformedHitbox.width + m_TransVector.x,rightBottomX.y };

		bool rightMiddleXHit{ utils::Raycast(object, rightMiddleX, rightMiddleXEnd, hitAvatarMiddleX) };
		bool rightBottomXHit{ utils::Raycast(object, rightBottomX, rightBottomXEnd, hitAvatarBottomX) };

		if (rightBottomXHit && rightMiddleXHit)ClipToEdge(hitAvatarBottomX, hitAvatarMiddleX, false);
		else if (rightBottomXHit) ClipToEdge(hitAvatarBottomX, false);
		else if (rightMiddleXHit)ClipToEdge(hitAvatarMiddleX, false);

		return false; //idk where i would need a return for so i put alse here without thinking
	}
	else
	{
		const Point2f leftMiddleX{ m_TransformedHitbox.left + 5.f, m_TransformedHitbox.bottom + m_TransformedHitbox.height / 2 };
		const Point2f leftMiddleXEnd{ m_TransformedHitbox.left + m_TransVector.x, leftMiddleX.y };

		const Point2f leftBottomX{ m_TransformedHitbox.left + 5.f, m_TransformedHitbox.bottom + 5.f };
		const Point2f leftBottomXEnd{ m_TransformedHitbox.left + m_TransVector.x,leftBottomX.y };

		bool leftMiddleXHit{ utils::Raycast(object, leftMiddleX, leftMiddleXEnd, hitAvatarMiddleX) };
		bool leftBottomXHit{ utils::Raycast(object, leftBottomX, leftBottomXEnd, hitAvatarBottomX) };

		if (leftBottomXHit && leftMiddleXHit)ClipToEdge(hitAvatarBottomX, hitAvatarMiddleX, false);
		else if (leftBottomXHit)ClipToEdge(hitAvatarBottomX, false);
		else if (leftMiddleXHit)ClipToEdge(hitAvatarMiddleX, false);

		return false; //idk where i would need a return for so i put alse here without thinking
	}
}
bool Box::CheckYCollision(const std::vector<Point2f>& object, float elapsedSec)
{
	//ONLY COLLISION UNDER AVATAR
	if (m_TransVector.y <= 0)
	{
		const Point2f leftBottomY{ m_TransformedHitbox.left, m_TransformedHitbox.bottom + 10.f };
		const Point2f leftBottomYEnd{ leftBottomY.x,  m_TransformedHitbox.bottom + m_TransVector.y };

		const Point2f rightBottomY{ m_TransformedHitbox.left + m_TransformedHitbox.width,leftBottomY.y};
		const Point2f rightBottomYEnd{ rightBottomY.x,  leftBottomYEnd.y };

		const Point2f middleBottomY{ m_TransformedHitbox.left + m_TransformedHitbox.width / 2.f,leftBottomY.y};
		const Point2f middleBottomYEnd{middleBottomY.x, leftBottomYEnd.y};

		utils::HitInfo hitAvatarLeftBottomY{};
		utils::HitInfo hitAvatarRightBottomY{};
		utils::HitInfo hitAvatarMiddleBottomY{};

		bool leftBottomYHit{ utils::Raycast(object, leftBottomY, leftBottomYEnd, hitAvatarLeftBottomY) };
		bool rightBottomYHit{ utils::Raycast(object, rightBottomY, rightBottomYEnd, hitAvatarRightBottomY) };
		bool middleBottomYHit{ utils::Raycast(object, middleBottomY, middleBottomYEnd, hitAvatarMiddleBottomY) };

		if (leftBottomYHit || rightBottomYHit || middleBottomYHit)
		{
			m_OnGround = true;
			if (m_IsJumping) ResetJump();
		}

		if (leftBottomYHit && rightBottomYHit)
		{
			ClipToEdge(hitAvatarLeftBottomY, hitAvatarRightBottomY, true);
			return true;
		}
		else if (leftBottomYHit)
		{
			ClipToEdge(hitAvatarLeftBottomY, true);
			return true;
		}
		else if (rightBottomYHit)
		{
			ClipToEdge(hitAvatarRightBottomY, true);
			return true;
		}
		else if (middleBottomYHit)
		{
			ClipToEdge(hitAvatarMiddleBottomY, true);
			return true;
		}
		else
		{
			m_OnGround = false;
			return false;
		}
	}
	else
	{
		return false; //topcollisions
	}
	return false;
}

void Box::DoorCollision(const Rectf rect)
{
	if (utils::IsOverlapping(rect, m_TransformedHitbox))
	{
		//xcolission
		if ((m_TransformedHitbox.left + m_TransformedHitbox.width - m_TransVector.x <= rect.left)
			&& m_Velocity.x > 0 && (m_TransformedHitbox.left < rect.left + rect.width)) //check left colission
		{
			m_Position.x = rect.left - m_TransformedHitbox.width;
			m_Velocity.x = 0;
		}
		else if ((m_TransformedHitbox.left - m_TransVector.x >= rect.left + rect.width)
			&& m_Velocity.x < 0 && m_TransformedHitbox.left > rect.left)
		{
			m_Position.x = rect.left + rect.width;
			m_Velocity.x = 0;
		}
		//y Collision
		else if ((m_TransformedHitbox.bottom - m_TransVector.y >= rect.bottom + rect.height)
			&& m_Velocity.y <= 0 && m_TransformedHitbox.bottom > rect.bottom)
		{
			m_Position.y = rect.bottom + rect.height;
			m_Velocity.y = 0;
			m_OnPlatform = true;
			if (m_IsJumping) ResetJump();
			//return true;
		}
		else if ((m_TransformedHitbox.bottom + m_TransformedHitbox.height - m_TransVector.y <= rect.bottom)
			&& m_Velocity.y > 0 && ((m_TransformedHitbox.bottom + m_TransformedHitbox.height) < rect.bottom + rect.height))
		{
			m_Position.y = rect.bottom - m_TransformedHitbox.height;
			m_Velocity.y = 0;
			//m_JumpHoldTime = 100;
		}
	}

	m_OnPlatform = false;
	//return false;
}

void Box::CheckDeath(const std::vector<std::vector<Point2f>>& SVGDeath)
{
	for (std::vector<Point2f> ground : SVGDeath)
	{
		if(CheckXDeath(ground))m_IsDead = true;
		if(CheckYDeath(ground))m_IsDead = true;

		if (m_IsDead)
		{
			std::cout << "You died"; // TODO: iets op het scherm gooien van text ipv console
			return;
		}
	}
}
bool Box::CheckXDeath(const std::vector<Point2f>& object)
{
	if (m_TransVector.x == 0)return false;

	utils::HitInfo hitAvatarMiddleX{};
	utils::HitInfo hitAvatarBottomX{};

	if (m_TransVector.x > 0)
	{
		const Point2f rightMiddleX{ m_TransformedHitbox.left + m_TransformedHitbox.width - 5.f, m_TransformedHitbox.bottom + m_TransformedHitbox.height / 2 };
		const Point2f rightMiddleXEnd{ m_TransformedHitbox.left + m_TransformedHitbox.width + m_TransVector.x, rightMiddleX.y };

		const Point2f rightBottomX{ m_TransformedHitbox.left + m_TransformedHitbox.width - 5.f, m_TransformedHitbox.bottom + 5.f };
		const Point2f rightBottomXEnd{ m_TransformedHitbox.left + m_TransformedHitbox.width + m_TransVector.x,rightBottomX.y };

		bool rightMiddleXHit{ utils::Raycast(object, rightMiddleX, rightMiddleXEnd, hitAvatarMiddleX) };
		bool rightBottomXHit{ utils::Raycast(object, rightBottomX, rightBottomXEnd, hitAvatarBottomX) };

		if (rightMiddleXHit || rightBottomXHit)return true;
	}
	else
	{
		const Point2f leftMiddleX{ m_TransformedHitbox.left + 5.f, m_TransformedHitbox.bottom + m_TransformedHitbox.height / 2 };
		const Point2f leftMiddleXEnd{ m_TransformedHitbox.left + m_TransVector.x, leftMiddleX.y };

		const Point2f leftBottomX{ m_TransformedHitbox.left + 5.f, m_TransformedHitbox.bottom + 5.f };
		const Point2f leftBottomXEnd{ m_TransformedHitbox.left + m_TransVector.x,leftBottomX.y };

		bool leftMiddleXHit{ utils::Raycast(object, leftMiddleX, leftMiddleXEnd, hitAvatarMiddleX) };
		bool leftBottomXHit{ utils::Raycast(object, leftBottomX, leftBottomXEnd, hitAvatarBottomX) };


		if(leftBottomXHit || leftMiddleXHit)return true;
	}

	return false;
}
bool Box::CheckYDeath(const std::vector<Point2f>& object)
{
	if (m_TransVector.y > 0)return false;

	const Point2f leftBottomY{ m_TransformedHitbox.left, m_TransformedHitbox.bottom + 10.f };
	const Point2f leftBottomYEnd{ leftBottomY.x,  m_TransformedHitbox.bottom + m_TransVector.y };

	const Point2f rightBottomY{ m_TransformedHitbox.left + m_TransformedHitbox.width, m_TransformedHitbox.bottom + 10.f };
	const Point2f rightBottomYEnd{ rightBottomY.x,  m_TransformedHitbox.bottom + m_TransVector.y };

	utils::HitInfo hitAvatarLeftBottomY{};
	utils::HitInfo hitAvatarRightBottomY{};

	bool leftBottomYHit{ utils::Raycast(object, leftBottomY, leftBottomYEnd, hitAvatarLeftBottomY) };
	bool rightBottomYHit{ utils::Raycast(object, rightBottomY, rightBottomYEnd, hitAvatarRightBottomY) };

	if (leftBottomYHit || rightBottomYHit)	return true;
	else return false;
}
void Box::ClipToEdge(const utils::HitInfo& hitInfo, bool isXAxes)
{
	if (isXAxes)
	{
		m_Position.y = hitInfo.intersectPoint.y;

		m_Velocity.y = 0;
		m_TransVector.y = 0;
	}
	else
	{
		if (m_TransVector.x > 0)
		{
			m_Position.x = hitInfo.intersectPoint.x - m_TransformedHitbox.width - 1;
		}
		else  m_Position.x = hitInfo.intersectPoint.x + 1;

		m_Velocity.x = 0;
		m_TransVector.x = 0;
	}
}
void Box::ClipToEdge(const utils::HitInfo& hitInfo1, const utils::HitInfo& hitInfo2, bool isXAxes)
{
	if (isXAxes)
	{
		if (hitInfo1.intersectPoint.y >= hitInfo2.intersectPoint.y) m_Position.y = hitInfo1.intersectPoint.y;
		else m_Position.y = hitInfo2.intersectPoint.y;

		m_Velocity.y = 0;
		m_TransVector.y = 0;
	}
	else
	{

		if (m_TransVector.x > 0)
		{
			if (hitInfo1.intersectPoint.x <= hitInfo2.intersectPoint.x) m_Position.x = hitInfo1.intersectPoint.x - m_TransformedHitbox.width - 1;
			else  m_Position.x = hitInfo2.intersectPoint.x - m_TransformedHitbox.width - 1;

		}
		else
		{
			if (hitInfo1.intersectPoint.x >= hitInfo2.intersectPoint.x) m_Position.x = hitInfo1.intersectPoint.x + 1;
			else  m_Position.x = hitInfo2.intersectPoint.x + 1;
		}

		m_Velocity.x = 0;
		m_TransVector.x = 0;
	}
}

void Box::DecreaseFood(float elapsedSec)
{
	float foodPerSec{ 20.f };
	if (m_ControllingFriend)foodPerSec /= 4.f;
	const float foodDecrease{ foodPerSec * elapsedSec };
	m_Food -= foodDecrease;

	std::cout << m_Food << "\n";

	if (m_Food <= 0)
	{
		m_IsDead = true;

		std::cout << "Dead! \n"; // TODO: iets op het scherm gooien van text ipv console
	}
}

Box::Box(const Point2f& startPos, const Vector2f& walkVelocity)
	:m_Position{ startPos },
	m_WalkAndJumpVelocity{ walkVelocity },
	m_IsJumping{ false }, m_IsMoving{ false }, m_IsDead{ false }, m_ControllingFriend{ false }, m_OnPlatform{ false }, m_InSwitchZone{false},
	m_OnGround{ true },
	m_Food{ 100 }, m_PickedUpFood{ false }, m_PickedUpFoodTextTimer{0}
{
	m_Hitbox = Rectf{ startPos.x, startPos.y, 100.f, 100.f };
	m_TransformedHitbox = Rectf{ startPos.x, startPos.y, m_Hitbox.width, m_Hitbox.height };

	TTF_Init();

	//extern DECLSPEC TTF_Font* SDLCALL TTF_OpenFont(const char* file, int ptsize);

	TTF_Font* font{TTF_OpenFont("ShapeBitRegular.otf", 100)};
	m_pPressE = new Texture{ std::string{"Press 'E'"},font, Color4f{1.f,1.f,1.f,1.f} };
	m_GotFoodText = new Texture{ std::string{"+50 food!"},font, Color4f{1.f,1.f,1.f,1.f} };
	m_PlayerHealthText = new Texture{ std::string{"Player Food"},font, Color4f{1.f,1.f,1.f,1.f} };
}
Box::~Box()
{
}


