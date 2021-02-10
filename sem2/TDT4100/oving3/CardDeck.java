package encapsulation;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class CardDeck {
	private List<Card> deck = new ArrayList<Card>();
	
	public CardDeck(int n) {
		char[] suits = new char[] {'S', 'H', 'D', 'C'};
		if (isValidNumber(n, false)) {
			for (char c: suits) {
				for (int i = 1; i <= n; i++) {
					deck.add(new Card(c, i));
				}
			}
		}
		else {
			throw new IllegalArgumentException("Invalid number (1 <= n <= 13)");
		}
	}
	
	public int getCardCount() {
		return deck.size();
	}
	
	public Card getCard(int n) {
		if (isValidNumber(n, true)) {
			return deck.get(n);
		}
		else {
			throw new IllegalArgumentException("Invalid number (0 <= n <= 12)");
		}
	}
	
	public void shufflePerfectly() {
		List<Card> firstHalf = new ArrayList<Card>(deck.subList(0, getCardCount()/2));
		List<Card> secondHalf = new ArrayList<Card>(deck.subList(getCardCount()/2, getCardCount()));
		int index = 0;
		
		for (int i = 0; i < getCardCount(); i += 2) {
			deck.set(i, firstHalf.get(index));
			deck.set(i+1, secondHalf.get(index));
			index++;
		}
	}
	
	private boolean isValidNumber(int n, boolean checkForZero) {
		List<Integer> validNumbers = new ArrayList<Integer>(Arrays.asList(0,1,2,3,4,5,6,7,8,9,10,11,12,13));
		if (checkForZero) {
			if (validNumbers.subList(0, validNumbers.size()-1).contains(n)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (validNumbers.subList(1, validNumbers.size()).contains(n)) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	

	public static void main(String[] args) {
		CardDeck cards = new CardDeck(2);
		System.out.println(cards.getCardCount());
		System.out.println(cards.getCard(0).toString());
		cards.shufflePerfectly();
	}

}
