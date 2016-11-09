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
#include "render_trading_card.hpp"

#include "render_text_texture.hpp"
#include "texture_loader.hpp"

#include <cstring>
#include <list>
#include <sstream>
#include <stdexcept>

constexpr SDL_Color WHITE = {255, 255, 255, 255};
constexpr SDL_Color BLACK = {0, 0, 0, 255};

//utility functions
#ifndef itoa
char* itoa(int value, char* str, int base) {
	//ignore base
	sprintf(str, "%d", value);
	return str;

}
#endif

#ifndef stricmp
//https://thedarkgod.wordpress.com/2009/08/12/stricmp-comparing-two-strings-in-c-case-insensitively/
int stricmp (const char *s1, const char *s2) {
	if (s1 == NULL) return s2 == NULL ? 0 : -(*s2);
	if (s2 == NULL) return *s1;

	char c1, c2;
	while ((c1 = tolower (*s1)) == (c2 = tolower (*s2))) {
		if (*s1 == '\0') break;
		++s1; ++s2;
	}

	return c1 - c2;
}
#endif

TradingCard::Type readType(std::string s) {
	if (!stricmp(s.c_str(), "encounter"))	return TradingCard::Type::ENCOUNTER;
	if (!stricmp(s.c_str(), "item"))		return TradingCard::Type::ITEM;
	std::ostringstream msg;
	msg << "Failed to read the card type: " << s;
	throw(std::runtime_error(msg.str()));
}

std::string writeType(TradingCard::Type type) {
	switch (type) {
		case TradingCard::Type::ENCOUNTER:	return "encounter";
		case TradingCard::Type::ITEM:		return "item";
	}
	std::ostringstream msg;
	msg << "Failed to write the card type: " << type;
	throw(std::runtime_error(msg.str()));
}

void renderTradingCard(SDL_Renderer* const renderer, TradingCard* card, TTF_Font* headerFont, TTF_Font* textFont) {
	//singleton
	TextureLoader& textureLoader = TextureLoader::GetSingleton();

	//background image
	SDL_Texture* backTexture = nullptr;

	switch(card->GetType()) {
		case TradingCard::Type::ENCOUNTER:
			backTexture = textureLoader.Find("back.png");
		break;
		case TradingCard::Type::ITEM:
			backTexture = textureLoader.Find("back.png");
		break;
	}

	if (backTexture == nullptr) {
		throw(std::runtime_error("Could not find needed textures in renderTradingCard"));
	}

	//prep the image for rendering
	int w = 0, h = 0;
	SDL_QueryTexture(backTexture, nullptr, nullptr, &w, &h);
	card->GetImage()->Create(renderer, w, h);
	SDL_SetRenderTarget(renderer, card->GetImage()->GetTexture());

	//variables
	SDL_Texture* tmpPtr = nullptr;
	SDL_Rect dclip;

	//render each component

	//background
	SDL_RenderCopy(renderer, backTexture, nullptr, nullptr);

	//NOTE: don't forget the 5px boarders

	//name
	renderTextDirect(renderer, headerFont, BLACK, card->GetName(), 15, 15);

	//text, splitting the string
	char buffer[512];
	std::list<std::string> linesList;
	strcpy(buffer, card->GetText().c_str());
	const char* cstr = strtok(buffer, "/");
	do {
		linesList.push_back(cstr);
	} while(cstr = strtok(nullptr, "/"));

	int increment = 0;
	for (auto& it : linesList) {
		renderTextDirect(renderer, textFont, BLACK, it, 15, 330 + increment * 16); //TODO variable text size
		increment++;
	}

	//cost
	if (card->GetType() == TradingCard::Type::ITEM) {
		char tmpcstr[16];
		itoa(card->GetCost(), tmpcstr, 10);
		renderTextDirect(renderer, headerFont, BLACK, std::string() + "Cost: " + tmpcstr, 25, 494);
	}

	char tmpcstr[16];

	//lower & upper levels
	std::string lower = itoa(card->GetLower(), tmpcstr, 10);
	std::string upper = itoa(card->GetUpper(), tmpcstr, 10);

	renderTextDirect(renderer, headerFont, BLACK, std::string() + "Levels: " + lower + "-" + upper , 220, 494);

	//cleanup
	SDL_SetRenderTarget(renderer, nullptr);
}
//TODO: better text positions