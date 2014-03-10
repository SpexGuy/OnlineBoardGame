#pragma once

class Material {
public:
	virtual void bind() = 0;
	virtual ~Material() {}
};
