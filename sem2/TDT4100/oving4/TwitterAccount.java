package objectstructures;

import java.util.ArrayList;
import java.util.List;

public class TwitterAccount {
	private String userName;
	private List<Tweet> tweets = new ArrayList<>();
	private List<TwitterAccount> following = new ArrayList<>();
	private List<TwitterAccount> followers = new ArrayList<>();
	
	public TwitterAccount(String userName) {
		this.userName = userName;
	}
	
	public String getUserName() {
		return userName;
	}
	
	public void follow(TwitterAccount account) {
		if (!isFollowing(account)) {
			following.add(account);
			account.followers.add(this);
		}
	}
	
	public void unfollow(TwitterAccount account) {
		if (isFollowing(account)) {
			account.followers.remove(this);
			following.remove(account);
		}
	}
	
	public boolean isFollowing(TwitterAccount account) {
		if (following.contains(account))
			return true;
		return false;
	}
	
	public boolean isFollowedBy(TwitterAccount account) {
		if (followers.contains(account))
			return true;
		return false;
	}
	
	public void tweet(String text) {
		Tweet tweet = new Tweet(this, text);
		tweets.add(0, tweet);
	}

	
	public void retweet(Tweet tweet) {
		Tweet retweet = new Tweet(this, tweet);
		tweets.add(0, retweet);
	}
	
	public Tweet getTweet(int i) {
		if (!(i > 0 && i < getTweetCount() + 1))
			throw new IllegalArgumentException("invalid index");
		return tweets.get(i-1);
	}
	
	public int getTweetCount() {
		return tweets.size();
	}
	
	public int getRetweetCount() {
		int count = 0;
		for (Tweet tweet: tweets) {
			count += tweet.getRetweetCount();
		}
		return count;
	}
	
	public static void main(String[] args) {

	}

}
