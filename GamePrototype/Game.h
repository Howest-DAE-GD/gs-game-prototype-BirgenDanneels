#pragma once
#include "BaseGame.h"
#include "Box.h"
#include "Food.h"
#include "Friend.h"
#include "Camera.h"
#include "Finish.h"
#include "SVGParser.h"
#include "SwitchZone.h"
#include "Button.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	//438
	bool m_IsControllingFriend{ false };

	Box m_Player1 { Point2f { 220, 462 }, Vector2f { 300, 1000 } };
	Friend m_Friend{ Point2f {3840, 1093 }, Vector2f { 300, 1000 } };

	std::vector<Food> m_VectorFood;
	Finish m_Finish{ Point2f{5500 ,670}, 30.f };
	Rectf buttonRect{ 5165 ,1232, 60, 25 };
	Rectf doorRect{ 4450 ,560, 100, 412 };
	Button m_Stage1Button{ buttonRect, doorRect };

	Rectf objectRect{GetViewPort().width * 2 / 5, GetViewPort().height / 3, GetViewPort().width / 5, GetViewPort().height / 10 };
	float m_PlayerCameraScale{ 1.f };
	float m_FriendCameraScale{ 0.5f };
	Camera m_Camera{ GetViewPort().width, GetViewPort().left,objectRect, m_PlayerCameraScale };

	std::vector<std::vector<Point2f>> m_GroundSVGTest;
	std::vector<std::vector<Point2f>> m_DeathBoxes;

	Texture* m_pBackground;
	Texture* m_pFinishText;
	Texture* m_pYouDied;
	Texture* m_pFriendDiedText;

	Texture* m_pRToResetText;

	SwitchZone m_Zone1{ Rectf{3900, 560, 400, 500} };

	void PickUpFood(); //should be in some manager class

	void PlayerUpdateCycle(float elapsedSec);
	void FriendUpdateCycle(float elapsedSec);

	void PlayerControls(float elapsedSec, const Uint8* pStates);
	void FriendContorls(float elapsedSec, const Uint8* pStates);

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void InputHandler(float elapsedSec);

	void Reset();
};