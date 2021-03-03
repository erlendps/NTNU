package objectstructures;

public class Tweet {
	private String text;
	private TwitterAccount owner;
	private int retweetCount = 0;
	private Tweet originalTweet;
	
	public Tweet(TwitterAccount owner, String tweet) {
		this.owner = owner;
		this.text = tweet;
	}
	
	public Tweet(TwitterAccount owner, Tweet tweet) {
		if (tweet.getOwner() == owner)
			throw new IllegalArgumentException("Can't retweet a tweet you own.");
		this.owner = owner;
		this.text = tweet.getText();
		if (tweet.getOriginalTweet() == null) 
			originalTweet = tweet;
		else
			originalTweet = tweet.getOriginalTweet();
		
		originalTweet.retweetCount++;
	}
	
	public String getText() {
		return text;
	}
	
	public TwitterAccount getOwner() {
		return owner;
	}
	
	public Tweet getOriginalTweet() {
		return originalTweet;
	}
	
	public int getRetweetCount() {
		return retweetCount;
	}

	public static void main(String[] args) {
		
	}

}
