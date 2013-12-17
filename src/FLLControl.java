import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.filechooser.FileFilter;


@SuppressWarnings("serial")
public class FLLControl extends JFrame implements ActionListener {
	
	JButton timerStartBtn, timerStopBtn, loadSchedBtn, setRoundBtn, enterScoreBtn, exportScoresBtn, viewConnsBtn, setPortBtn;
	JTextField durationText;
	ControlNetworkManager network;
	TeamList teams;
	TeamScheduler schedule;
	Thread scoreIterator;
	long timerStartTime, timerDuration;
	boolean timerIsRunning;
	int currentRound;
	Color defaultButtonColor;

	public FLLControl() {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (Exception e) {
			e.printStackTrace();
		}
		setTitle("FLL Control");
		setLocation(50,50);
		setSize(250,400);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setLayout(new GridLayout(13,1));
		setResizable(false);

		setIconImage(new ImageIcon(getClass().getResource("/images/FLL-logo.png")).getImage());
		
		JLabel timerLabel = new JLabel("TIMER");
		add(timerLabel);
		
		timerStartBtn = new JButton("Start");
		timerStartBtn.addActionListener(this);
		add(timerStartBtn);
		
		timerStopBtn = new JButton("Stop");
		timerStopBtn.addActionListener(this);
		add(timerStopBtn);
		
		durationText = new JTextField("2:30");
		durationText.addActionListener(this);
		add(durationText);
		
		setTimerDuration(durationText.getText());
		
		JLabel schedLabel = new JLabel("SCHEDULE");
		add(schedLabel);
		
		loadSchedBtn = new JButton("Load");
		loadSchedBtn.addActionListener(this);
		defaultButtonColor = loadSchedBtn.getBackground();
		loadSchedBtn.setBackground(Color.RED);
		add(loadSchedBtn);
		
		setRoundBtn = new JButton("Set round");
		setRoundBtn.addActionListener(this);
		setRoundBtn.setEnabled(false);
		add(setRoundBtn);
		
		JLabel scoresLabel = new JLabel("SCORING");
		add(scoresLabel);
		
		enterScoreBtn = new JButton("Enter score");
		enterScoreBtn.addActionListener(this);
		enterScoreBtn.setEnabled(false);
		add(enterScoreBtn);
		
		exportScoresBtn = new JButton("Export scores");
		exportScoresBtn.addActionListener(this);
		exportScoresBtn.setEnabled(false);
		add(exportScoresBtn);
		
		JLabel connectLabel = new JLabel("CONNECTIONS");
		add(connectLabel);
		
//		viewConnsBtn = new JButton("View connections");
//		viewConnsBtn.addActionListener(this);
//		add(viewConnsBtn);
		
		setPortBtn = new JButton("Set port number");
		setPortBtn.addActionListener(this);
		setPortBtn.setEnabled(false);
		add(setPortBtn);
		
		teams = new TeamList();
		schedule = new TeamScheduler();
//		TeamLoader.LoadTeams("teamforMatchImport.csv", teams, schedule);
		
		network = new ControlNetworkManager(this, 9999);
		
		currentRound = 0;
		
		scoreIterator = new Thread(new Runnable() {
			@Override
			public void run() {
				long lastHit = System.currentTimeMillis();
				while(true) {
					if(System.currentTimeMillis() - lastHit >= 2000) {
						network.sendCommand("nextTenScores");
						lastHit = System.currentTimeMillis();
					}
					if(timerIsRunning) {
						long timerRemaining = timerDuration - (System.currentTimeMillis() - timerStartTime);
						int minutes = (int) (timerRemaining / 60000);
						int seconds = (int) ( ( timerRemaining - (minutes*60000) ) / 1000);
						if(timerRemaining <= 0) {
							network.sendCommand("timer=stop");
							network.sendCommand("timer="+durationText.getText());
							timerIsRunning = false;
							currentRound++;
							network.sendCommand("round="+currentRound);
						}
						network.sendCommand("timer="+minutes+":"+String.format("%02d", seconds));
					}
				}
			}
		});
		scoreIterator.start();
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		JFrame window = new FLLControl();
		window.setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource() == durationText) {
			if(e.getID() == 1001) {
				if(!durationText.getText().matches("[0-9]+:[0-9][0-9]")) {
					durationText.setText("2:30");
					JOptionPane.showMessageDialog(this, "Incorrect time formatting. Should be MM:SS or M:SS .");
				}
				setTimerDuration(durationText.getText());
			}
		}
		if(e.getSource() == timerStartBtn) {
			network.sendCommand("timer=start");
			timerStartTime = System.currentTimeMillis();
			timerIsRunning = true;
		}
		if(e.getSource() == timerStopBtn) {
			timerIsRunning = false;
			network.sendCommand("timer=stop");
			network.sendCommand("timer="+durationText.getText());
		}
		if(e.getSource() == loadSchedBtn) {
			JFileChooser fileChooser = new JFileChooser();
			fileChooser.setFileFilter(new FileFilter() {
				@Override
				public boolean accept(File pathname) {
					
					return pathname.isDirectory() || pathname.getPath().endsWith(".csv");
				}

				@Override
				public String getDescription() {
					return "Comma Seperated Values (*.csv)";
				}
			});
			fileChooser.showDialog(this, "Open");
			File file = fileChooser.getSelectedFile();
			
			if(file != null) {
				String path = file.getPath();
				teams = new TeamList();
				TeamLoader.LoadTeams(path, teams, schedule);
				network.sendTeamAndScheduleInfo();
				
				setRoundBtn.setEnabled(true);
				enterScoreBtn.setEnabled(true);
				exportScoresBtn.setEnabled(true);
//				viewConnsBtn.setEnabled(true);
				setPortBtn.setEnabled(true);
				
				loadSchedBtn.setBackground(defaultButtonColor);
			}
		}
		if(e.getSource() == setRoundBtn) {
			String input = JOptionPane.showInputDialog(this, "Round number:", currentRound);
			if(input != null) {
				try {
				currentRound = Integer.parseInt(input);
				} catch(NumberFormatException nfi) {
					JOptionPane.showMessageDialog(this, "Please enter an integer round number.");
					actionPerformed(e);
				}
				network.sendCommand("round="+currentRound);
			}
		}
		if(e.getSource() == enterScoreBtn) {
			ScoreEntryDialog.GO(this, teams);
		}
		if(e.getSource() == exportScoresBtn) {
			ScoreExporter.ExportScores(teams);
		}
		if(e.getSource() == viewConnsBtn) {
			//TODO view connections
			System.out.println("view connections");
		}
		if(e.getSource() == setPortBtn) {
			String input = JOptionPane.showInputDialog(this, "Enter new port number:",network.getPort());
			if(input != null) {
				int newPort = network.getPort();
				try {
					newPort = Integer.parseInt(input);
				} catch(NumberFormatException nfe) {
					JOptionPane.showMessageDialog(this, "Please enter an integer port number.");
					actionPerformed(e);
				}
				network.setPort(newPort);
			}
		}
	}
	
	private void setTimerDuration(String time) {
		String[] tokens = time.split(":");
		int minutes = Integer.parseInt(tokens[0]);
		int seconds = Integer.parseInt(tokens[1]);
		seconds += minutes * 60;
		timerDuration = seconds * 1000;
	}
}
