package encapsulation;

import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;

public class Person {
	private String name;
	private String eMail;
	private Date birthDate;
	private char gender;
	private List<String> countryCodes = new ArrayList<String>(Arrays.asList( 
			"ad", "ae", "af", "ag", "ai", "al", "am", "ao", "aq", "ar", "as", "at", "au", "aw", "ax", "az",
			"ba", "bb", "bd", "be", "bf", "bg", "bh", "bi", "bj", "bl", "bm", "bn", "bo", "bq", "br", "bs",
			"bt", "bv", "bw", "by", "bz", "ca", "cc", "cd", "cf", "cg", "ch", "ci", "ck", "cl", "cm", "cn",
			"co", "cr", "cu", "cv", "cw", "cx", "cy", "cz", "de", "dj", "dk", "dm", "do", "dz", "ec", "ee",
			"eg", "eh", "er", "es", "et", "fi", "fj", "fk", "fm", "fo", "fr", "ga", "gb", "gd", "ge", "gf",
			"gg", "gh", "gi", "gl", "gm", "gn", "gp", "gq", "gr", "gs", "gt", "gu", "gw", "gy", "hk", "hm",
			"hn", "hr", "ht", "hu", "id", "ie", "il", "im", "in", "io", "iq", "ir", "is", "it", "je", "jm",
			"jo", "jp", "ke", "kg", "kh", "ki", "km", "kn", "kp", "kr", "kw", "ky", "kz", "la", "lb", "lc",
			"li", "lk", "lr", "ls", "lt", "lu", "lv", "ly", "ma", "mc", "md", "me", "mf", "mg", "mh", "mk",
			"ml", "mm", "mn", "mo", "mp", "mq", "mr", "ms", "mt", "mu", "mv", "mw", "mx", "my", "mz", "na",
			"nc", "ne", "nf", "ng", "ni", "nl", "no", "np", "nr", "nu", "nz", "om", "pa", "pe", "pf", "pg",
			"ph", "pk", "pl", "pm", "pn", "pr", "ps", "pt", "pw", "py", "qa", "re", "ro", "rs", "ru", "rw",
			"sa", "sb", "sc", "sd", "se", "sg", "sh", "si", "sj", "sk", "sl", "sm", "sn", "so", "sr", "ss",
			"st", "sv", "sx", "sy", "sz", "tc", "td", "tf", "tg", "th", "tj", "tk", "tl", "tm", "tn", "to",
			"tr", "tt", "tv", "tw", "tz", "ua", "ug", "um", "us", "uy", "uz", "va", "vc", "ve", "vg", "vi",
			"vn", "vu", "wf", "ws", "ye", "yt", "za", "zm", "zw"));
	
	public String getName() {
		return name;
	}
	
	public String getEmail() {
		return eMail;
	}
	
	public Date getBirthday() {
		return birthDate;
	}
	
	public char getGender() {
		return gender;
	}
	
	public void setName(String name) {
		validateName(name);
		this.name = name;
	}
	
	public void setEmail(String eMail) {
		if (eMail == null) {
			this.eMail = null;
		}
		else {
			validateEmail(eMail);
			this.eMail = eMail;
		}
	}
	
	public void setBirthday(Date date) {
		validateDate(date);
		birthDate = date;
	}
	
	public void setGender(char gender) {
		validateGender(gender);
		this.gender = gender;
	}
	
	private boolean isAlpha(String s) {
		return s.matches("[a-zA-ZæøåÆØÅ]+");
	}
	
	private void validateName(String name) {
		String[] nameArr = name.split(" ");
		if (nameArr.length == 2) {
			for (int i = 0; i < 2; i++) {
				if (!(nameArr[i].length() >= 2 && isAlpha(nameArr[i]))) {
					throw new IllegalArgumentException("First name and last name must be of length 2 or more and only contain letters.");
				}
			}
		}
		else {
			throw new IllegalArgumentException("You can only have first name and last name, no middle name.");
		}
	}
	
	private void validateEmail(String eMail) {
		String[] emailArr = eMail.split("\\W");
		String[] nameArr = getName().split(" ");
		// first name and last name
		if (emailArr.length == 4) {
			if (!(emailArr[0].equals(nameArr[0].toLowerCase()) && emailArr[1].equals(emailArr[1].toLowerCase()))) {
				throw new IllegalArgumentException("Must be on the form: firstname.lastname@domain.countrycode, i.e all in lowercase");
			}
			else if (!countryCodes.contains(emailArr[3])) {
				throw new IllegalArgumentException("Illegal country code.");
			}
		}
		else {
			throw new IllegalArgumentException("Must be on the form: firstname.lastname@domain.countrycode");
		}
		
	}
	
	private void validateDate(Date date) {
		Date dateToday = new Date();
		if (date.after(dateToday)) {
			throw new IllegalArgumentException("Date can't be in the future.");
		}
	}
	
	private void validateGender(char gender) {
		char[] allowedGenders = new char[] {'M', 'F', '\0'};
		boolean contains = false;
		for (char i: allowedGenders) {
			if (i == gender) {
				contains = true;
			}
		}
		if (!contains) {
			throw new IllegalArgumentException("Invalid gender.");
		}
	}
	

	public static void main(String[] args) {
		Person p1 = new Person();
		p1.setName("Ola Nordmann");
	}

}
