package encapsulation;

public class Account {
	private double balance;
	private double interestRate;
	
	
	public Account(double balance, double interestRate) {
		checkIfNegative(balance, false);
		checkIfNegative(interestRate, false);
		this.balance = balance;
		this.interestRate = interestRate;
	}
	
	
	private void checkIfNegative(double number, boolean checkWithdraw) {
		if (number < 0) {
			if (checkWithdraw) {
				throw new IllegalArgumentException("You are withdrawing more money than your balance.");
			}
			else {
				throw new IllegalArgumentException("You cannot enter negative values.");
			}
		}
	}
	
	public double getBalance() {
		return balance;
	}
	
	public double getInterestRate() {
		return interestRate;
	}
	
	public void setInterestRate(double interestRate) {
		checkIfNegative(interestRate, false);
		this.interestRate = interestRate;
	}
	
	public void deposit(double amount) {
		checkIfNegative(amount, false);
		balance += amount;
	}
	
	public void withdraw(double amount) {
		checkIfNegative(amount, false);
		checkIfNegative(balance - amount, true);
		balance -= amount;
	}
	
	public void addInterest() {
		double interest = balance * (interestRate/100);
		balance += interest;
	}
	
	@Override
	public String toString() {
		return String.format("Balance: %s | Interest Rate: %s", balance, interestRate);
	}
	
	public static void main(String[] args) {
		Account ac = new Account(10000, 1.8);
		System.out.println(ac.toString());
		ac.setInterestRate(1);
		System.out.println(ac.toString());
		ac.withdraw(1000);
		System.out.println(ac.toString());
		ac.deposit(1000);
		System.out.println(ac.toString());
		ac.addInterest();
		System.out.println(ac.toString());
	}
}
