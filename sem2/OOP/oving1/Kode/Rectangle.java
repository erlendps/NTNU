package stateandbehavior;

public class Rectangle {
	// koordinater
	// øverst til venstre
	int leftX; 
	int leftY;
	
	// nederst til høyre
	int rightX;
	int rightY;
	
	public boolean add(int x, int y) {
		if (getMinX() == 0) {
			leftX = x;
			rightX = x;
			leftY = y;
			rightY = y;
			return true;
		}
		else {
			if (x < leftX) {
				leftX = x;
				if (y < leftY) {
					leftY = y;
				}
				else if (y > rightY) {
					rightY = y;
				}
				return true;
			}
			else if (x > rightX) {
				rightX = x;
				if (y < leftY) {
					leftY = y;
				}
				else if (y > rightY) {
					rightY = y;
				}
				return true;
			}
			else if (y < leftY) {
				leftY = y;
				if (x < leftX) {
					leftX = x;
				}
				else if (x > rightX) {
					rightX = x;
				}
				return true;
			}
			else if (y > rightY) {
				rightY = y;
				if (x < leftX) {
					leftX = x;
				}
				else if (x > rightX) {
					rightX = x;
				}
				return true;
			}
			else {
				return false;
			}
			
		}
		
	}
	
	public boolean add(Rectangle rect) {
		if (add(rect.leftX, rect.leftY)) {
			add(rect.rightX, rect.rightY);
			return true;
		}
		else if (add(rect.rightX, rect.rightY)) {
			return true;
		}
		else {
			return false;
		}
	}
	
	public int getMinX() {
		return leftX;
	}
	
	public int getMinY() {
		return leftY;
	}
	
	public int getMaxX() {
		return rightX;
	}
	
	public int getMaxY() {
		return rightY;
	}
	
	public int getWidth() {
		if (getMinX() == 0) {
			return 0;
		}
		else {
			return rightX - leftX + 1;
		}
	}
	
	public int getHeight() {
		if (getMinX() == 0) {
			return 0;
		}
		else {
			return rightY - leftY + 1;
		}
	}
	
	public boolean isEmpty() {
		if (getHeight() == 0 || getWidth() == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	
	public boolean contains(int x, int y) {
		if (getHeight() == 0 || getWidth() == 0) {
			return false;
		}
		else if (x >= leftX && x <= rightX) {
			if (y >= leftY && y <= rightY) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
		
	}
	
	public Rectangle union(Rectangle rect) {
		Rectangle uni = new Rectangle();
		uni.add(this);
		uni.add(rect);
		return uni;
	}
	
	
	
	@Override
	public String toString() {
		return "Hjørne 1: (" + leftX + ", " + leftY + ") Hjørne 2: (" + rightX + ", " + rightY + ") | Høyde: " + getHeight() + " Bredde: " + getWidth();
	}
	
	public static void main(String[] args) {
		Rectangle rec = new Rectangle();
		System.out.println(rec.contains(0, 0));
		System.out.println(rec.isEmpty());
		rec.add(2, 3);
		System.out.println(rec);
		rec.add(5, 5);
		System.out.println(rec);
		rec.add(3, 2);
		System.out.println(rec);
		
		Rectangle rect = new Rectangle();
		rect.add(1, 1);
		rect.add(2, 1);
		System.out.println(rec.add(rect));
		System.out.println(rec);

		
		
	}

}
