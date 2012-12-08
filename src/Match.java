import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


public class Match {
	private List<Team> teams;
	private Map<Integer, List<Team>> tableAssignments;
	
	public Match() {
		teams = new LinkedList<Team>();
		tableAssignments = new HashMap<>();
	}
	
	public void addTeam(Team team, Integer table) {
		teams.add(team);
		if(!tableAssignments.containsKey(table))
			tableAssignments.put(table, new LinkedList<Team>());
		tableAssignments.get(table).add(team);
	}
	
	public List<Team> getTeams() {
		return teams;
	}
	
	public List<Team> getTeamsOnTable(int table) {
		return tableAssignments.get(table);
	}
}
