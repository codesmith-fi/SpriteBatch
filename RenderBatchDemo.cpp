/** 
 * SpriteBatch test application
 * Author: Erno Pakarinen (codesmith.fi@gmail.com)
 */

#include <iostream>
#include <memory>

#define OLC_PGE_APPLICATION
#include "pge/olcPixelGameEngine.h"
#include "RenderBatch.h"
#include "DebugLogger.h"

 // Override base class with your custom functionality
class PGEApplication : public olc::PixelGameEngine
{
public:
	PGEApplication()
	{
		LOG_INFO() << "PGEApplication created";
		sAppName = "PGEApplication - SpriteBatch demo";
	}

	virtual ~PGEApplication() {
		LOG_INFO() << "PGEApplication destroyed";
	}

public:
	bool OnUserCreate() override
	{
		LOG_INFO() << "PGEApplication::OnUserCreate() initializing";
		m_Sprite1.Load("Assets\\desert.png");
		m_Sprite2.Load("Assets\\soniccd.png");
		m_RenderBatch.SetOrder(olc::RenderBatch::DrawOrder::Z_INC);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		olc::vi2d mpos(GetMousePos());

		m_RenderBatch.Begin();
		m_RenderBatch.Draw(
			&m_Sprite1,
			olc::vf2d(0.0f, 0.0f),
			olc::vf2d(ScreenWidth(), ScreenHeight()),
			1.0f
		);
		m_RenderBatch.Draw(
			&m_Sprite2,
			olc::vf2d(mpos.x - m_Sprite2.Sprite()->width/2, 
				mpos.y - m_Sprite2.Sprite()->height/2),
			1.0f,
			0.0f
			);
		m_RenderBatch.End();

		std::string fpss = "FPS: " + std::to_string(GetFPS());

		DrawStringDecal(olc::vf2d(10.0f, 10.0f), fpss);
		return true;
	}

	/**
	 * If OnUserDestroy() returns false, the application will not exit,
	 * it keeps running.
	 */
	bool OnUserDestroy() override
	{
		LOG_INFO() << "PGEApplication::OnUserDestroy()";
		return true;
	}

private:
	olc::RenderBatch m_RenderBatch;
	olc::Renderable m_Sprite1;
	olc::Renderable m_Sprite2;
};

int main()
{
	PGEApplication demo;
	if (demo.Construct(1024, 768, 1, 1))
		demo.Start();
	return 0;
}
