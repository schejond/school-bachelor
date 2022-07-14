package GUI;

import javafx.event.EventHandler;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderStroke;
import javafx.scene.layout.BorderStrokeStyle;
import javafx.scene.layout.BorderWidths;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import RC.thedrake.BoardPos;
import RC.thedrake.Move;
import RC.thedrake.Tile;

public class TileView extends Pane {

    private final TileViewContext context;
    private final BoardPos position;
    private Tile tile;
    private Move move;

    private final ImageView moveImage;
    private final TileBackgrounds backgrounds;
    private final Border selectBorder = new Border(
            new BorderStroke(
                    Color.BLACK,
                    BorderStrokeStyle.SOLID,
                    CornerRadii.EMPTY,
                    new BorderWidths(3)));

    public TileView(TileViewContext context, Tile tile, BoardPos position) {
        this.context = context;
        this.tile = tile;
        this.position = position;
        this.backgrounds = new TileBackgrounds();

        this.moveImage = new ImageView(getClass().getResource("assets/move.png").toString());
        moveImage.setVisible(false);

        this.setPrefSize(100, 100);
        this.setMinSize(USE_PREF_SIZE, USE_PREF_SIZE);
        this.setMaxSize(USE_PREF_SIZE, USE_PREF_SIZE);
        this.getChildren().add(moveImage);
        this.setOnMouseClicked(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent event) {
                onClicked();
            }
        });
        update();
    }

    public BoardPos position() {
        return position;
    }

    public void setTile(Tile tile) {
        this.tile = tile;
    }

    public void update() {
        this.setBackground(backgrounds.get(tile));
    }

    public void select() {
        this.setBorder(selectBorder);
        context.tileSelected(this);
    }

    public void unselect() {
        this.setBorder(null);
    }

    public void setMove(Move move) {
        this.move = move;
        moveImage.setVisible(true);
    }

    public void clearMove() {
        this.move = null;
        moveImage.setVisible(false);
    }

    private void onClicked() {
        if (move != null) {
            context.executeMove(move);
        } else if (tile.hasTroop()) {
            select();
        }
    }
}
