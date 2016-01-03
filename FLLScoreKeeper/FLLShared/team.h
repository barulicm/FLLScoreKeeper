#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <ostream>

class Team
{
public:
    int getScoreForRound(int round) const;

    int getHighestScore() const;

    void setScore(int round, int score);

    bool operator<(const Team& other) const;

    std::string to_string() const;

    std::string getName() const;

    void setName(std::string name);

    int getNumber() const;

    void setNumber(int number);

    friend std::ostream& operator<< (std::ostream& stream, const Team& team);

private:
    std::string name;
    int number = 0;
    int score1 = 0;
    int score2 = 0;
    int score3 = 0;
    int high = 0;

};

#endif // TEAM_H
