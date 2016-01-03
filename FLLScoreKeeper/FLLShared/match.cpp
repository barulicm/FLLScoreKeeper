#include "match.h"

using namespace std;

void Match::addTeam(shared_ptr<Team> team, int table) {
    teams.push_back(team);
    table_assignments[table].push_back(team);
}

const vector<shared_ptr<Team>>& Match::getTeams() const {
    return teams;
}

const vector<shared_ptr<Team>>& Match::getTeamsOnTable(int table) const {
    return table_assignments.at(table);
}
