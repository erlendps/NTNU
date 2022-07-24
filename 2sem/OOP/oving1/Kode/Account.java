package stateandbehavior;

public class Account {
	double balance = 0;
	double interestRate = 0;
	
	public void deposit(double amount) {
		if (amount > 0) {
			balance += amount;
		}
	}
	
	public void addInterest() {
		double interest = balance * (interestRate/100);
		balance += interest;
	}
	
	public double getBalance() {
		return balance;
	}
	
	public double getInterestRate() {
		return interestRate;
	}
	
	public void setInterestRate(double rate) {
		interestRate = rate;
	}
	
	@Override
	public String toString() {
		return String.format("Balance: %s | Interest Rate: %s", balance, interestRate);
	}
	
	
	public static void main(String[] args) {
		Account myAccount = new Account();
		myAccount.deposit(10000);
		System.out.println(myAccount.toString());
		myAccount.setInterestRate(2.5);
		System.out.println(myAccount.toString());
		myAccount.addInterest();
		System.out.println(myAccount.toString());
	}

}
