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

template<typename Card>
class CardSorter {
public:
	//this is a functor
	void operator()(Card** head);

protected:
	//pure virtual base class
	virtual int Compare(Card* lhs, Card* rhs) = 0;

private:
	//utility methods
	Card* PopFront(Card** head);
	void Insert(Card* prev, Card* n);
};

template<typename Card>
void CardSorter<Card>::operator()(Card** head) {
	//don't sort an empty list
	if (!(*head)) return;

	//initialize the tmp list
	Card* tmpHead = PopFront(head);

	while(*head) {
		//get the card to move
		Card* floatingCard = PopFront(head);

		//insert at the start
		if (Compare(floatingCard, tmpHead) < 0) {
			floatingCard->SetNext(tmpHead);
			tmpHead = floatingCard;
			continue;
		}

		//find it's spot
		for (Card* it = tmpHead; it; it = it->GetNext()) {
			//insert at the end
			if (!it->GetNext()) {
				Insert(it, floatingCard);
				break;
			}

			//insert in the middle
			if (Compare(floatingCard, it->GetNext()) < 0) {
				Insert(it, floatingCard);
				break;
			}
		}
	}

	//"return" the sorted list
	(*head) = tmpHead;
}

template<typename Card>
Card* CardSorter<Card>::PopFront(Card** head) {
	Card* ret = *head;
	(*head) = (*head)->GetNext();
	ret->SetNext(nullptr);
	return ret;
}

template<typename Card>
void CardSorter<Card>::Insert(Card* prev, Card* n) {
	n->SetNext(prev->GetNext());
	prev->SetNext(n);
}

//no sorting needed
template<typename Card>
class NullSorter : public CardSorter<Card> {
public:
	//EMPTY

protected:
	virtual int Compare(Card* lhs, Card* rhs) override {
		return 0;
	}
};