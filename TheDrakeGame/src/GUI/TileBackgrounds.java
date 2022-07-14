package GUI;

import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.paint.Color;
import RC.thedrake.BoardTile;
import RC.thedrake.PlayingSide;
import RC.thedrake.Tile;
import RC.thedrake.Troop;
import RC.thedrake.TroopFace;
import RC.thedrake.TroopTile;

/**
 * Manages backgrounds for all possible Tiles
 */
public class TileBackgrounds {

    public static final Background EMPTY_BG = new Background(
            new BackgroundFill(new Color(0.9, 0.9, 0.9, 1), null, null));
    private final Background mountainBg;

    public TileBackgrounds() {
        Image img = new Image(getClass().getResourceAsStream("assets/mountain.png"));
        this.mountainBg = new Background(
                new BackgroundImage(img, null, null, null, null));
    }

    /*
	 * Returns appropriate background for a given Tile
     */
    public Background get(Tile tile) {
        if (tile.hasTroop()) {
            TroopTile armyTile = ((TroopTile) tile);
            return getTroop(armyTile.troop(), armyTile.side(), armyTile.face());
        }

        if (tile == BoardTile.MOUNTAIN) {
            return mountainBg;
        }

        return EMPTY_BG;
    }

    /*
	 * Returns background for a given Troop
     */
    public Background getTroop(Troop info, PlayingSide side, TroopFace face) {
        TroopImageSet images = new TroopImageSet(info.name());
        BackgroundImage bgImage = new BackgroundImage(
                images.get(side, face), null, null, null, null);

        return new Background(bgImage);
    }
}
