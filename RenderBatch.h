/*
	olcPGEX_RenderBatch.h
	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                RenderBatch - v0.1.0			              |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to Pixel Game Engine and aims to implement
	a simple texture atlas (or sprite sheet). Texture atlas is basically
	one big sprite having multiple sub images. These sub images can
	be drawn separately to form textures or animated sprites.
	This class does not animate anything by itself, you must ask the
	Draw() call to draw sub frames separately.

	See the source file TextureAtlasAppPGE.cpp for an example how to use.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2021 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions or derivations of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce the above
	copyright notice. This list of conditions and the following	disclaimer must be
	reproduced in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may
	be used to endorse or promote products derived from this software without specific
	prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	"AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL,	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.

	Links
	~~~~~
	Homepage:	https://github.com/codesmith-fi
	Mail:		codesmith.fi@gmail.com

	Author
	~~~~~~
	Erno Pakarinen (aka xanaki#9472 in discord, aka codesmith-fi)

*/

#ifndef __RENDERBATCH_H_DEFINED__
#define __RENDERBATCH_H_DEFINED__

#include <list>
#include <memory>

namespace olc {

class Camera
{
public: // Methods
	Camera() { };
	Camera(const olc::vf2d& pos, const olc::vf2d& vs) :
		position(pos), viewsize(vs) { };
	void moveTo(const olc::vf2d& newpos) {
		position = newpos;
	}
public: // Data
	olc::vf2d position;
	olc::vf2d viewsize;
};

class RenderBatchEntry
{
public:
	RenderBatchEntry(
		olc::Renderable* d, 
		const olc::vf2d& p,
		const olc::vf2d& s,
		float order) : renderable(d), position(p), depth(order), scale(s) {
	}
	virtual ~RenderBatchEntry() {};

	olc::Renderable* renderable;
	float depth;
	olc::vf2d scale;
	olc::vf2d position;
};

class RenderBatch : public olc::PGEX
{
public: // Enums
	enum class DrawOrder {
		ASCENDING,
		DESCENDING,
		UNORDERED
	};

public:
	RenderBatch(const DrawOrder& order = DrawOrder::ASCENDING) :
		m_order(order) {};
	virtual ~RenderBatch() {}

	void SetOrder(const DrawOrder& order) {
		m_order = order;
	}

	void Begin() {
		m_drawables.clear();
	}

	void End() {
		for (auto i = m_drawables.begin(); i != m_drawables.end(); ++i) {
			pge->DrawDecal(
				i->get()->position, 
				i->get()->renderable->Decal(), 
				i->get()->scale);
		}
	}

	void Draw(
		olc::Renderable* renderable,
		const olc::vf2d& pos, 
		const olc::vf2d& scale, 
		float depth) 
	{
		if (renderable != nullptr) {
			std::unique_ptr<RenderBatchEntry> sbe = std::make_unique<RenderBatchEntry>(
				renderable,
				pos,
				scale,
				depth);

			m_drawables.push_back(std::move(sbe));
		}

		// TODO: Ordering
		/*
		for (auto i = m_drawables.begin(); i != m_drawables.end(); ++i) {

		}
		*/
	}

private:
	std::list<std::unique_ptr<RenderBatchEntry>> m_drawables;
	DrawOrder m_order;
};

} // namespace olc

#endif // __RENDERBATCH_H_DEFINED__
