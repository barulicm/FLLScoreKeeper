import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTable;

//TODO !!! Needs to ask user for what IP / Port to connect to on startup.

@SuppressWarnings("serial")
public class FLLDisplay extends JFrame {
	
	JLabel timerDisplay;
	JTable scoreDisplay;
	JTable upNextDisplay;
	
	TeamList teams;
	TeamScheduler schedule;
	DisplayNetworkManager network;
	
	public FLLDisplay () {
		setTitle("FLL Display");
		setSize(1200,800);
		setLocation(0,0);
		setExtendedState(MAXIMIZED_BOTH);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		setIconImage(new ImageIcon(getClass().getResource("/images/FLL-logo.png")).getImage());
		
		final JFrame window = this;
		addComponentListener(new ComponentAdapter() {
			public void componentResized(ComponentEvent e) {
				timerDisplay.setPreferredSize(new Dimension(window.getContentPane().getSize().width, window.getContentPane().getSize().height / 4));
				upNextDisplay.setPreferredSize(new Dimension(window.getContentPane().getSize().width, window.getContentPane().getSize().height / 4));
				if(upNextDisplay.getRowCount() > 0)
					upNextDisplay.setRowHeight(Math.max(10,upNextDisplay.getSize().height / upNextDisplay.getRowCount() + 1));
				scoreDisplay.setPreferredSize(new Dimension(window.getContentPane().getSize().width, window.getContentPane().getSize().height / 2));
				if(scoreDisplay.getRowCount() > 0)
					scoreDisplay.setRowHeight(Math.max(10,scoreDisplay.getSize().height / 13));
			}
		});
		
		teams = new TeamList();
		schedule = new TeamScheduler();
		
		timerDisplay = new JLabel("00:00", JLabel.CENTER);
		timerDisplay.setBackground(Color.YELLOW);
		timerDisplay.setFont(new Font("Serif", Font.PLAIN, 200));
		timerDisplay.setPreferredSize(new Dimension(this.getSize().width, this.getSize().height / 4));
		timerDisplay.setOpaque(true);
		
		add(timerDisplay, BorderLayout.PAGE_START);
		
		scoreDisplay = new JTable(teams);
		scoreDisplay.setPreferredSize(new Dimension(this.getSize().width, this.getSize().height / 2));
		scoreDisplay.setBackground(Color.WHITE);
		scoreDisplay.setGridColor(Color.BLACK);
		scoreDisplay.setRowMargin(0);
		scoreDisplay.setFont(new Font("Serif", Font.PLAIN, 32));
		
		add(scoreDisplay, BorderLayout.CENTER);
		
		upNextDisplay = new JTable(schedule);
		upNextDisplay.setPreferredSize(new Dimension(this.getSize().width, this.getSize().height / 4));
		upNextDisplay.setBackground(Color.LIGHT_GRAY);
		upNextDisplay.setGridColor(Color.BLACK);
		upNextDisplay.setRowMargin(0);
		upNextDisplay.setFont(new Font("Serif", Font.PLAIN, 32));
		add(upNextDisplay, BorderLayout.PAGE_END);
		
		connectToControl();
	}
	
	public void connectToControl() {
		String IP = JOptionPane.showInputDialog(this, "Please enter the host address:", "localHost");
		
		if(IP == null) {
			System.exit(0);
			return;
		}

		int port = getPortNumber();

		try {
			network = new DisplayNetworkManager(IP, port, this);
		} catch (Exception e) {
			JOptionPane.showMessageDialog(this, "Could not connect to given host/port.");
			System.out.println("ERROR: Could not connect to given host/port.");
			e.printStackTrace();
			connectToControl();
		}
	}
	
	private int getPortNumber() {
		String portInput = JOptionPane.showInputDialog(this, "Please enter the port number:", "9999");
		try {
			return Integer.parseInt(portInput);
		} catch (NumberFormatException e) {
			JOptionPane.showMessageDialog(this, "Please enter a valid port number.");
			return getPortNumber();
		}
	}
	
	public void receivedCommandFromControlApp(String command) {
		String[] tokens = command.split("=");
		if(tokens[0].equals("nextTenScores")) {
			teams.showNextTen();
		}
		else if(tokens[0].equals("timer")) {
			if(tokens[1].equals("start")) {
				timerDisplay.setBackground(Color.GREEN);
			}
			else if(tokens[1].equals("stop")) {
				timerDisplay.setBackground(Color.RED);
			}
			else {
				timerDisplay.setText(tokens[1]);
			}
		}
		else if(tokens[0].equals("round")) {
			if(tokens[1].equals("next"))
				schedule.nextRound();
			else
				schedule.setRound(Integer.parseInt(tokens[1]));
		}
		else if(tokens[0].equals("setScore")) {
			String[] args = tokens[1].split(",");
			int teamNumber = Integer.parseInt(args[0]);
			int round = Integer.parseInt(args[1]);
			int score = Integer.parseInt(args[2]);
			Team t = teams.getTeamWithNumber(teamNumber);
			t.setScore(round, score);
			teams.update();
		}
		else if(tokens[0].equals("addTeam")) {
			String[] args = tokens[1].split(",");
			int teamNumber = Integer.parseInt(args[0]);
			String teamName = args[1];
			Team t = new Team();
			t.name = teamName;
			t.number = teamNumber;
			teams.addTeam(t);
		}
		else if(tokens[0].equals("assignMatch")) {
			String[] args = tokens[1].split(",");
			int matchNumber = Integer.parseInt(args[0]);
			int teamNumber = Integer.parseInt(args[1]);
			int tableNumber = Integer.parseInt(args[2]);
			Team t = teams.getTeamWithNumber(teamNumber);
			schedule.addTeamToRound(t, matchNumber, tableNumber);
		}
		else if(tokens[0].equals("setTables")) {
			String[] args = tokens[1].split(",");
			schedule.setTables(args);
		}
		else if(tokens[0].equals("setTPT")) {
			int tpt = Integer.parseInt(tokens[1]);
			schedule.teamsPerTable = tpt;
		}
		else if(tokens[0].equals("clearSched")) {
			teams.clear();
			schedule.clear();
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		JFrame window = new FLLDisplay();
		window.setVisible(true);
	}

}
