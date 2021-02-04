package encapsulation;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.*;

public class Vehicle {
	
	private char vehicleType;
	private char fuelType;
	private String registrationNumber;
	private List<Character> fuelTypes = new ArrayList<Character>(Arrays.asList('H', 'E', 'D', 'G'));
	
	
	public Vehicle(char vehicleType, char fuelType, String registrationNumber) {
		validateVehicleType(vehicleType);
		this.vehicleType = vehicleType;
		
		validateFuelType(fuelType);
		this.fuelType = fuelType;
		
		validateRegistrationNumber(registrationNumber);
		this.registrationNumber = registrationNumber;
	}
	
	private void validateVehicleType(char vehicleType) {
		if (!(vehicleType == 'C' || vehicleType == 'M')) {
			throw new IllegalArgumentException("Vehicle type must be car ('C') or motorcycle ('M').");
		}
	}
	
	private void validateFuelType(char fuelType) {
		if (!fuelTypes.contains(fuelType)) {
			throw new IllegalArgumentException("Invalid fuel type.");
		}
		else if (getVehicleType() == 'M' && fuelType == 'H') {
			throw new IllegalArgumentException("Invalid fuel type.");
		}
	}
	
	private void validateRegistrationNumber(String registrationNumber) {
		if (getFuelType() == 'H') {
			if (getVehicleType() == 'C' && !checkRegistrationNumber("HY[0-9]{5}\\b", registrationNumber)) {
				throw new IllegalArgumentException("Wrong registration number for hydrogen car.");
			}
		}
		else if (getFuelType() == 'E') {
			if (getVehicleType() == 'C' && !checkRegistrationNumber("(EL|EK)[0-9]{5}\\b", registrationNumber)) {
				throw new IllegalArgumentException("Wrong registration number for electric car.");
			}
			else if (getVehicleType() == 'M' && !checkRegistrationNumber("(EL|EK)[0-9]{4}\\b", registrationNumber)) {
				throw new IllegalArgumentException("Wrong registration number for electric motorcycle.");
			}
		}
		else {
			if (getVehicleType() == 'C' && !checkRegistrationNumber("(?!EK|EL|HY)\\b[A-Z]{2}[0-9]{5}\\b", registrationNumber)) {
				throw new IllegalArgumentException("Wrong registration number for diesel/gas car.");
			}
			else if (getVehicleType() == 'M' && !checkRegistrationNumber("(?!EK|EL|HY)\\b[A-Z]{2}[0-9]{4}\\b", registrationNumber)) {
				throw new IllegalArgumentException("Wrong registration number for diesel/gas motorcycle.");
			}
		}
	}
	
	private boolean checkRegistrationNumber(String pattern, String matcher) {
		Pattern pat = Pattern.compile(pattern);
		Matcher match = pat.matcher(matcher);
		return match.find();
	}
	
	public char getVehicleType() {
		return vehicleType;
	}
	
	public char getFuelType() {
		return fuelType;
	}
	
	public String getRegistrationNumber() {
		return registrationNumber;
	}
	
	public void setRegistrationNumber(String registrationNumber) {
		validateRegistrationNumber(registrationNumber);
		this.registrationNumber = registrationNumber;
	}
	
	@Override
	public String toString() {
		return String.format("Type: %s | Fuel: %s | Reg: %s", vehicleType, fuelType, registrationNumber);
	}
	
	public static void main(String[] args) {
		Vehicle v1 = new Vehicle('C', 'H', "HY43012");
		System.out.println(v1.toString());
		Vehicle vehicle = new Vehicle('C', 'D', "BN12345");
		vehicle.setRegistrationNumber("AB54321");
	}

}
