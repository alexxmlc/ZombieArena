#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;

class TextureHolder
{
private:
	//Map that contains the pair (string, texture)
	map<string, Texture> m_Textures;

	//Pointer
	static TextureHolder* m_s_Instance;

public:
	TextureHolder();

	//Returns a reference to a texture, more efficient than makig a copy of the texture
	//As the function is static it can be used without an instance of the class: 
	static Texture& GetTexture(string const& filename);

};
#endif


