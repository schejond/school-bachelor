package RC.thedrake;

import java.io.PrintWriter;

public class Board implements JSONSerializable {
	private final int dimension;
	private final BoardTile[][] tiles;
	
	public Board(int dimension) {
		this.dimension = dimension;
		this.tiles = emptyTiles(dimension);
	}
	
	private Board(int dimension, BoardTile[][] tiles) {
		this.dimension = dimension;
		this.tiles = tiles;
	}
	
	public int dimension() {
		return dimension;
	} 
	
	public BoardTile at(BoardPos pos) {
		return tiles[pos.i()][pos.j()];
	}
	
	private static BoardTile[][] emptyTiles(int dimension) {
		BoardTile[][] tiles = new BoardTile[dimension][dimension];
		
		for(int j = 0; j < dimension; j++) {
			for(int i = 0; i < dimension; i++) {
				tiles[i][j] = BoardTile.EMPTY;
			}
		}
		
		return tiles;
	}
	
	private static BoardTile[][] cloneTiles(BoardTile[][] tiles) {
		BoardTile[][] newTiles = new BoardTile[tiles.length][];
		
		for(int i = 0; i < tiles.length; i++) {
			newTiles[i] = tiles[i].clone();
		}
		
		return newTiles;
	}
	
	public Board withTiles(TileAt ...ats) {
		BoardTile[][] newTiles = cloneTiles(tiles);
		for(TileAt at : ats) {
			newTiles[at.pos.i()][at.pos.j()] = at.tile;
		}
		
		return new Board(this.dimension, newTiles);
	}
	
	public PositionFactory positionFactory() {
		return new PositionFactory(dimension);
	}
	
	public static class TileAt {
		public final BoardPos pos;
		public final BoardTile tile;
		
		public TileAt(BoardPos pos, BoardTile tile) {
			this.pos = pos;
			this.tile = tile;
		}
	}
	
	@Override
	public void toJSON(PrintWriter writer) {
		writer.printf("{\"dimension\":%d,\"tiles\":[", dimension);
		int count = 0;
		for(int j = 0; j < tiles.length; j++) {
			for(int i = 0; i < tiles[j].length; i++) {
				count++;
				tiles[i][j].toJSON(writer);
				if(count < dimension*dimension) {
					writer.printf(",");
				}
			}
		}
		writer.printf("]}");
	}
}

