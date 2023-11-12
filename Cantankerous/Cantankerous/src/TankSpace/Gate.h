#pragma once

#include <FlatWhite.h>

class Gate : public fw::GameObject
{
public:
	Gate(
		const fw::Vec2f& position,
		const fw::Vec2f& size,
		std::shared_ptr<fw::Texture> texture
	);
};

