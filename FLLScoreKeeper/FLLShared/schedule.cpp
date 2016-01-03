#include "schedule.h"

Schedule::Schedule()
    : current_match(0)
{
}

void Schedule::advanceMatch() {
    current_match++;
}

void Schedule::setMatch(int match) {
    current_match = match;
}

int Schedule::getCurrentMatch() const {
    return current_match;
}

const std::vector<std::shared_ptr<Team>>& Schedule::teamsForMatch(int match) const {
    return matches[match].getTeams();
}

void Schedule::addTeamToMatch(std::shared_ptr<Team> team, int match, int table) {
    while(match >= matches.size())
        matches.push_back(Match());
    matches[match].addTeam(team, table);
}

const std::vector<std::shared_ptr<Team>>& Schedule::getCurrentTeams() const {
    return matches[current_match].getTeams();
}

const std::vector<std::shared_ptr<Team>> Schedule::getNextTeams() const {
    if((current_match+1) >= matches.size()) {
        return std::vector<std::shared_ptr<Team>>();
    }
    return matches[current_match+1].getTeams();
}

void Schedule::setTables(std::vector<std::string> names) {
    table_names = names;
    num_tables = (int)names.size();
}

void Schedule::clear() {
    current_match = 0;
    num_tables = 0;
    teams_per_table = 0;
    table_names.clear();
    matches.clear();
}
