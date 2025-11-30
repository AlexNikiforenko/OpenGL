#include "Texture.h"

#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path) {
	stbi_set_flip_vertically_on_load(1);

	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

	GLenum internalFormat = 0;
	GLenum dataFormat = 0;

	if (data) {
		if (m_BPP == 1) {
			internalFormat = GL_RED;
			dataFormat = GL_RED;
			std::cout << "  -> Format: GL_RED (1 channel, grayscale)" << std::endl;

		}
		else if (m_BPP == 3) {
			internalFormat = GL_RGB;
			dataFormat = GL_RGB;
			std::cout << "  -> Format: GL_RGB (3 channels)" << std::endl;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
		else if (m_BPP == 4) {
			internalFormat = GL_RGBA;
			dataFormat = GL_RGBA;
			std::cout << "  -> Format: GL_RGBA (4 channels)" << std::endl;
		}
		else {
			std::cerr << "  -> UNKNOWN BPP: " << m_BPP << std::endl;
			stbi_image_free(data);
			return;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		stbi_image_free(data);
	}
	else {
		std::cout << "ERROR::TEXTURE::LOAD_FAILD" << path << "\n";
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &m_ID);
}

void Texture::bind(unsigned int slot) {
	if (m_ID == 0) return;

	if (slot >= 0 && slot < 32)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}