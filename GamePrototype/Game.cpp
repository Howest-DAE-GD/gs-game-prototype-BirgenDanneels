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
	SVGParser::GetVerticesFromSvgFile("Ground.svg", m_GroundSVGTest);
	SVGParser::GetVerticesFromSvgFile("DeathBoxes.svg", m_DeathBoxes);

	m_VectorFood.push_back(Food{ Point2f{800 ,593 }, 20.f});
	m_VectorFood.push_back(Food{ Point2f{1900 , 643}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{2800 ,643}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{3500 ,643}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{3800 ,700}, 20.f });
	m_VectorFood.push_back(Food{ Point2f{4150 ,700}, 20.f });

	m_pBackground = new Texture{ "Level.png" };

	TTF_Font* font{ TTF_OpenFont("ShapeBitRegular.otf", 100) };
	m_pFinishText = new Texture{ std::string{"YOU FINISHED!"},font, Color4f{1.f,1.f,1.f,1.f} };
	m_pRToResetText = new Texture{ std::string{"Press 'r' to restart."},font, Color4f{1.f,1.f,1.f,1.f} };
	m_pYouDied = new Texture{ std::string{"You died!"},font, Color4f{1.f,1.f,1.f,1.f} };
	m_pFriendDiedText = new Texture{ std::string{"Your friend died. You can reset with 'r' if you are stuck."},font, Color4f{1.f,1.f,1.f,1.f} };
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

	const Rectf dstnRec{ 0,0, m_pBackground->GetWidth(), m_pBackground->GetHeight() };
	m_pBackground->Draw(dstnRec);

	m_Zone1.Draw();

	for (int idx{}; idx < m_VectorFood.size(); ++idx)
	{
		m_VectorFood[idx].Draw();
	}

	m_Finish.Draw();
	m_Stage1Button.Draw();

	m_Friend.Draw();
	m_Player1.Draw();

	glPopMatrix();

	m_Player1.DrawFoodBar();

	if (m_Finish.GetFinished())
	{
		float scale0{ 0.2f };
		Rectf rect1{0, 0, m_pFinishText->GetWidth() / m_pFinishText->GetHeight() * GetViewPort().height * scale0,GetViewPort().height * scale0};
		Rectf dstnRect1{ (GetViewPort().width  - rect1.width) /2.f, GetViewPort().height / 2.f, rect1.width, rect1.height};
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
		utils::FillRect(dstnRect1);
		m_pFinishText->Draw(dstnRect1);

		float scale1{ 0.6f };

		Rectf rect2{0,0,rect1.width * scale1, (m_pRToResetText->GetHeight() / m_pRToResetText->GetWidth()) * rect1.width * scale1 };
		Rectf dstnRect2{ dstnRect1.left + (rect1.width - rect2.width) / 2.f, dstnRect1.bottom - rect2.height, rect2.width, rect2.height };
		utils::FillRect(dstnRect2);
		m_pRToResetText->Draw(dstnRect2);
	}
	else if (m_Player1.GetIsDead())
	{
		float scale0{ 0.2f };
		Rectf rect1{ 0, 0, m_pYouDied->GetWidth() / m_pYouDied->GetHeight() * GetViewPort().height * scale0,GetViewPort().height * scale0 };
		Rectf dstnRect1{ (GetViewPort().width - rect1.width) / 2.f, GetViewPort().height / 2.f, rect1.width, rect1.height };
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
		utils::FillRect(dstnRect1);
		m_pYouDied->Draw(dstnRect1);

		float scale1{ 0.6f };

		Rectf rect2{ 0,0,rect1.width * scale1, (m_pRToResetText->GetHeight() / m_pRToResetText->GetWidth()) * rect1.width * scale1 };
		Rectf dstnRect2{ dstnRect1.left + (rect1.width - rect2.width) / 2.f, dstnRect1.bottom - rect2.height, rect2.width, rect2.height };
		utils::FillRect(dstnRect2);
		m_pRToResetText->Draw(dstnRect2);
	}
	else if (m_Friend.GetIsDead())
	{
		float scale0{ 0.05f };
		Rectf rect1{ 0, 0, m_pFriendDiedText->GetWidth() / m_pFriendDiedText->GetHeight() * GetViewPort().height * scale0, GetViewPort().height * scale0 };
		Rectf dstnRect1{ (GetViewPort().width - rect1.width) / 2.f, GetViewPort().height - (rect1.height * 1.5f), rect1.width, rect1.height };
		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.5f });
		utils::FillRect(dstnRect1);
		m_pFriendDiedText->Draw(dstnRect1);
	}
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
		
		if (m_Player1.GetIsDead())return;

		if (m_Zone1.GetPlayerInZone() && !m_IsControllingFriend && !m_Friend.GetIsDead())
		{
			m_IsControllingFriend = true;
			m_Camera.SetScale(m_FriendCameraScale);
			m_Player1.SetControllingFriend(false);
		}
		else if (m_IsControllingFriend)
		{
			m_IsControllingFriend = false;
			m_Camera.SetScale(m_PlayerCameraScale);
			m_Player1.SetControllingFriend(true);
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
	glClearColor(0.757f, 0.882f, 0.757f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InputHandler(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_R])
	{
		Reset();
	}

	if (m_Finish.GetFinished() || m_Player1.GetIsDead()) return;

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
	m_Player1.SetInSwitchZone(m_Zone1.GetPlayerInZone());
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
