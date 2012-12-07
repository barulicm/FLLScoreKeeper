import java.io.*;

public class TeamLoader {
	public static void LoadTeams(String path, TeamList list, TeamScheduler schedule) {
		try {
			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(path)));
			String line = reader.readLine();
			while(!line.startsWith("Number of Teams")) {
				line = reader.readLine();
			}
			// Populate Team List
			int numberOfTeams = Integer.parseInt(line.split(",")[1]);
			for(int i = 0; i < numberOfTeams; i++) {
				line = reader.readLine();
				String[] tokens = line.split(",");
				Team team = new Team();
				team.number = Integer.parseInt(tokens[0]);
				team.name = tokens[1];
				list.addTeam(team);
			}
			
			// Populate Scheduler
			while(!(line = reader.readLine()).startsWith("Number of Ranking Matches"));
			int numMatches = Integer.parseInt(line.split(",")[1]);
			while(!(line = reader.readLine()).startsWith("Number of Tables"));
			int numTables = Integer.parseInt(line.split(",")[1]);
			schedule.numTables = numTables;
			while(!(line = reader.readLine()).startsWith("Number of Teams Per Table"));
			int teamsPerTable = Integer.parseInt(line.split(",")[1]);
			schedule.teamsPerTable = teamsPerTable;
			String[] tableNames = new String[numTables];
			while(!(line = reader.readLine()).startsWith("Table Names"));
			String[] tokens = line.split(",");
			for(int i = 1; i <= numTables; i++)
				tableNames[i-1] = tokens[i];
			schedule.tableNames = tableNames;
			for(int i = 0; i < numMatches; i++) {
				line = reader.readLine();
				tokens = line.split(",");
				for(int j=3; j < tokens.length; j++) {
					if(!tokens[j].equals("")) {
						int number = Integer.parseInt(tokens[j]);
						schedule.addTeamToRound(list.getTeamWithNumber(number), i, (j-3)/teamsPerTable);
					}
				}
			}
			
			reader.close();
		} catch (IOException e) {
			System.out.println("Error reading schedule file");
			e.printStackTrace();
		}
	}
}
