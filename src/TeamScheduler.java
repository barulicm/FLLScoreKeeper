import java.util.LinkedList;
import java.util.List;

import javax.swing.table.AbstractTableModel;


@SuppressWarnings("serial")
public class TeamScheduler extends AbstractTableModel {
	private int currentRound;
	public int numTables, teamsPerTable;
	public String[] tableNames;
	private List<Match> matches;
	
	public TeamScheduler() {
		currentRound = 0;
		matches = new LinkedList<>();
	}
	
	public void nextRound() {
		currentRound++;
		fireTableDataChanged();
	}
	
	public void setRound(int round) {
		currentRound = round;
		fireTableDataChanged();
	}
	
	public List<Team> teamsForRound(int round) {
		if(round >= matches.size() || round < 0)
			throw new IndexOutOfBoundsException();
		return matches.get(round).getTeams();
	}
	
	public void addTeamToRound(Team team, int round, int table) {
		while(round >= matches.size())
			matches.add(new Match());
		matches.get(round).addTeam(team, table);
		fireTableDataChanged();
	}
	
	public List<Team> getCurrentTeams() {
		return matches.get(currentRound).getTeams();
	}
	
	public List<Team> getNextTeams() {
		return matches.get(currentRound+1).getTeams();
	}

	@Override
	public int getColumnCount() {
		return numTables*2 + 1;
	}

	@Override
	public int getRowCount() {
		return teamsPerTable * 2;
	}

	@Override
	public Object getValueAt(int row, int col) {
		int round  = currentRound + ( row / teamsPerTable );
		int table = ( col - 1 ) / 2;
		if(col == 0) {
			if(row == 0)
				return "Playing:";
			if(row == teamsPerTable)
				return "Next:";
		}
		else if((col-1) % 2 == 0) {
			if(row == 0)
				return tableNames[(col-1)/2];
			if(row == teamsPerTable)
				return tableNames[(col-1)/2];
		}
		else if((col-1) % 2 == 1) {
			if(matches.get(round).getTeamsOnTable(table) != null)
				return matches.get(round).getTeamsOnTable(table).get(row % teamsPerTable).number;
		}
		return null;
	}
	
	public Match[] toArray() {
		Match[] array = new Match[matches.size()];
		matches.toArray(array);
		return array;
	}
	
	public void setTables(String[] tableNames) {
		this.tableNames = tableNames;
		this.numTables = tableNames.length;
		fireTableStructureChanged();
	}

	public void clear() {
		currentRound = 0;
		numTables = 0;
		teamsPerTable = 0;
		tableNames = new String[0];
		matches.clear();
	}
}
