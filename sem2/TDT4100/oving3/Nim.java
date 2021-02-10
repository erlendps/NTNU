package encapsulation;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Nim {
	private List<Integer> piles = new ArrayList<Integer>(Arrays.asList(0,0,0));
	
	public Nim(int pileSize) {
		for (int i = 0; i < piles.size(); i++) {
			piles.set(i, pileSize);
		}
	}
	
	public Nim() {
		this(10);
	}
	
	public boolean isValidMove(int number, int targetPile) {
		if (isGameOver()) {
			return false;
		}
		if (number < 1) {
			return false;
		}
		else if (targetPile < 0 || targetPile > 2) {
			return false;
		}
		else if (number > getPile(targetPile)) {
			return false;
		}
		else {
			return true;
		}
	}
	
	public boolean isGameOver() {
		for (int i: piles) {
			if (i == 0) {
				return true;
			}
		}
		return false;
	}
	
	public int getPile(int targetPile) {
		return piles.get(targetPile);
	}
	
	
	public void removePieces(int number, int targetPile) {
		if (!isValidMove(number, targetPile) && isGameOver()) {
			throw new IllegalStateException("Game is over");
		}
		else if (!isValidMove(number, targetPile)) {
			throw new IllegalArgumentException("Number must be greater og equal to 1, but smaller than the amount in the specified pile.");
		}
		else {
			piles.set(targetPile, getPile(targetPile) - number);
		}
	}
	
	@Override
	public String toString() {
		return String.format("Pile 1: %d Pile 2: %d Pile 3: %d", getPile(0), getPile(1), getPile(2));
	}

	public static void main(String[] args) {
		Nim n1 = new Nim();
	}

}
