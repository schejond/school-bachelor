package GUI;

import RC.thedrake.Move;

/* 
 * A context throw which a TileView communicates an action 
 * which needs to be executed. 
 */
public interface TileViewContext {
  public void tileSelected(StackView view);
  public void tileSelected(TileView view);
  public void executeMove(Move move);
}
