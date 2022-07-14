package RC.thedrake;

import java.io.PrintWriter;

public enum PlayingSide implements JSONSerializable {
	BLUE, ORANGE;

	@Override
	public void toJSON(PrintWriter writer) {
		writer.printf("\"%s\"", this.toString());
	}
}