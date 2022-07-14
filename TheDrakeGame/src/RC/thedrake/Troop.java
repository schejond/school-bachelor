package RC.thedrake;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Troop implements JSONSerializable {
	private final String name;
	private final Offset2D aversPivot;
	private final Offset2D reversPivot;
	private final List<TroopAction> aversActions;
	private final List<TroopAction> reversActions;
	
	public Troop(
			String name, 
			Offset2D aversPivot, 
			Offset2D reversPivot, 
			List<TroopAction> aversActions,
			List<TroopAction> reversActions) {
		this.name = name;
		this.aversPivot = aversPivot;
		this.reversPivot = reversPivot;
		this.aversActions = new ArrayList<>(aversActions);
		this.reversActions = new ArrayList<>(reversActions);
	}
	
	public Troop(
			String name,
			Offset2D pivot, 
			List<TroopAction> frontActions,
			List<TroopAction> backActions) {
		this(
				name, 
				pivot,
				pivot,
				new ArrayList<>(frontActions),
				new ArrayList<>(backActions));
	}
	
	public Troop(
			String name,  
			List<TroopAction> frontActions,
			List<TroopAction> backActions) {
		this(
				name, 
				new Offset2D(1, 1), 
				new Offset2D(1, 1), 
				new ArrayList<>(frontActions),
				new ArrayList<>(backActions));
	}
	
	public String name() {
		return name;
	}

	public Offset2D pivot(TroopFace face) {
		return face == TroopFace.AVERS ? aversPivot : reversPivot;
	}
	
	public List<TroopAction> actions(TroopFace face) {
		return face == TroopFace.AVERS ? 
				Collections.unmodifiableList(aversActions) : 
				Collections.unmodifiableList(reversActions);
	}
	
	@Override
	public void toJSON(PrintWriter writer) {
		writer.printf("\"%s\"", this.name);
	}
}
