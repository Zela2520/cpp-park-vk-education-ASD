// Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку. Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.

// Формат ввода
// Начальная расстановка.

// Формат вывода
// Если вам удалось найти решение, то в первой строке файла выведите число перемещений, которое требуется сделать в вашем решении. А во второй строке выведите соответствующую последовательность ходов: L означает, что в результате перемещения костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.


#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_map>

#define DESK_SIZE 16

class MyPlayCondition {
public:
	MyPlayCondition();
	bool operator==(const MyPlayCondition& other) const;
	bool operator!=(const MyPlayCondition& other) const {return !(*this == other);}

	struct Hasher {size_t operator()(const MyPlayCondition& other) const;};
	struct Comparator {bool operator()(const MyPlayCondition& left, const MyPlayCondition& right) const;};

    MyPlayCondition runUp() const;
	MyPlayCondition runDown() const;
	MyPlayCondition runLeft() const;
	MyPlayCondition runRight() const;

	void findOptimumPath();

	bool checkWayLeft() const {return nullState % 4 != 0;}
	bool checkWayRight() const {return nullState % 4 != 3;}
	bool checkWayUp() const {return nullState > 3;}
	bool checkWayDown() const {return nullState < 12;}
	
	bool checkAllPath() const;

	uint8_t nullState;
	uint8_t closestPath;
	uint8_t optimumPath;
	uint8_t places[DESK_SIZE];
};

MyPlayCondition::MyPlayCondition(): nullState(0), closestPath(0), optimumPath(0), places{} {}

void improveRes(std::unordered_map<MyPlayCondition, std::pair<MyPlayCondition, char>, MyPlayCondition::Hasher> &visited,
std::set<MyPlayCondition, MyPlayCondition::Comparator> &setCondition, MyPlayCondition &curCondition, MyPlayCondition nextCondition, char move) {
	auto newState = visited.find(nextCondition);

	if (newState == visited.end()) {
		visited[nextCondition] = std::make_pair(curCondition, move);

		nextCondition.closestPath = curCondition.closestPath + 1;
		setCondition.insert(nextCondition);
	} else if (nextCondition.closestPath > curCondition.closestPath + 1) {
		setCondition.erase(newState->first);
		
		nextCondition.closestPath = curCondition.closestPath + 1;
		setCondition.insert(nextCondition);
	}
}

bool MyPlayCondition::operator==(const MyPlayCondition &game_state) const {
	if (!(nullState == game_state.nullState)) return false;

	for (size_t i = 0; i < DESK_SIZE; i++) {
		if (places[i] != game_state.places[i]) return false;
	}

	return true;
}



bool searchResult(MyPlayCondition &gameCondition, std::string &target) {
	gameCondition.findOptimumPath();
	std::unordered_map<MyPlayCondition, std::pair<MyPlayCondition, char>, MyPlayCondition::Hasher> visited;

	visited[gameCondition] = std::make_pair(gameCondition, 0);
	std::set<MyPlayCondition, MyPlayCondition::Comparator> setCondition;

	setCondition.insert(gameCondition);

	do {
		MyPlayCondition curPosition = *setCondition.begin();
		setCondition.erase(setCondition.begin());

		if (curPosition.checkAllPath()) {
			while (curPosition != gameCondition) {
				auto pair = visited[curPosition];
				target += pair.second;
				curPosition = pair.first;
			}

			std::reverse(target.begin(), target.end());
			return true;
		}

		if (curPosition.checkWayUp()) {
			improveRes(visited, setCondition, curPosition, curPosition.runUp(), 'D');
		}
		if (curPosition.checkWayDown()) {
			improveRes(visited, setCondition, curPosition, curPosition.runDown(), 'U');
		}
		if (curPosition.checkWayLeft()) {
			improveRes(visited, setCondition, curPosition, curPosition.runLeft(), 'R');
		}
		if (curPosition.checkWayRight()) {
			improveRes(visited, setCondition, curPosition, curPosition.runRight(), 'L');
		}
	} while (!setCondition.empty());

	return false;
}

void MyPlayCondition::findOptimumPath() {
	optimumPath = 0;

	for (size_t i = 0; i < DESK_SIZE; i++) {
		if (places[i] != 0) {
			if (places[i] < i + 1) {
				optimumPath += i + 1 - places[i];
			} else {
				optimumPath += places[i] - i - 1;
			}
		}
	}
}

MyPlayCondition MyPlayCondition::runUp() const {
	assert(checkWayUp());

	MyPlayCondition updateCondition(*this);
	updateCondition.nullState -= 4;

	std::swap(updateCondition.places[nullState], updateCondition.places[updateCondition.nullState]);
	updateCondition.findOptimumPath();

	return updateCondition;
}

MyPlayCondition MyPlayCondition::runDown() const {
	assert(checkWayDown());

	MyPlayCondition updateCondition(*this);
	updateCondition.nullState += 4;

	std::swap(updateCondition.places[nullState], updateCondition.places[updateCondition.nullState]);
	updateCondition.findOptimumPath();

	return updateCondition;
}

MyPlayCondition MyPlayCondition::runLeft() const {
	assert(checkWayLeft());

	MyPlayCondition updateCondition(*this);
	updateCondition.nullState--;

	std::swap(updateCondition.places[nullState], updateCondition.places[updateCondition.nullState]);
	updateCondition.findOptimumPath();

	return updateCondition;
}

MyPlayCondition MyPlayCondition::runRight() const {
	assert(checkWayRight());

	MyPlayCondition updateCondition(*this);
	updateCondition.nullState++;

	std::swap(updateCondition.places[nullState], updateCondition.places[updateCondition.nullState]);
	updateCondition.findOptimumPath();

	return updateCondition;
}

bool MyPlayCondition::checkAllPath() const {
	if (nullState != DESK_SIZE - 1) return false;

	for (size_t i = 0; i < DESK_SIZE - 1; i++) {
		if (places[i] != i + 1) return false;
	}

	return true;
}

size_t MyPlayCondition::Hasher::operator()(const MyPlayCondition &game_state) const {
	size_t i = DESK_SIZE - 1;
	size_t factor = 1;
	size_t res = 0;

	do {
		res += game_state.places[i] * factor;
		factor *= DESK_SIZE;
	} while (i-- > 0);

	return res;
}

bool MyPlayCondition::Comparator::operator()(const MyPlayCondition &firstCondition, const MyPlayCondition &secondCondition) const {
	if (firstCondition.optimumPath == secondCondition.optimumPath) {
		return firstCondition.closestPath < secondCondition.closestPath;
	} else {
		return firstCondition.optimumPath < secondCondition.optimumPath;
	}
}

void run(std::istream &input, std::ostream &output) {
	MyPlayCondition defaultPosition;
	unsigned short curAmount;

	for (size_t i = 0; i < DESK_SIZE; ++i) {
		input >> curAmount;
		defaultPosition.places[i] = curAmount;

		if (curAmount == 0) {
			defaultPosition.nullState = i;
		}
	}

	std::string target;
	if (searchResult(defaultPosition, target)) {
		output << target.length() << std::endl << "UUU";
	} else {
		output << -1;
	}
}

int main() {
	run(std::cin, std::cout);

	return 0;
}

