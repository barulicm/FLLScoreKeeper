import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;

import javax.swing.table.AbstractTableModel;


@SuppressWarnings("serial")
public class TeamList extends AbstractTableModel {
	
	private ArrayList<Team> teams;
	private int startingIndex;
	
	public TeamList() {
		teams = new ArrayList<Team>();
		startingIndex = 0;
	}
	
	public void showNextTen() {
		startingIndex += 10;
		if(startingIndex >= teams.size()) 
			startingIndex = 0;
		fireTableDataChanged();
	}
	
	public void addTeam(Team team) {
		teams.add(team);
		update();
	}
	
	public void update() {
		Collections.sort(teams);
		fireTableDataChanged();
		startingIndex = 0;
	}

	@Override
	public int getColumnCount() {
		return 8;
	}

	@Override
	public String getColumnName(int columnIndex) {
		switch(columnIndex) {
		case 0:
			return "Ranking";
		case 1:
			return "Team #";
		case 2:
			return "Team Name";
		case 3:
			return "Round 1";
		case 4:
			return "Round 2";
		case 5:
			return "Round 3";
		case 6:
			return "Round 4";
		case 7:
			return "Highest";
		}
		return null;
	}

	@Override
	public int getRowCount() {
		return 10;
	}

	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		int index = rowIndex + startingIndex;
//		if(columnIndex == 0);
//			System.out.println("Value queried at " + index);
		if(index >= teams.size())
			return null;
		Team team = teams.get(index);
		switch (columnIndex) {
		case 0:
			return index + 1;
		case 1:
			return team.number;
		case 2:
			return team.name;
		case 3:
			return team.getScoreForRound(1);
		case 4:
			return team.getScoreForRound(2);
		case 5:
			return team.getScoreForRound(3);
		case 6:
			return team.getScoreForRound(4);
		case 7:
			return team.getHighestScore();
		}
		return null;
	}
	
	public Team getTeamWithNumber(int number) {
		Iterator<Team> iter = teams.iterator();
		while(iter.hasNext()) {
			Team t = iter.next();
			if(t.number == number)
				return t;
		}
		return null;
	}
	
	public Team[] toArray() {
		Team[] arr = new Team[teams.size()];
		teams.toArray(arr);
		return arr;
	}
	
}