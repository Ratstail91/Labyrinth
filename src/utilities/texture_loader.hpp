/* Copyright: (c) Kayne Ruse 2013-2016
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 
 * 3. This notice may not be removed or altered from any source
 * distribution.
*/
#pragma once

#include "image.hpp"
#include "singleton.hpp"

#include <functional>
#include <map>

class TextureLoader : public Singleton<TextureLoader> {
public:
	SDL_Texture* Load(SDL_Renderer*, std::string dirname, std::string fname);
	SDL_Texture* Find(std::string fname);
	void Unload(std::string fname);
	void UnloadAll();
	void UnloadIf(std::function<bool(std::pair<const std::string, Image const&>)> fn);

	int Size();

private:
	friend Singleton<TextureLoader>;
	TextureLoader();
	~TextureLoader();

	std::map<std::string, Image> elementMap;
};