
public class Team implements Comparable<Team> {
	public String name;
	public int number;
	private int score1, score2, score3, score4, high;
	
	public int getScoreForRound(int round) {
		switch(round) {
		case 1:
			return score1;
		case 2:
			return score2;
		case 3:
			return score3;
		case 4:
			return score4;
		default:
			return 0;
		}
	}
	
	public int getHighestScore() {
		return high;
	}
	
	public void setScore(int round, int score) {
		switch(round) {
		case 1:
			score1 = score;
			break;
		case 2:
			score2 = score;
			break;
		case 3:
			score3 = score;
			break;
		case 4:
			score4 = score;
			break;
		default:
			return;
		}
		high = Math.max(high, score);
	}

	@Override
	public int compareTo(Team other) {
		if(other.getHighestScore() < this.getHighestScore()) {
			return -1;
		} else if (other.getHighestScore() > this.getHighestScore()){
			return 1;
		} else {
			return 0;
		}
	}
	
	public String toString() {
		return number +", " + name;
	}
}
