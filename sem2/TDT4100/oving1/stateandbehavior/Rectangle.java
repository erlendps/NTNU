package stateandbehavior;

public class Rectangle {
	// koordinater
	// øverst til venstre
	int leftX = 0; 
	int leftY = 0;
	
	// nederst til høyre
	int rightX = 0;
	int rightY = 0;
	
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
	
	public boolean contains(Rectangle rect) {
		if (rect.contains(rect.leftX, rect.leftY) && rect.contains(rect.rightX, rect.rightY))
			return true;
		else
			return false;
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
		Rectangle rec1 = new Rectangle();
		System.out.println(rec1.getHeight());
		System.out.println(rec1.getWidth());
		System.out.println(rec1.getMaxX());
		System.out.println(rec1.getMaxY());
		System.out.println(rec1.getMinX());
		System.out.println(rec1.getMinY());
		System.out.println(rec1.add(2,3));
		System.out.println(rec1.toString());
		System.out.println(rec1.contains(2, 3));
		System.out.println(rec1.add(4, 4));
		System.out.println(rec1.toString());
		System.out.println(rec1.add(3,2));
		System.out.println(rec1.toString());
		System.out.println(rec1.add(3,3));
		
		Rectangle rec2 = new Rectangle();
		rec2.add(4,3);
		rec2.add(5,5);
		System.out.println(rec2.toString());
		System.out.println(rec2.add(rec1));
		System.out.println(rec2.toString());
		
		Rectangle rec3 = new Rectangle();
		rec3.add(1,1);
		rec3.add(2,1);
		
		Rectangle rec4 = rec2.union(rec3);
		System.out.println(rec4.toString());
		
	}

}
