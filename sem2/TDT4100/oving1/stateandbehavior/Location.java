package stateandbehavior;

public class Location {
	double x = 0;
	double y = 0;
	
	public void up() {
		y -= 1;
	}
	
	public void down() {
		y += 1;
	}
	
	public void right() {
		x += 1;
	}
	
	public void left() {
		x -= 1;
	}
	
	public double getX() {
		return x;
	}
	
	public double getY() {
		return y;
	}
	
	@Override
	public String toString() {
		return String.format("x: %s | y: %s", x, y);
	}
	
	
	public static void main(String[] args) {
		Location loc = new Location();
		System.out.println(loc.toString());
		loc.right();
		System.out.println(loc.toString());
		loc.down();
		System.out.println(loc.toString());
		loc.left();
		System.out.println(loc.toString());
		loc.up();
		System.out.println(loc.toString());
	}

}
