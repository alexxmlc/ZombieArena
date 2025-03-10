#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	//Make sure m_s_Instance = nullptr, if it doesn t the game will exit execution
	assert(m_s_Instance == nullptr);

	//Assing the ponter to *this* instance
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	//Get a reference to m_Textures using m_s_Instance
	// auto = map<string, Texture>
	auto& m = m_s_Instance->m_Textures;

	//Create an iterator to hold the key-value-pair and search for required kvp in the file "filename"
	//auto = map<string, Texture>::iterator
	auto keyValuePair = m.find(filename);

	if (keyValuePair != m.end())
	{
		//If found matchign key return the texture (the second part of the kvp)
		return keyValuePair->second;
	}
	else
	{
		//If not found create new kvp using filename
		auto& texture = m[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}