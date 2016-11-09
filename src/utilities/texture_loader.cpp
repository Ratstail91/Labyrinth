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
#include "texture_loader.hpp"

TextureLoader::TextureLoader() {
	//EMPTY
}

TextureLoader::~TextureLoader() {
	UnloadAll();
}

SDL_Texture* TextureLoader::Load(SDL_Renderer* renderer, std::string dirname, std::string fname) {
	//if this file is already loaded, return the loaded version rather than a new one
	std::map<std::string, Image>::iterator it = elementMap.find(fname);
	if (it == elementMap.end()) {
		elementMap[fname].Load(renderer, dirname + fname);
		return elementMap[fname].GetTexture();
	}
	else {
		return it->second.GetTexture();
	}
}

SDL_Texture* TextureLoader::Find(std::string fname) {
	std::map<std::string, Image>::iterator it = elementMap.find(fname);
	if (it == elementMap.end()) {
		return nullptr;
	}
	else {
		return it->second.GetTexture();
	}
}

void TextureLoader::Unload(std::string fname) {
	elementMap.erase(fname);
}

void TextureLoader::UnloadAll() {
	elementMap.clear();
}

void TextureLoader::UnloadIf(std::function<bool(std::pair<const std::string, Image const&>)> fn) {
	std::map<std::string, Image>::iterator it = elementMap.begin();
	while (it != elementMap.end()) {
		if (fn(*it)) {
			it = elementMap.erase(it);
		}
		else {
			++it;
		}
	}
}

int TextureLoader::Size() {
	return elementMap.size();
}