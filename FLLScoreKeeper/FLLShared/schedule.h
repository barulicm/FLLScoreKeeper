#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include "match.h"

class Schedule
{
public:
    explicit Schedule();

    void advanceMatch();

    void setMatch(int match);

    int getCurrentMatch() const;

    const std::vector<std::shared_ptr<Team>>& teamsForMatch(int match) const;

    void addTeamToMatch(std::shared_ptr<Team> team, int match, int table);

    const std::vector<std::shared_ptr<Team>>& getCurrentTeams() const;

    const std::vector<std::shared_ptr<Team> > getNextTeams() const;

    void setTables(std::vector<std::string> names);

    void clear();

private:
    int current_match;
    int num_tables;
    int teams_per_table;
    std::vector<std::string> table_names;
    std::vector<Match> matches;
};

#endif // SCHEDULE_H
