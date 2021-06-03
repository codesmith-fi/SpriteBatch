SpriteBatch for PixelGameEngine

Author: Erno Pakarinen
Contact: codesmith.fi@gmail

This is a SpriteBatch implementation in C++ for Pixel Game Engine made by javidx9 (see: https://github.com/OneLoneCoder/olcPixelGameEngine)

SpriteBatch allows to chain multiple Draw calls with Z-ordering and basic 2-D Camera Translations.

For example on how to use, see the demo code in "RenderBatchDemo.cpp". You can use cursor keys to move the camera in the world.

Basic usage.

When you start drawing in your main render loop:

Have a RenderBatch instance somewhere outside your main loop:
   	olc::RenderBatch m_RenderBatch;

Load your assets as Renderables and store them somewhere outside the main loop:
    olc::Renderable m_Ball;
    ...
    m_Ball.Load("Assets\\ball.png");
    ...

First call:
    m_RenderBatch.Begin();

This initializes the RenderBatch instance to accept new Draw() calls. End must be called when you have drawn all sprites.

To Draw sprites, use Draw() calls with or without Camera.
	
	m_RenderBatch.Draw(&m_Ball, position, scalefactor, z_order );

or with camera

    m_RenderBatch.Draw(&m_Ball, position, scalefactor, z_order, &_camera);

For the camera version, you need to of course have an instance of a Camera2D class which currently only handles the world position. The version without Camera argument uses screen coordinates as is.

There are other overloads, for example one which allows drawing the sprite to a target rectangle, automatically scaling the sprite if needed.

To cause the actual drawing happen to the screen call:

    m_RenderBatch.End();

Internally it uses PGE's Decals to draw the sprites with GPU.

I will continue implementing new features for this. To see how the basic features with camera translations work, check here: 

https://www.youtube.com/watch?v=TpriX6zgNaU

Thanks,

See my git hub main page @ https://github.com/codesmith-fi for more details on how to get in touch with me if you so desire.
