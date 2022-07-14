package GUI;

import java.util.List;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.layout.GridPane;
import RC.thedrake.BoardPos;
import RC.thedrake.BoardTile;
import RC.thedrake.GameResult;
import RC.thedrake.GameState;
import RC.thedrake.Move;
import RC.thedrake.PlayingSide;
import RC.thedrake.PositionFactory;
import RC.thedrake.TilePos;
import RC.thedrake.ValidMoves;
import javafx.application.Platform;
import javafx.scene.control.Label;
import javafx.scene.text.Font;
import javafx.scene.text.TextAlignment;

/*
 * A View which displays the tiles on the board.
 */
public class BoardView extends GridPane implements TileViewContext {

    private GameState state;
    private TileView selected;
    private StackView selectedStack;
    private ValidMoves validMoves;
    private Label victoryLabel;
    
    private int orangeCapturedCnt = 0;
    private int blueCapturedCnt = 0;

    public BoardView(GameState state) {
        this.state = state;
        this.validMoves = new ValidMoves(state);

        // Set the parameters of the Board grid
        this.setVgap(5);
        this.setHgap(5);
        this.setPadding(new Insets(15));
        this.setAlignment(Pos.CENTER);

        PositionFactory pf = state.board().positionFactory();
        // Fill the grid with TileViews, each for one Tile in the game
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                int i = x;
                int j = 3 - y;
                BoardPos pos = pf.pos(i, j);
                this.add(new TileView(this, state.tileAt(pos), pos), x, y);
            }
        }
        StackView blueStack = new StackView(this, state.army(PlayingSide.BLUE).stack(), PlayingSide.BLUE);
        StackView orangeStack = new StackView(this, state.army(PlayingSide.ORANGE).stack(), PlayingSide.ORANGE);
        this.add(blueStack, 4, 3);
        this.add(orangeStack, 4, 0);
        
        //vytvorime labely
        Label blueCaptured = new Label("Blue player has captured: " + blueCapturedCnt + " troops");
        blueCaptured.setTextAlignment(TextAlignment.LEFT);
        blueCaptured.setFont(Font.font(20));
        this.add(blueCaptured, 4, 2);
        
        Label orangeCaptured = new Label("Orange player has captured:  " + orangeCapturedCnt + " troops");
        orangeCaptured.setTextAlignment(TextAlignment.LEFT);
        orangeCaptured.setFont(Font.font(20));
//tady udelej barvy
        this.add(orangeCaptured, 4, 1);
        
        this.victoryLabel = new Label("");
        victoryLabel.setFont(Font.font(30));
        victoryLabel.setTextAlignment(TextAlignment.RIGHT);
        this.add(victoryLabel, 4, 4);
    }

    /*
	 * Return the TileView on a given position
     */
    public TileView tileViewAt(BoardPos pos) {
        int index = (3 - pos.j()) * 4 + pos.i();
        return (TileView) getChildren().get(index);
    }

    /*
	 * Clear all moves associated with TileViews
     */
    private void clearMoves() {
        for (Node n : getChildren()) {
            if (n instanceof TileView) {
                TileView view = (TileView) n;
                view.clearMove();
            } else if (n instanceof StackView) {
                StackView view = (StackView) n;
                view.clearMove();
            } else {
            }
        }
    }

    /*
	 * Associate given moves with corresponding target TileViews.
     */
    private void showMoves(List<Move> moves) {
        for (Move move : moves) {
            tileViewAt(move.target()).setMove(move);
        }
    }

    /*
	 * Update the state of all TileViews according to the state of the game.
     */
    private void updateTiles() {
        for (Node n : getChildren()) {
            if (n instanceof TileView) {
                TileView view = (TileView)n;
                view.setTile(state.tileAt(view.position()));
                view.update();
            } else if (n instanceof StackView) {
                StackView view = (StackView)n;
                if (view.playingSide() != state.sideOnTurn() && selectedStack != null) {
                    view.removeFront();
                }

                view.update();
            } else {
                Label label = (Label)n;
                if (label.getText().charAt(0) == 'B') {
                    label.setText("Blue player has captured: " + blueCapturedCnt + " troops");
                } else if (label.getText().charAt(0) == 'O') {
                    label.setText("Orange player has captured: " + orangeCapturedCnt + " troops");
                } else {
                    
                }
            }
        }
    }

    @Override
    public void tileSelected(TileView view) {
        if (selected != null && selected != view) {
            selected.unselect();
        }

        selected = view;
        clearMoves();

        // Display all possible moves from the selected Tile
        showMoves(validMoves.boardMoves(view.position()));
    }

    @Override
    public void executeMove(Move move) {
        if (selected == null && selectedStack != null) {
            selectedStack.unselect();
            
            clearMoves();

            this.state = move.execute(state);
            this.validMoves = new ValidMoves(state);
            updateTiles();
            selectedStack = null;
        } else {
            BoardPos boardPosCopy = move.target();
            TilePos tilePosCopy = (TilePos)boardPosCopy;
            if (state.canCapture(selected.position(), tilePosCopy)) {
                if (PlayingSide.BLUE == state.sideOnTurn()) {
                    this.blueCapturedCnt++;
                } else {
                    this.orangeCapturedCnt++;
                }
            }
            selected.unselect();
            selected = null;
            clearMoves();

            
            this.state = move.execute(state);
            if (state.result() == GameResult.VICTORY) {
                //System.out.println("vitezstvi!");
                //tady bude win screen pro 
                //state.armyOnTurn().side()
                String victorySide;
                if (state.armyOnTurn().side() == PlayingSide.BLUE) {
                    victorySide = " MODRY ";
                } else {
                    victorySide = " ORANZOVY ";
                }
                victoryLabel.setText("VYHRAL" + victorySide + "HRAC");
                //Platform.exit();
            }
            if (state.result() == GameResult.DRAW) {
                //todo
            }
            this.validMoves = new ValidMoves(state);
            updateTiles();
        }
    }

    @Override
    public void tileSelected(StackView view) {
        if (selectedStack != null && selectedStack != view) {
            selectedStack.unselect();
        }
        if (view.tile() == BoardTile.EMPTY) {
            return;
        }

        selectedStack = view;
        clearMoves();
        
        if (view.playingSide() == state.sideOnTurn()) {
            showMoves(validMoves.movesFromStack());   
        }
    }
}
