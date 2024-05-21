#include "pch.h"
#include "Game.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	SVGParser::GetVerticesFromSvgFile("BG.svg", m_GroundSVGTest);
	SVGParser::GetVerticesFromSvgFile("DeathBoxes.svg", m_DeathBoxes);

	m_VectorFood.push_back(Food{ Point2f{653 ,205 }, 20.f});
	m_VectorFood.push_back(Food{ Point2f{1900 ,205}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{2800 ,205}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{3500 ,205}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{3800 ,350}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{4100 ,350}, 20.f });

	//m_Finish = Finish{ Point2f{3800 ,205}, 30.f };
}

void Game::Cleanup( )
{

}

void Game::Update( float elapsedSec )
{
	InputHandler(elapsedSec);
	if (m_Finish.GetFinished()) return;


	if (m_IsControllingFriend) FriendUpdateCycle(elapsedSec);
	else PlayerUpdateCycle(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();
	m_Camera.TranslateAndScaleCamera();

	m_Zone1.Draw();

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	for (std::vector<Point2f> ground : m_GroundSVGTest)
	{
		utils::DrawPolygon(ground, true);
	}

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (std::vector<Point2f> ground : m_DeathBoxes)
	{
		utils::DrawPolygon(ground, true);
	}

	for (int idx{}; idx < m_VectorFood.size(); ++idx)
	{
		m_VectorFood[idx].Draw();
	}

	m_Finish.Draw();
	m_Stage1Button.Draw();

	m_Friend.Draw();
	m_Player1.Draw();

	glPopMatrix();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_e:
		
		if (m_Zone1.GetPlayerInZone() || m_IsControllingFriend)
		{
			m_IsControllingFriend = !m_IsControllingFriend;
			if (m_IsControllingFriend)
			{
				m_Camera.SetScale(m_FriendCameraScale);
				m_Player1.EnableFoodloss(false);
			}
			else
			{
				m_Camera.SetScale(m_PlayerCameraScale);
				m_Player1.EnableFoodloss(true);
			}
		}

		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InputHandler(float elapsedSec)
{

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_R])
	{
		Reset();
	}

	if (m_Finish.GetFinished()) return;

	if (m_IsControllingFriend)FriendContorls(elapsedSec, pStates);
	else PlayerControls(elapsedSec, pStates);
}

void Game::Reset()
{
	m_Player1.Reset();
	m_Friend.Reset();
	m_Finish.Reset();
	m_Stage1Button.Reset();
	m_IsControllingFriend = false;
	m_Camera.SetScale(m_PlayerCameraScale);

	for (int idx{}; idx < m_VectorFood.size(); ++idx)
	{
		m_VectorFood[idx].Reset();
	}
}

void Game::PickUpFood()
{
	for (int idx{}; idx < m_VectorFood.size(); ++idx)
	{
		if (!m_VectorFood[idx].GetPickedUp() && utils::IsOverlapping(m_Player1.GetHitbox(), m_VectorFood[idx].GetHitbox()))
		{
			m_Player1.AddFood(m_VectorFood[idx].PickUp());
		}
	}
}

void Game::PlayerUpdateCycle(float elapsedSec)
{
	m_Friend.Update(elapsedSec, m_GroundSVGTest, m_DeathBoxes);
	m_Player1.Update(elapsedSec, m_GroundSVGTest, m_DeathBoxes, m_Stage1Button.GetDoorHitbox());
	m_Zone1.CheckPlayerInZone(m_Player1.GetHitbox());
	PickUpFood();

	m_Finish.HitFinish(m_Player1.GetHitbox());

	m_Camera.UpdateTranslationCamera(m_Player1.GetPos());
}

void Game::FriendUpdateCycle(float elapsedSec)
{
	m_Player1.Update(elapsedSec, m_GroundSVGTest, m_DeathBoxes, m_Stage1Button.GetDoorHitbox());
	m_Friend.Update(elapsedSec, m_GroundSVGTest, m_DeathBoxes);
	PickUpFood();

	m_Stage1Button.PressButton(m_Friend.GetHitbox());
	m_Camera.UpdateTranslationCamera(m_Friend.GetPos());
}

void Game::PlayerControls(float elapsedSec, const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_D])
	{
		m_Player1.MoveRight(elapsedSec);
	}

	if (pStates[SDL_SCANCODE_A])
	{
		m_Player1.MoveLeft(elapsedSec);
	}

	if (pStates[SDL_SCANCODE_SPACE])
	{
		m_Player1.Jump(elapsedSec);
	}
}

void Game::FriendContorls(float elapsedSec, const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_D])
	{
		m_Friend.MoveRight(elapsedSec);
	}

	if (pStates[SDL_SCANCODE_A])
	{
		m_Friend.MoveLeft(elapsedSec);
	}

	if (pStates[SDL_SCANCODE_SPACE])
	{
		m_Friend.Jump(elapsedSec);
	}
}
