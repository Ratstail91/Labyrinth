/* Copyright: (c) Kayne Ruse 2016
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
#include "trading_card.hpp"

//-------------------------
//card components
//-------------------------

TradingCard::Type TradingCard::SetType(Type t) {
	return type = t;
}

TradingCard::Type TradingCard::GetType() const {
	return type;
}

std::string TradingCard::SetName(std::string s) {
	return name = s;
}

std::string TradingCard::GetName() const {
	return name;
}

std::string TradingCard::SetText(std::string s) {
	return text = s;
}

std::string TradingCard::GetText() const {
	return text;
}

int TradingCard::SetCost(int i) {
	return cost = i;
}

int TradingCard::GetCost() const {
	return cost;
}

int TradingCard::SetLower(int i) {
	return lower = i;
}

int TradingCard::GetLower() const {
	return lower;
}

int TradingCard::SetUpper(int i) {
	return upper = i;
}

int TradingCard::GetUpper() const {
	return upper;
}

int TradingCard::SetCopies(int i) {
	return copies = i;
}

int TradingCard::GetCopies() const {
	return copies;
}

//-------------------------
//graphics
//-------------------------

int TradingCard::SetPosX(int i) {
	return posX = i;
}

int TradingCard::GetPosX() const {
	return posX;
}

int TradingCard::SetPosY(int i) {
	return posY = i;
}

int TradingCard::GetPosY() const {
	return posY;
}

Image* TradingCard::GetImage() {
	return &image;
}

//-------------------------
//linked list
//-------------------------

TradingCard* TradingCard::SetNext(TradingCard* n) {
	return next = n;
}

TradingCard* TradingCard::GetNext() {
	return next;
}
