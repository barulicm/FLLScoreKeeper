#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include <map>
#include <memory>
#include "team.h"

class Match
{
public:
    void addTeam(std::shared_ptr<Team> team, int table);

    const std::vector<std::shared_ptr<Team>>& getTeams() const;

    const std::vector<std::shared_ptr<Team>>& getTeamsOnTable(int table) const;

private:
    std::vector<std::shared_ptr<Team>> teams;
    std::map<int, std::vector<std::shared_ptr<Team>>> table_assignments;
};

#endif // MATCH_H
