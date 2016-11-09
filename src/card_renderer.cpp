#include "card_list.hpp"
#include "card_shuffler.hpp"
#include "card_sorter.hpp"
#include "csv_tool.hpp"
#include "render_trading_card.hpp"
#include "texture_loader.hpp"
#include "texture_util.hpp"
#include "trading_card.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <iostream>
#include <sstream>

extern char* itoa(int value, char* str, int base);
extern int stricmp (const char *s1, const char *s2);

template<typename Card>
class AlphabetSorter : public CardSorter<Card> {
	//DOCS: reverse, since the linked list inherently reverses shit.
protected:
	int Compare(Card* lhs, Card* rhs) override {
		return stricmp(lhs->GetName().c_str(), rhs->GetName().c_str());
	}
};

typedef CardList<TradingCard, AlphabetSorter<TradingCard>, CardShuffler<TradingCard>> CardListType;

enum class Mode {
	BASIC,
	ARRAY
}mode;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

//a custom written blit function, because the built-in one won't work for some reason
void customBlit(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect) {
	SDL_LockSurface(src);
	SDL_LockSurface(dst);

	//iterate over each pixel in src
	for (int i = srcRect->x; i < srcRect->w; i++) {
		for (int j = srcRect->y; j < srcRect->h; j++) {
			((int*)dst->pixels)[j*dst->w + i + dst->w*dstRect->y + dstRect->x] = ((int*)src->pixels)[j*src->w + i];
		}
	}

	SDL_UnlockSurface(src);
	SDL_UnlockSurface(dst);
}

#undef SDL_BlitSurface
#define SDL_BlitSurface customBlit

//wrap SDL_BlitSurface for textures
void blitTexture(SDL_Renderer* renderer, SDL_Texture* srcTex, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect) {
	SDL_Surface* srcSurface = makeSurfaceFromTexture(renderer, srcTex);
	SDL_BlitSurface(srcSurface, srcRect, dst, dstRect);
	SDL_FreeSurface(srcSurface);
}

//NOTE: this is calibrated for an A4 printing size
void saveCardArray(SDL_Renderer* const, CardListType cardList) {
	TradingCard* masterIterator = cardList.Peek();
	int nameCounter = 0;

	//iterate over cards six at a time
	while(masterIterator) {
		//error
		if (nameCounter > 999) {
			break;
		}

		//get a pointer to each of the nine cards
		constexpr int CARDCOUNT = 9;
		TradingCard *elements[CARDCOUNT];

		for (int i = 0; i < CARDCOUNT; i++) {
			elements[i] = masterIterator;

			//WARNING: this part permanently alters the content of cardList
			if (masterIterator && masterIterator->GetCopies() <= 1) {
				masterIterator = masterIterator->GetNext();
			}
			else if (masterIterator) {
				masterIterator->SetCopies(masterIterator->GetCopies() - 1);
			}
		}

		//Create an empty RGB surface that will be used to create the screenshot bmp file
		//NOTE: magic numbers for dimensions
		SDL_Surface* targetSurface = SDL_CreateRGBSurface(0, elements[0]->GetImage()->GetClipW()*3, elements[0]->GetImage()->GetClipH()*3, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

		if (!targetSurface) {
			throw(std::runtime_error("saveCardArray error"));
		}

		//surface pointer
		SDL_Surface* srcSurface = nullptr;
		SDL_Rect srcRect = {0, 0, elements[0]->GetImage()->GetClipW(), elements[0]->GetImage()->GetClipH()};

		//each element in it's own place
		if (elements[0]) blitTexture(renderer, elements[0]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{0 * srcRect.w, 0 * srcRect.h});
		if (elements[1]) blitTexture(renderer, elements[1]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{1 * srcRect.w, 0 * srcRect.h});
		if (elements[2]) blitTexture(renderer, elements[2]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{2 * srcRect.w, 0 * srcRect.h});
		if (elements[3]) blitTexture(renderer, elements[3]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{0 * srcRect.w, 1 * srcRect.h});
		if (elements[4]) blitTexture(renderer, elements[4]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{1 * srcRect.w, 1 * srcRect.h});
		if (elements[5]) blitTexture(renderer, elements[5]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{2 * srcRect.w, 1 * srcRect.h});
		if (elements[6]) blitTexture(renderer, elements[6]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{0 * srcRect.w, 2 * srcRect.h});
		if (elements[7]) blitTexture(renderer, elements[7]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{1 * srcRect.w, 2 * srcRect.h});
		if (elements[8]) blitTexture(renderer, elements[8]->GetImage()->GetTexture(), &srcRect, targetSurface, &SDL_Rect{2 * srcRect.w, 2 * srcRect.h});

		//save the file
		char nameBuffer[32];
		SDL_SaveBMP(targetSurface, (std::string() + "img/" + std::string(itoa(nameCounter, nameBuffer, 10)) + ".bmp").c_str());
		nameCounter++;

		SDL_FreeSurface(targetSurface);
	}
}

//-------------------------
//Main parts of the program
//-------------------------

void init(int argc, char** argv) {
	//create the singletons
	TextureLoader::CreateSingleton();

	//create and check the window
	int screenWidth = 800;
	int screenHeight = 600;

	//Setting up SDL2
	if (SDL_Init(0)) {
		std::ostringstream msg;
		msg << "Failed to initialize SDL2: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//setting up SDL2_ttf
	if (TTF_Init()) {
		std::ostringstream msg;
		msg << "Failed to initialize SDL_ttf 2.0: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}


	window = SDL_CreateWindow(
		"Example Caption",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth,
		screenHeight,
		SDL_WINDOW_RESIZABLE);

	if (!window) {
		std::ostringstream msg;
		msg << "Failed to create the window: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//create and check the renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		std::ostringstream msg;
		msg << "Failed to create the renderer: " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//screen scaling
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
}

void proc(int argc, char** argv) {
	//textures
	TextureLoader& textureLoader = TextureLoader::GetSingleton();

	textureLoader.Load(renderer, "rsc\\", "back.png");

	//fonts
	TTF_Font* headerFont = TTF_OpenFont("rsc\\belerensmallcaps-bold-webfont.ttf", 24);
	TTF_Font* textFont = TTF_OpenFont("rsc\\beleren-bold-webfont.ttf", 16);

	//check that the font loaded
	if (!headerFont || !textFont) {
		std::ostringstream msg;
		msg << "Failed to load a font file; " << SDL_GetError();
		throw(std::runtime_error(msg.str()));
	}

	//load the card data
	//Format: type;name;text;cost;lower;upper;copies
	CSVObject<7> cardCSV = readCSV<7>("rsc/base_set.csv", ';');

	//create the storage container for mode ARRAY
	CardListType cardList;

	//create each card
	for (auto& it : cardCSV) {
		std::cout << it[1] << std::endl;

		//make and set the new card
		TradingCard* floatingCard = new TradingCard();
		floatingCard->SetType(readType(it[0]));
		floatingCard->SetName(it[1]);
		floatingCard->SetText(it[2]);
		floatingCard->SetCost(std::atoi(it[3].c_str()));
		floatingCard->SetLower(std::atoi(it[4].c_str()));
		floatingCard->SetUpper(std::atoi(it[5].c_str()));
		floatingCard->SetCopies(std::atoi(it[6].c_str()));

		renderTradingCard(renderer, floatingCard, headerFont, textFont);

		cardList.Push(floatingCard);
	}

	//Sort the cards
	cardList.Sort();

	//process each card based on mode
	switch(mode) {
		case Mode::BASIC:
			for (TradingCard* floatingCard = cardList.Peek(); floatingCard; floatingCard = floatingCard->GetNext()) {
				SDL_Surface* surface = makeSurfaceFromTexture(renderer, floatingCard->GetImage()->GetTexture());
				SDL_SaveBMP(surface, (std::string() + "img/" + floatingCard->GetName() + ".bmp").c_str());
				SDL_FreeSurface(surface);
			}
		break;
		case Mode::ARRAY:
			saveCardArray(renderer, cardList);
		break;
	}

	//clean up
	while (cardList.Peek()) {
		delete cardList.Pop();
	}

	TTF_CloseFont(headerFont);
	TTF_CloseFont(textFont);
}

void quit(int argc, char** argv) {
	//close the APIs
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	SDL_Quit();

	//destroy the singletons
	TextureLoader::DeleteSingleton();
}

int main(int argc, char** argv) {
	std::cout << "Beginning " << argv[0] << std::endl;

	if (argc == 1) {
		mode = Mode::BASIC;
	}
	else {
		if (!stricmp(argv[1], "array")) {
			mode = Mode::ARRAY;
		}
	}
	try {
		//open the application
		init(argc, argv);

		//run the main body
		proc(argc, argv);

		//close the application
		quit(argc, argv);
	}
	catch(std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "Clean exit from " << argv[0] << std::endl;
	return 0;
}