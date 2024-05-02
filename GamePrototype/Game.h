#pragma once
#include "BaseGame.h"
#include "Box.h"
#include "Food.h"
#include "Camera.h"
#include "Finish.h"
#include "SVGParser.h"

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

	Box m_Player1 { Point2f { 100, 200 }, Vector2f { 300, 1000 } };
	std::vector<Food> m_VectorFood;
	Finish m_Finish{ Point2f{3800 ,205}, 30.f };

	Rectf objectRect{GetViewPort().width * 2 / 5, GetViewPort().height / 3, GetViewPort().width / 5, GetViewPort().height / 10 };

	Camera m_Camera{ GetViewPort().width, GetViewPort().left,objectRect, 1.f };

	std::vector<std::vector<Point2f>> m_GroundSVGTest;
	std::vector<std::vector<Point2f>> m_DeathBoxes;

	void PickUpFood(); //should be in some manager class

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void InputHandler(float elapsedSec);
};