package stateandbehavior;

public class StopWatch {
	private boolean hasStarted = false;
	private boolean hasStopped = false;
	private int ticks = 0; // totalt antall ticks uavhengig av om klokken er stoppet eller ikke
	private int time = 0;  // antall ticks klokken har gått
	private int lapTime = 0;
	private int lastLap = 0;
	
	
	public void tick(int ticks) {
		if (isStarted() && !isStopped()) {
			this.ticks += ticks;
			time += ticks;
			lapTime += ticks;
		}
		else {
			this.ticks += ticks;
		}
	}
	
	public void start() {
		hasStarted = true;
	}
	
	public void lap() {
		lastLap = lapTime;
		lapTime = 0;
	}
	
	public void stop() {
		hasStopped = true;
		lastLap = lapTime;
		lapTime = 0;
	}
	
	public boolean isStarted() {
		if (hasStarted == true) {
			return true;
		}
		else {
			return false;
		}
	}
	
	public boolean isStopped() {
		if (hasStopped == true) {
			return true;
		}
		else {
			return false;
		}
	}
	
	public int getTicks() {
		return ticks;
	}
	
	public int getTime() {
		if (!isStarted()) {
			return -1;
		}
		else {
			return time;
		}
	}
	
	public int getLapTime() {
		if (!isStarted()) {
			return -1;
		}
		else {
			return lapTime;
		}
	}
	
	public int getLastLapTime() {
		if (lastLap == 0) {
			return -1;
		}
		else {
			return lastLap;
		}
	}
}
