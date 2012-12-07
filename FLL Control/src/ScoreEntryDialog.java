import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JTextField;


@SuppressWarnings("serial")
public class ScoreEntryDialog extends JFrame implements ActionListener {
	
	JButton okBtn, cancelBtn;
	JTextField scoreText;
	JComboBox<Integer> roundCombo;
	JComboBox<Team> teamCombo;
	FLLControl parent;
	TeamList teamList;

	public ScoreEntryDialog(FLLControl parent, TeamList teams) {
		this.parent = parent;
		this.teamList = teams;
		
		setTitle("Enter score:");
		setSize(600,100);
		setResizable(false);
		setLocation(parent.getLocation().x + getSize().width/2,parent.getLocation().y + getSize().height/2);
		setType(Type.POPUP);
		setDefaultCloseOperation(DISPOSE_ON_CLOSE);
		
		GroupLayout layout = new GroupLayout(getContentPane());
		setLayout(layout);
		layout.setAutoCreateGaps(true);
		layout.setAutoCreateContainerGaps(true);
		
		scoreText = new JTextField("0");

		teamCombo = new JComboBox<Team>(teams.toArray());
		
		Integer[] roundOptions = {1,2,3,4};
		roundCombo = new JComboBox<Integer>(roundOptions);
		
		okBtn = new JButton("OK");
		okBtn.addActionListener(this);
		
		cancelBtn = new JButton("Cancel");
		cancelBtn.addActionListener(this);
		
		layout.setHorizontalGroup(layout.createSequentialGroup()
				.addComponent(scoreText)
				.addComponent(teamCombo)
				.addComponent(roundCombo)
				.addComponent(okBtn)
				.addComponent(cancelBtn));
		layout.setVerticalGroup(layout.createSequentialGroup()
				.addGroup(layout.createParallelGroup()
						.addComponent(scoreText)
						.addComponent(teamCombo)
						.addComponent(roundCombo))
				.addGroup(layout.createParallelGroup()
						.addComponent(okBtn)
						.addComponent(cancelBtn)));
	}
	
	public static void GO(FLLControl parent, TeamList teams) {
		ScoreEntryDialog sed = new ScoreEntryDialog(parent, teams);
		sed.setVisible(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource() == okBtn) {
			Team t = (Team) teamCombo.getSelectedItem();
			int round, score;
			round = (Integer)roundCombo.getSelectedItem();
			try {
				score = Integer.parseInt(scoreText.getText());
			} catch(NumberFormatException nfi) {
				JOptionPane.showMessageDialog(this, "Please eneter a valid score number.");
				return;
			}
			t.setScore(round, score);
			this.parent.network.sendCommand("setScore="+t.number+","+round+","+score);
			teamList.update();
			this.dispose();
		}
		if(e.getSource() == cancelBtn) {
			this.dispose();
		}
	}
}
