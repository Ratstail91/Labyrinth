/* Copyright: (c) Kayne Ruse 2014, 2016
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

#include <cstdlib>

template<typename Card>
class CardShuffler {
public:
	//this is a functor
	void operator()(Card** head);

private:
	//utility methods
	Card* PopRandom(Card** head, int size);
	void PushFront(Card** head, Card* n);
};

template<typename Card>
void CardShuffler<Card>::operator()(Card** head) {
	//count the number of cards
	int size = 0;
	Card* it = *head;
	while (it) it = it->GetNext(), size++;

	//shuffle the cards
	Card* tmpHead = nullptr;
	while(size) {
		PushFront(&tmpHead, PopRandom(head, size));
		size--;
	}

	*head = tmpHead;
}

template<typename Card>
Card* CardShuffler<Card>::PopRandom(Card** head, int size) {
	//choose a card to return
	int i = rand() % size;

	//returning the top card
	if (i == 0) {
		Card* ret = *head;
		(*head) = (*head)->GetNext();
		return ret;
	}

	//find the prev of the card to return
	Card* prev = *head;
	while(--i) prev = prev->GetNext();

	//pop this card
	Card* ret = prev->GetNext();
	prev->SetNext(ret->GetNext());
	ret->SetNext(nullptr);

	return ret;
}

template<typename Card>
void CardShuffler<Card>::PushFront(Card** head, Card* n) {
	n->SetNext(*head);
	(*head) = n;
}
