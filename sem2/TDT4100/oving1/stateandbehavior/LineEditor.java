package stateandbehavior;

public class LineEditor {
	String text = "";
	int insertionIndex = 0;
	
	public void left() {
		if (insertionIndex == 0) {
			insertionIndex = text.length();
		}
		else {
		insertionIndex -= 1;
		}
	}
	
	public void right() {
		if (insertionIndex == text.length()) {
			insertionIndex = 0;
		}
		else {
		insertionIndex += 1;
		}
	}
	
	public void insertString(String s) {
		String toLeft = text.substring(0, insertionIndex);
		String toRight = text.substring(insertionIndex, text.length());
		
		text = toLeft + s + toRight;
		insertionIndex += s.length();
	}
	
	public void deleteLeft() {
		if (insertionIndex != 0) {
			String toLeft = text.substring(0, insertionIndex-1);
			String toRight = text.substring(insertionIndex, text.length());
			
			text = toLeft + toRight;
			insertionIndex -= 1;
		}
	}
	
	public void deleteRight() {
		if (insertionIndex != text.length()) {
			String toLeft = text.substring(0, insertionIndex);
			String toRight = text.substring(insertionIndex+1, text.length());
			
			text = toLeft + toRight;
		}
	}
	
	public String getText() {
		return text;
	}
	
	public void setText(String s) {
		text = s;
		insertionIndex = 0;
	}
	
	public int getInsertionIndex() {
		return insertionIndex;
	}
	
	public void setInsertionIndex(int i) {
		insertionIndex = i;
	}
	
	public String toString() {
		int pos = insertionIndex;
		return text.substring(0, pos) + "|" + text.substring(pos);
	}
	
	public static void main(String[] args) {
		LineEditor str = new LineEditor();
		System.out.println(str.toString());
		str.setText("gaming");
		System.out.println(str.toString());
		str.left();
		System.out.println(str.toString());
		str.insertString("gaming");
		System.out.println(str.toString());
		str.setInsertionIndex(3);
		System.out.println(str.toString());
		str.deleteRight();
		System.out.println(str.toString());
		str.deleteLeft();
		System.out.println(str.toString());
		System.out.println(str.getInsertionIndex());
		str.right();
		System.out.println(str.toString());
		System.out.println(str.getText());
		str.setText("gaming");
		System.out.println(str.toString());
	}

}
