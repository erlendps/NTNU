package encapsulation;
import java.util.Stack;
import java.util.ArrayList;
import java.util.Arrays;
import java.lang.Math;

public class RPNCalc {
	private Stack<Double> stack = new Stack<Double>();
	
	public void push(double value) {
		stack.push(value);
	}
	
	public double pop() {
		if (stack.empty()) {
			return Double.NaN;
		}
		else {
			double popped = peek(0);
			stack.pop();
			return popped;
		}
	}
	
	public int getSize() {
		return stack.size();
	}
	
	public double peek(int n) {
		if (getSize() <= n || n < 0 || getSize() == 0) {
			return Double.NaN;
		}
		else {
			return stack.get(getSize() - 1 - n);
		}
	}
	
	public void performOperation(char op) {
		double a = 0;
		double b = 0;
		
		if (getSize() >= 2) {
			a = peek(0);
			b = peek(1);
		}
		else if (getSize() == 1) {
			b = peek(0);
			if (op == '+' || op == '-') {
				a = 0;
			}
			else {
				a = 1;
			}
		}
		else if (op == 'p' || op == 'π') {;}
		else {
			throw new IllegalArgumentException("Empty stack!");
		}
		
		ArrayList<Character> op1 = new ArrayList<Character>(Arrays.asList('+', '-', '*', '/'));
		
		if (op1.contains(op)) {
			double c = 0;
			this.pop();
			this.pop();
			
			switch(op) {
				case '+': 
					c = b + a;
					break;
				case '-':
					c = b - a;
					break;
				case '*':
					c = b * a;
					break;
				case '/':
					c = b / a;
					break;
			}
			this.push(c);
		}
		
		else if (op == '~' && getSize() >= 2) {
			this.pop();
			this.pop();
			this.push(a);
			this.push(b);
		}
		else if (op == '~') {;}
		else if (op == 'p' || op == 'π') {
			this.push(Math.PI);
		}
		else if (op == '|') {
			double c = Math.abs(peek(0));
			this.pop();
			this.push(c);
		}
		else {
			throw new IllegalArgumentException("The operator is not supported");
		}
		
	}
	
	public String toString() {
		return String.format("%s", stack);
	}

	public static void main(String[] args) {
		RPNCalc rpn = new RPNCalc();
		System.out.println(rpn.pop());
		rpn.push(1);
		rpn.push(2);
		rpn.push(3);
		System.out.println(rpn.peek(2));
		System.out.println(rpn.toString());
		rpn.performOperation('-');
		System.out.println(rpn.toString());
		System.out.println(rpn.peek(2));
		System.out.println(rpn.getSize());
		rpn.performOperation('|');
		System.out.println(rpn.toString());
		rpn.performOperation('p');
		System.out.println(rpn.toString());
		System.out.println(rpn.pop());
		System.out.println(rpn.toString());
	}

}
