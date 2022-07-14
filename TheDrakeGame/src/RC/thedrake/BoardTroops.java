package RC.thedrake;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

public class BoardTroops implements JSONSerializable {
	private final PlayingSide playingSide;
	private final Map<BoardPos, TroopTile> troopMap;
	private final TilePos leaderPosition;
	private final int guards;
	
	public BoardTroops(PlayingSide playingSide) { 
		this(playingSide, 
				Collections.emptyMap(), 
				TilePos.OFF_BOARD,
				0);
	}
	
	public BoardTroops(
			PlayingSide playingSide,
			Map<BoardPos, TroopTile> troopMap,
			TilePos leaderPosition, 
			int guards) {
		this.playingSide = playingSide;		
		this.troopMap = troopMap;
		this.leaderPosition = leaderPosition;
		this.guards = guards;
	}

	public Optional<TroopTile> at(TilePos pos) {
		return Optional.ofNullable(troopMap.get(pos));
	}
	
	public PlayingSide playingSide() {
		return playingSide;
	}
	
	public TilePos leaderPosition() {
		return leaderPosition;
	}

	public int guards() {
		return guards;
	}
	
	public boolean isLeaderPlaced() {
		return leaderPosition() != TilePos.OFF_BOARD;
	}
	
	public boolean isPlacingGuards() {
		return leaderPosition != TilePos.OFF_BOARD && guards < 2;
	}	
	
	public Set<BoardPos> troopPositions() {
		return troopMap.keySet();
	}

	public BoardTroops placeTroop(Troop troop, BoardPos target) {
		if(at(target).isPresent())
			throw new IllegalArgumentException();
		
		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
		newTroops.put(target, new TroopTile(troop, playingSide, TroopFace.AVERS));

		int newGuards = guards;	
		if(isPlacingGuards())
			newGuards++;
		
		TilePos newLeaderPos = leaderPosition;
		if(leaderPosition == TilePos.OFF_BOARD) {
			newLeaderPos = target;
		}
		
		return new BoardTroops(playingSide(), newTroops, newLeaderPos, newGuards);
	}
	
	public BoardTroops troopStep(BoardPos origin, BoardPos target) {
		if(!isLeaderPlaced()) {
			throw new IllegalStateException(
					"Cannot move troops before the leader is placed.");			
		}
		
		if(isPlacingGuards()) {
			throw new IllegalStateException(
					"Cannot move troops before guards are placed.");			
		}
		
		if(!at(origin).isPresent())
			throw new IllegalArgumentException();
		
		if(at(target).isPresent())
			throw new IllegalArgumentException();
		
		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
		TroopTile tile = newTroops.remove(origin);
		newTroops.put(target, tile.flipped());

		TilePos newLeaderPos = leaderPosition;
		if(leaderPosition.equals(origin)) {
			newLeaderPos = target;
		}

		return new BoardTroops(playingSide(), newTroops, newLeaderPos, guards);
	}
	
	public BoardTroops troopFlip(BoardPos origin) {
		if(!isLeaderPlaced()) {
			throw new IllegalStateException(
					"Cannot move troops before the leader is placed.");			
		}
		
		if(isPlacingGuards()) {
			throw new IllegalStateException(
					"Cannot move troops before guards are placed.");			
		}
		
		if(!at(origin).isPresent())
			throw new IllegalArgumentException();
		
		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
		TroopTile tile = newTroops.remove(origin);
		newTroops.put(origin, tile.flipped());

		return new BoardTroops(playingSide(), newTroops, leaderPosition, guards);
	}
	
	public BoardTroops removeTroop(BoardPos target) {
		if(!isLeaderPlaced()) {
			throw new IllegalStateException(
					"Cannot move troops before the leader is placed.");			
		}
		
		if(isPlacingGuards()) {
			throw new IllegalStateException(
					"Cannot move troops before guards are placed.");			
		}
		
		if(!at(target).isPresent())
			throw new IllegalArgumentException();
		
		Map<BoardPos, TroopTile> newTroops = new HashMap<>(troopMap);
		newTroops.remove(target);
		
		TilePos newLeaderPos = leaderPosition;
		if(leaderPosition.equals(target)) {
			newLeaderPos = TilePos.OFF_BOARD;
		}
		
		return new BoardTroops(playingSide(), newTroops, newLeaderPos, guards);
	}
	
	@Override
	public void toJSON(PrintWriter writer) {
		writer.printf("{\"side\":");
  	playingSide.toJSON(writer);
  	writer.printf(",\"leaderPosition\":");
  	leaderPosition.toJSON(writer);
  	writer.printf(",\"guards\":%s,\"troopMap\":", guards);
  	troopMapToJSON(writer);
		writer.printf("}");
	}
	
	private void troopMapToJSON(PrintWriter writer) {
		writer.printf("{");
		int count = 0;
		
		List<BoardPos> sortedPos = new ArrayList<BoardPos>(troopMap.keySet());
		sortedPos.sort(new Comparator<BoardPos>() {
			public int compare(BoardPos pos1, BoardPos pos2) {
				return pos1.toString().compareTo(pos2.toString());
			};
		});
	
		for(BoardPos pos : sortedPos) {
			count++;
			pos.toJSON(writer);
			writer.printf(":");
			troopMap.get(pos).toJSON(writer);
			if(count < troopMap.size())
				writer.printf(",", pos.column(), pos.row());
		}
		writer.printf("}");
	}
}
