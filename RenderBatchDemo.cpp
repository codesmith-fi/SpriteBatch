/** 
 * SpriteBatch test application
 * Author: Erno Pakarinen (codesmith.fi@gmail.com)
 */

#include <iostream>
#include <memory>
#include <array>
#include <random>

#define OLC_PGE_APPLICATION
#include "pge/olcPixelGameEngine.h"
#include "RenderBatch.h"
#include "DebugLogger.h"

class RenderableSprite
{
public:
	RenderableSprite() = default;
	virtual ~RenderableSprite() = default;
public: // data
	olc::Renderable* renderable = nullptr;
	olc::vf2d pos;
	olc::vf2d size;
	float z = 0.0f;
	olc::vf2d pos_delta;
};

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
		m_Ball.Load("Assets\\ball.png");
		m_RenderBatch.SetOrder(olc::RenderBatch::DrawOrder::Z_INC);
		m_camera.Set(olc::vf2d(0.0f, 0.0f));
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		olc::vi2d mpos(GetMousePos());

		m_RenderBatch.Begin();
		m_RenderBatch.Draw(
			&m_Sprite1,
			olc::vf2d(0.0f, 0.0f),
			olc::vf2d((float)ScreenWidth(), (float)ScreenHeight()),
			1.0f
		);
		float sw = ((float)m_Ball.Sprite()->width);
		float sh = ((float)m_Ball.Sprite()->height);
		float rw = (float)ScreenWidth();
		float rh = (float)ScreenHeight();
		m_RenderBatch.Draw(
			&m_Ball,
			olc::vf2d(0.0f - sw/2, 0.0f-sh/2),
			1.0f,
			0.4f,
			&m_camera
		);
		m_RenderBatch.Draw(
			&m_Ball,
			olc::vf2d(rw - sw/2, 0.0f - sh / 2),
			1.0f,
			0.4f,
			&m_camera
		);
		m_RenderBatch.Draw(
			&m_Ball,
			olc::vf2d(0.0f - sw / 2, rh - sh/2),
			1.0f,
			0.4f,
			&m_camera
		);
		m_RenderBatch.Draw(
			&m_Ball,
			olc::vf2d(rw - sw / 2, rh - sh/2),
			1.0f,
			0.4f,
			&m_camera
		);
		m_RenderBatch.Draw(
			&m_Ball,
			olc::vf2d(rw/2-sw/2, rh/2-sh/2),
			1.0f,
			0.4f,
			&m_camera
		);
		m_RenderBatch.Draw(
			&m_Ball,
			olc::vf2d( mpos.x - sw/2*0.5f, mpos.y - sh/2*0.5f),
			0.5f,
			0.1f
			);
		m_RenderBatch.End();

		std::string fpss = "FPS: " + std::to_string(GetFPS());
		DrawStringDecal(olc::vf2d(10.0f, 10.0f), fpss);

		olc::vf2d delta(0.0f, 0.0f);
		if(GetKey(olc::Key::LEFT).bHeld) {
			delta.x = -0.1;
		}
		if (GetKey(olc::Key::RIGHT).bHeld) {
			delta.x = 0.1;
		}
		if (GetKey(olc::Key::UP).bHeld) {
			delta.y = -0.1;
		}
		if (GetKey(olc::Key::DOWN).bHeld) {
			delta.y = 0.1;
		}
		m_camera.Move(delta);
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
	olc::Renderable m_Ball;
	olc::Camera2D m_camera;
	olc::vf2d m_prevMousePos;
};

int main()
{
	PGEApplication demo;
	if (demo.Construct(1024, 768, 1, 1))
		demo.Start();
	return 0;
}
