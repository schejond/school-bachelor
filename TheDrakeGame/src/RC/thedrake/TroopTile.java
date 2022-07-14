package RC.thedrake;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class TroopTile implements Tile {

	private final Troop troop;
	private final PlayingSide side;
	private final TroopFace face;

	public TroopTile(Troop troop, PlayingSide side, TroopFace face) {
		this.troop = troop;
		this.side = side;
		this.face = face;
	}

	public PlayingSide side() {
		return side;
	}

	public TroopFace face() {
		return face;
	}

	public Troop troop() {
		return troop;
	}

	@Override
	public boolean canStepOn() {
		return false;
	}

	@Override
	public boolean hasTroop() {
		return true;
	}

	public TroopTile flipped() {
		if (face == TroopFace.AVERS)
			return new TroopTile(troop, side, TroopFace.REVERS);

		return new TroopTile(troop, side, TroopFace.AVERS);
	}

	public List<Move> movesFrom(BoardPos origin, GameState state) {
		List<Move> result = new ArrayList<>();
		for (TroopAction action : troop.actions(face)) {
			result.addAll(action.movesFrom(origin, side, state));
		}

		return result;
	}

	@Override
	public void toJSON(PrintWriter writer) {
		writer.printf("{\"troop\":");
		troop.toJSON(writer);
		writer.printf(",\"side\":");
		side.toJSON(writer);
		writer.printf(",\"face\":");
		face.toJSON(writer);
		writer.printf("}");
	}
}
