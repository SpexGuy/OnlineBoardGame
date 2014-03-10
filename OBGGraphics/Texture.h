#pragma once

class Texture {
public:
	virtual void bind() = 0;
	virtual ~Texture() {}
};
