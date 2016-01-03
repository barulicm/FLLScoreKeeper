#include "team.h"
#include <algorithm>

using namespace std;

int Team::getHighestScore() const {
    return high;
}

string Team::getName() const {
    return name;
}

int Team::getNumber() const {
    return number;
}

int Team::getScoreForRound(int round) const {
    switch(round) {
    case 1:
        return score1;
        break;
    case 2:
        return score2;
        break;
    case 3:
        return score3;
        break;
    default:
        return 0;
    }
}

void Team::setName(string name) {
    this->name = name;
}

void Team::setNumber(int number) {
    this->number = number;
}

void Team::setScore(int round, int score) {
    switch(round) {
    case 1:
        score1 = score;
        break;
    case 2:
        score2 = score;
        break;
    case 3:
        score3 = score;
        break;
    default:
        return;
    }
    high = std::max(high, score);
}

string Team::to_string() const {
    return std::to_string(number) + " - " + name;
}

bool Team::operator<(const Team& other) const {
    return getHighestScore() < other.getHighestScore();
}

ostream& operator<<(ostream& stream, const Team& team) {
    stream << team.to_string() << endl;
    return stream;
}
