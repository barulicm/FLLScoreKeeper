import java.io.File;
import java.io.FileWriter;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JProgressBar;
import javax.swing.filechooser.FileFilter;


@SuppressWarnings("serial")
public class ScoreExporter extends JFrame {
	
	TeamList teams;
	
	String output;
	
	JProgressBar progressBar;
	
	public ScoreExporter(TeamList teams) {
		this.teams = teams;
		output = "";
		
		setTitle("Exporting Scores...");
		setLocation(0,0);
		setSize(200,100);
		
		progressBar = new JProgressBar();
		progressBar.setOpaque(true);
		add(progressBar);
	}
	
	private void populateOutput() {
		Team[] teamArr = teams.toArray();
		Team team;
		for(int i = 0; i < teamArr.length; i++) {
			team = teamArr[i];
			output += /*Rank*/ (i+1) + ";" + team.number + ";" + team.name + ";" + team.getScoreForRound(1) + ";" + team.getScoreForRound(2) + ";" + team.getScoreForRound(3) + ";" + team.getScoreForRound(4) + System.getProperty("line.separator");
			progressBar.setValue( (int) ( ( (double)i / (double)teamArr.length ) * 90 ) );
		}
	}
	
	private void saveOutput(String path) {
		try {
			FileWriter writer = new FileWriter(path);
			writer.write(output);
			writer.close();
		} catch (Exception e) {
			JOptionPane.showMessageDialog(this, "Error saving file at given location.");
			e.printStackTrace();
		}
	}

	public static void ExportScores(TeamList teams) {
		ScoreExporter se = new ScoreExporter(teams);
		se.setVisible(true);
		se.populateOutput();
		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setFileFilter(new FileFilter() {
			@Override
			public boolean accept(File pathname) {
				return pathname.getPath().endsWith(".sco");
			}

			@Override
			public String getDescription() {
				return "FLL Score File";
			}
		});
		fileChooser.showDialog(se, "Save");
		File file = fileChooser.getSelectedFile();
		if(file != null)
			se.saveOutput(file.getAbsolutePath());
		se.progressBar.setValue(100);
		se.dispose();
	}
}
