package GUI;

import RC.thedrake.Board;
import RC.thedrake.BoardTile;
import RC.thedrake.GameState;
import RC.thedrake.PositionFactory;
import RC.thedrake.StandardDrakeSetup;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.stage.Stage;

public class Controller implements Initializable {

    @FXML
    private Button pvpbutonik;

    @FXML
    private void end() {
        Platform.exit();
        System.exit(0);
    }

    @FXML
    private void localPvPGame() {
        GameState state = createSampleState();
        BoardView boardView = new BoardView(state);
        Scene scene = new Scene(boardView, 1344, 756);
        Stage stage = (Stage) pvpbutonik.getScene().getWindow();
        stage.setScene(scene);
        stage.setTitle("The Drake");
        stage.show();
    }

    private GameState createSampleState() {
        Board board = new Board(4);
        PositionFactory pf = board.positionFactory();
        board = board.withTiles(
                new Board.TileAt(pf.pos(1, 1), BoardTile.MOUNTAIN));

        GameState state = new StandardDrakeSetup().startState(board);

        return state;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
    }
}
