#pragma once
#include <string>

class Texture
{
public:
	Texture() = default;

	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0);
	void unbind();

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline unsigned int getID() const { return m_ID; }

private:
	unsigned int m_ID = 0;

	int m_Width = 0;
	int m_Height = 0;

	int m_BPP = 0;

};

