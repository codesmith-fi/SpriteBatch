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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		m_RenderBatch.Begin();
		m_RenderBatch.Draw(
			&m_Sprite1,
			olc::vf2d(0.0f, 0.0f),
			olc::vf2d(1.0f, 1.0f),
			1.0f
		);
		m_RenderBatch.Draw(
			&m_Sprite2,
			olc::vf2d(-300.0f, 50.0f),
			olc::vf2d(1.0f, 1.0f),
			1.0f
			);
		m_RenderBatch.End();
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
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}
