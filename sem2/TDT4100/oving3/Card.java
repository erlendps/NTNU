package encapsulation;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Card {
	private char suit = '\0';
	private int face = 0;
	
	public Card(char suit, int face) {
		if (isValidCard(suit, face)) {
			this.suit = suit;
			this.face = face;
		}
		else {
			throw new IllegalArgumentException("Invalid card.");
		}
	}
	
	private boolean isValidCard(char suit, int face) {
		List<Character> validSuits = new ArrayList<Character>(Arrays.asList('S', 'H', 'C', 'D'));
		List<Integer> validFaces = new ArrayList<Integer>(Arrays.asList(1,2,3,4,5,6,7,8,9,10,11,12,13));
		
		if (validSuits.contains(suit) && validFaces.contains(face)) {
			return true;
		}
		else {
			return false;
		}
	}

	public char getSuit() {
		return suit;
	}
	
	public int getFace() {
		return face;
	}
	
	@Override
	public String toString() {
		return String.format("%c%d", getSuit(), getFace());
	}
	
	public static void main(String[] args) {
		Card c1 = new Card('H', 12);
		System.out.println(c1.toString());
	}

}
