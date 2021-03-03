package objectstructures;

import java.util.ArrayList;
import java.util.List;

public class Person {
	private String name;
	private char gender;
	private Person mother;
	private Person father;
	private List<Person> children = new ArrayList<>();
	
	public Person(String name, char gender) {
		this.name = name;
		this.gender = gender;
	}
	
	public String getName() {
		return name;
	}
	
	public char getGender() {
		return gender;
	}
	
	public Person getMother() {
		return mother;
	}
	
	public Person getFather() {
		return father;
	}
	
	public int getChildCount() {
		return children.size();
	}
	
	public Person getChild(int n) {
		if (!(n >= 0 && n < getChildCount()))
			throw new IllegalArgumentException("No child at given index");
		
		return children.get(n);
	}
	
	// from children perspective
	public void addChild(Person child) {
		if (!children.contains(child)) {
			children.add(child);
			if (getGender() == 'F') 
				child.setMother(this);
			else
				child.setFather(this);
		}
	}
		
	public void removeChild(Person child) {
		if (children.contains(child)) {
			if (getGender() == 'F')
				child.mother = null;
			else
				child.father = null;
			
			children.remove(child);
		}
	}
	
	// from mother/father perspective
	public void setMother(Person mother) {
		if (!isCorrectGender('F', mother))
			throw new IllegalArgumentException("Mother cant be male");
		if (this == mother)
			throw new IllegalArgumentException("Cant be your own mother");
		
		if (this.mother == null) {
			this.mother = mother;
			
			if (mother != null)
				mother.addChild(this);	
		}
		else if (this.mother != mother) {
			this.mother.removeChild(this);
			this.mother = mother;
			
			if (mother != null)
				mother.addChild(this);
		}
	}
	
	public void setFather(Person father) {
		if (!isCorrectGender('M', father))
			throw new IllegalArgumentException("Father cant be a female");
		if (this == father)
			throw new IllegalArgumentException("Cant be your own father");
		
		if (this.father == null) {
			this.father = father;
			
			if (father != null)
				father.addChild(this);	
		}
		else if (this.father != father) {
			this.father.removeChild(this);
			this.father = father;
			
			if (father != null)
				father.addChild(this);
		}
	}
	
	private boolean isCorrectGender(char gender, Person person) {
		if (gender == person.getGender())
			return true;
		return false;
	}
	
	@Override
	public String toString() {
		if (getFather() == null && getMother() == null) 
			return String.format("Person: %s har barn: %s", getName(), children);
		
		else if (getFather() == null) {
			return String.format("Person: %s har mor %s og barn: %s",
					getName(), getMother().getName(), children);
		}
		else if (getMother() == null) {
			return String.format("Person: %s har far %s og barn: %s",
					getName(), getFather().getName(), children);
		}
		else {
			return String.format("Person: %s har mor/far %s/%s og barn: %s",
					getName(), getMother().getName(), getFather().getName(), children);
		}
	}

	public static void main(String[] args) {
		Person eps = new Person("Erlend", 'M');
		Person ops = new Person("Øyvind", 'M');
		Person dhp = new Person("Dag", 'M');
		Person ims = new Person("Ingunn", 'F');
		Person EP = new Person("Elizabeth", 'F');
		Person egs = new Person("Besta", 'F');
		
		eps.setFather(dhp);
		eps.setMother(ims);
		ops.setFather(dhp);
		ops.setMother(ims);
		dhp.setMother(EP);
		ims.setMother(egs);
		dhp.setFather(dhp);
		
		System.out.println(eps.getFather());
		System.out.println(eps.getMother());
		System.out.println(dhp.getChild(0).getName());
		System.out.println(ims.getChild(0).getName());
//		System.out.println(ops);
//		System.out.println(ims);
//		System.out.println(dhp);
//		System.out.println(EP);
//		System.out.println(egs);
		
	}

}
