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
	RenderableSprite() : z(0.0f) { };
	olc::Renderable* renderable;
	olc::vf2d pos;
	olc::vf2d size;
	float z;
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

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist_x(0, ScreenWidth()-1);
		std::uniform_int_distribution<std::mt19937::result_type> dist_y(0, ScreenHeight() - 1);
		std::uniform_int_distribution<std::mt19937::result_type> dist_scale(10, 100);

		for (int i = 0; i < 100; ++i) {
			RenderableSprite s;
			s.z = (float)dist_scale(rng) / 100.0f;
			s.pos.x = (float)dist_x(rng);
			s.pos.y = (float)dist_y(rng);
			s.pos_delta = olc::vf2d(dist_scale(rng)/100.0f, dist_scale(rng) / 100.0f);
			s.renderable = &m_Ball;
			float scale = (float)dist_scale(rng) / 100.0f;
			s.size.x = m_Ball.Sprite()->width * scale;
			s.size.y = m_Ball.Sprite()->height * scale;		
			m_sprites.push_back(s);
		}
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
		m_RenderBatch.Draw(
			&m_Sprite2,
			olc::vf2d(mpos.x - ((float)m_Sprite2.Sprite()->width) / 2.0f,
				mpos.y - ((float)m_Sprite2.Sprite()->height) / 2.0f),
			1.0f,
			0.4f
			);

		for (auto i = m_sprites.begin(); i != m_sprites.end(); ++i) {
			m_RenderBatch.Draw(
				i->renderable,
				i->pos,
				i->size,
				i->z
			);

			i->pos += i->pos_delta;
			if(i->pos.x <= (0 - i->renderable->Sprite()->width/2) || 
				i->pos.x >= ScreenWidth() - (i->renderable->Sprite()->width / 2))
			{
				i->pos_delta.x = -1.0f * i->pos_delta.x;
			}
			if(i->pos.y <= (0 - i->renderable->Sprite()->height/2) || 
				i->pos.y >= (ScreenHeight() - i->renderable->Sprite()->height / 2))
			{
				i->pos_delta.y = -1.0f * i->pos_delta.y;
			}

		}

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
	olc::Renderable m_Ball;
	std::vector<RenderableSprite> m_sprites;
};

int main()
{
	PGEApplication demo;
	if (demo.Construct(1024, 768, 1, 1))
		demo.Start();
	return 0;
}
