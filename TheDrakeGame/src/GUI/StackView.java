package GUI;

import RC.thedrake.BoardTile;
import RC.thedrake.Move;
import RC.thedrake.PlayingSide;
import RC.thedrake.Tile;
import RC.thedrake.TilePos;
import RC.thedrake.Troop;
import RC.thedrake.TroopFace;
import RC.thedrake.TroopTile;
import java.util.ArrayList;
import java.util.List;
import javafx.event.EventHandler;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Background;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderStroke;
import javafx.scene.layout.BorderStrokeStyle;
import javafx.scene.layout.BorderWidths;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.Pane;
import static javafx.scene.layout.Region.USE_PREF_SIZE;
import javafx.scene.paint.Color;

public class StackView extends Pane {
    private List<Troop> army;

    private final TileViewContext context;
    
    private Tile tile;
    private Move move;
    private PlayingSide playingSide;

    private final ImageView moveImage;
    private final TileBackgrounds backgrounds;

    private final Border selectBorder = new Border(
            new BorderStroke(
                    Color.BLACK,
                    BorderStrokeStyle.SOLID,
                    CornerRadii.EMPTY,
                    new BorderWidths(3)));

    public StackView(TileViewContext context, final List<Troop> army, PlayingSide side) {
        this.context = context;
        this.tile = new TroopTile(army.get(0), side, TroopFace.AVERS);

        this.backgrounds = new TileBackgrounds();

        this.playingSide = side;
        this.army = army;

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

    public PlayingSide playingSide() {
        return playingSide;
    }

    public void removeFront() {
        if (army.size() > 1) {
            army = army.subList(1, army.size());
            this.tile = new TroopTile(army.get(0), playingSide, TroopFace.AVERS);
        } else if (army.size() == 1) {
            army = new ArrayList<>();
            this.tile = BoardTile.EMPTY;
        }
    }
    
    public Tile tile() {
        return tile;
    }

    public void setTile(Tile tile) {
        this.tile = tile;
    }

    public void update() {
        if (army.isEmpty()) {
            this.setBackground(TileBackgrounds.EMPTY_BG);
        } else {
            this.setBackground(backgrounds.get(tile));    
        }    
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
