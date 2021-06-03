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
#include <assert.h>

namespace olc {

template<class T>
class r2d_generic
{
public: // Constructors
	r2d_generic() : r2d_generic(v2d_generic<T>(0,0), v2d_generic<T>(0,0)) { };
	r2d_generic(const v2d_generic<T>& _pos, const v2d_generic<T>& _size) :
		m_position(_pos), m_size(_size) { };
	r2d_generic(const r2d_generic<T>& _other) :
		m_position(_other.m_position), m_size(_other.m_size) { };
	r2d_generic(const r2d_generic<T>&& _other) :
		m_position(_other.m_position), m_size(_other.m_size) { };
	r2d_generic& operator=(r2d_generic<T>& _other) {
		m_position = _other.m_position;
		m_size = _other.m_size;
	}
	virtual ~r2d_generic() = default;
public: // New methods
	inline v2d_generic<T> tl() const { return m_position; }
	inline v2d_generic<T> br() const { return m_position + m_size; }
	inline v2d_generic<T> size() const { return m_size;	}
	inline void grow(v2d_generic<T> _delta) { m_size += _delta;	}
	inline void move(v2d_generic<T> _delta) { m_position += _delta;	}
	inline v2d_generic<T> center() const { return v2d_generic<T>(m_position / 2); }
	const std::string str() const { return std::string("[") + m_position.str() + ", " + m_size.str() + "]"; }
	friend std::ostream& operator << (std::ostream& os, const r2d_generic<T>& rhs) { os << rhs.str(); return os; }
	r2d_generic<T> operator*(const T& rhs) const { return r2d_generic<T>(m_position, m_size * 2); }
	r2d_generic<T> operator/(const T& rhs) const { return r2d_generic<T>(m_position, m_size / 2); }
	r2d_generic<T>& operator *= (const T& rhs) { m_size *= rhs; return *this; }
	r2d_generic<T>& operator /= (const T& rhs) { m_size /= rhs; return *this; }

private: // Data
	v2d_generic<T> m_position;
	v2d_generic<T> m_size;
};

// Helper type definitions for common types
typedef v2d_generic<float> rf2d;
typedef v2d_generic<double> rd2d;
typedef v2d_generic<std::uint32_t> ri2d;
typedef v2d_generic<std::int32_t> ru2d;

class Camera2D
{
public: // Methods
	Camera2D() { };
	Camera2D(const olc::vf2d& _pos, const olc::vf2d& _viewsize) :
		m_position(_pos), m_viewsize(_viewsize) { };
	inline void Set(const olc::vf2d& _newpos) { m_position = _newpos; }
	inline void Move(const olc::vf2d& _delta) { m_position += _delta; }
	inline const olc::vf2d& Position() const { return m_position; }
// @TODO: lerp functions
private: // Data
	olc::vf2d m_position;
	olc::vf2d m_viewsize;
};

class RenderBatchEntry
{
public:
	RenderBatchEntry(
		olc::Renderable* d, 
		const olc::vf2d& p,
		const olc::vf2d& targetsize,
		float order = 1.0f) : renderable(d), position(p), size(targetsize), z(order) {
	}
	virtual ~RenderBatchEntry() {};

public: // Data
	olc::Renderable* renderable;
	olc::vf2d position;
	olc::vf2d size;
	float z;
};

class RenderBatch : public olc::PGEX
{
public: // Enums
	enum class DrawOrder {
		// lower Z is in front, Z-value increases when deeper
		Z_INC,
		// higher Z is in front. Z-value decreases when deeper
		Z_DECR,
		// Unordered, insertion order
		UNORDERED
	};

public:
	RenderBatch(const DrawOrder& order = DrawOrder::UNORDERED) :
		m_order(order), m_active(false) { };
	virtual ~RenderBatch() {}

	/**
	 * Set the desired Z ordering. 
	 * @param order @see DrawOrder enumeration
	 * @notice Defaults to DrawOrder::UnOrdered
	 */
	void SetOrder(const DrawOrder& order) { m_order = order;}

	/**
	 * Begin drawing with this RenderBatch
	 * Makes this RenderBatch active allowing calls to Draw() and End() etc.
	 */
	void Begin() {
		// Ensure that this RenderBatch is not already active
		assert(!m_active);

		m_active = true;
		m_drawables.clear();
	}

	/**
	 * End drawing and cause all Renderables to be drawn 
	 */
	void End() {
		// Ensure that Begin() was called prior to End()
		assert(m_active);

		for (auto i = m_drawables.begin(); i != m_drawables.end(); ++i) {
			pge->DrawPartialDecal(
				i->position,
				i->size,
				i->renderable->Decal(), 
				vi2d(0,0), 
				vi2d(i->renderable->Sprite()->width, i->renderable->Sprite()->height));
		}
		m_active = false;
	}

	/**
	 * Add a new Renderable to the draw queue. 
	 * Will not draw anything in this method, a call to End() will cause the 
	 * drawing based on objects added with Draw() calls
	 * 
	 * @param renderable A pointer to renderable object containing Sprite and Decal
	 * @param pos Position vector, location where the renderable will be drawn to
	 * @param size Size of the target renderable, will be scaled to fit
	 * @param z Depth value (Z), order depends on which DrawMode was set using SetOrder()
	 */
	void Draw(
		olc::Renderable* _renderable,
		const olc::vf2d& _pos,
		const olc::vf2d& _size,
		float _z, olc::Camera2D* _camera = nullptr)
	{
		// Ensure that Begin() was called and this RenderBatch is active
		assert(m_active);

		if(_renderable != nullptr) {
			RenderBatchEntry entry(
				_renderable,
				_pos,
				_size,
				_z
			);
			insertBatchEntry(entry, _camera);
		}
	}

	/**
	 * Add a new Renderable to the draw queue.
	 * Will not draw anything in this method, a call to End() will cause the
	 * drawing based on objects added with Draw() calls
	 *
	 * @param renderable A pointer to renderable object containing Sprite and Decal
	 * @param pos Position vector, location where the renderable will be drawn to
	 * @param scale Scaling factor, 1.0f means the original bitmap size
	 * @param z Depth value (Z), order depends on which DrawMode was set using SetOrder()
	 */
	void Draw(
		olc::Renderable* _renderable,
		const olc::vf2d& _pos, 
		float _scale, 
		float _z,
		olc::Camera2D* _camera = nullptr) 
	{
		// Ensure that Begin() was called and this RenderBatch is active

		assert(m_active);
		if(_renderable != nullptr) {
			RenderBatchEntry entry(
				_renderable,
				_pos,
				olc::vf2d(_renderable->Sprite()->width * _scale, _renderable->Sprite()->height * _scale),
				_z
			);
			insertBatchEntry(entry, _camera);
		}
	}
private:
	/**
	 * Insert a RenderBatchEntry in desired Z order, either z increasing, 
	 * decreasing or unordered.
	 * Use SetOrder() to set the desired order. Defaults to UNORDERED which 
	 * is the insertion order
	 */
	void insertBatchEntry(RenderBatchEntry& _entry, Camera2D* _camera) {
		auto begin = m_drawables.begin();
		auto end = m_drawables.end();

		olc::vf2d tpos(_entry.position);
		if (_camera != nullptr) {
			tpos = translatePosition(tpos, *_camera);
		}
		_entry.position = tpos;

		if(m_order == DrawOrder::Z_INC) {
			while ((begin != end) && ((*begin).z > _entry.z)) {
				++begin;
			}
			m_drawables.insert(begin, _entry);
		}
		else if (m_order == DrawOrder::Z_DECR) {
			while ((begin != end) && ((*begin).z < _entry.z)) {
				++begin;
			}
			m_drawables.insert(begin, _entry);
		}
		else {			
			m_drawables.push_back(_entry);
		}
	}

	olc::vf2d translatePosition(const olc::vf2d& _pos, const Camera2D& camera) {
		olc::vf2d pos(_pos - camera.Position());
		return pos;
	}

private: // Data
	std::list<RenderBatchEntry> m_drawables;
	DrawOrder m_order = DrawOrder::UNORDERED;
	bool m_active = false;
};

} // namespace olc

#endif // __RENDERBATCH_H_DEFINED__
