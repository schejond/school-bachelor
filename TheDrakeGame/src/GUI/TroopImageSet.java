package GUI;

import java.io.InputStream;

import javafx.scene.image.Image;
import RC.thedrake.PlayingSide;
import RC.thedrake.TroopFace;

/*
 * Manages images for all the possible Troops
 */
public class TroopImageSet {

    private final Image troopFrontB;
    private final Image troopBackB;
    private final Image troopFrontO;
    private final Image troopBackO;

    public TroopImageSet(String troopName) {
        troopFrontB = new Image(assetFromJAR("front" + troopName + "B"));
        troopBackB = new Image(assetFromJAR("back" + troopName + "B"));
        troopFrontO = new Image(assetFromJAR("front" + troopName + "O"));
        troopBackO = new Image(assetFromJAR("back" + troopName + "O"));
    }

    private InputStream assetFromJAR(String fileName) {
        return getClass().getResourceAsStream("assets/" + fileName + ".png");
    }

    /*
	 * Returns an image of a Troop according to its PlayingSide and TroopFace
     */
    public Image get(PlayingSide side, TroopFace face) {
        if (side == PlayingSide.BLUE) {
            if (face == TroopFace.AVERS) {
                return troopFrontB;
            } else {
                return troopBackB;
            }
        } else {
            if (face == TroopFace.AVERS) {
                return troopFrontO;
            } else {
                return troopBackO;
            }
        }
    }
}
