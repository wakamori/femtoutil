using konoha.visual.*;

class Gesture {
	float rotationAngle = 0;
	float position = 0;
	float horizontalOffset = 0;
	float verticalOffset = 0;
	float scaleFactor = 1;
	float currentStepScaleFactor = 1;
	
	void pinchEvent(QWidget w, PinchGesture gesture, dynamic data) {
		OUT <<< "pinch" <<< EOL;
		int change_flags = gesture.changeFlags();
		if (change_flags & QPinchGesture.ROTATION_ANGLE_CHANGED) {
			float value = gesture.getRotationAngle();
			float lastValue = gesture.getLastRotationAngle();
			rotationAngle += value - lastValue;
			//OUT <<< "[changeflags] : rotation angle = " + rotationAngle <<< EOL;
		}
		if (change_flags & QPinchGesture.SCALE_FACTOR_CHANGED) {
			float value = gesture.getScaleFactor();
			currentStepScaleFactor = value;
			//OUT <<< "[changeflags] : current step scale factor = " + currentStepScaleFactor <<< EOL;
		}
		if (gesture.state() == Qt.GESTURE_FINISHED) {
			scaleFactor *= currentStepScaleFactor;
			currentStepScaleFactor = 1;
			//OUT <<< "[gesture.state] : current step scale factor = " + currentStepScaleFactor <<< EOL;
		}
		w.update();
	}

	void paintEvent(QWidget w, QPainter p, dynamic data) {
		//OUT <<< "paint event" <<< EOL;
		QPixmap image = (QPixmap)data;
		float iw = image.width;
		float ih = image.height;
		float wh = w.height;
		float ww = w.width;
		p.translate(ww/2, wh/2);
		p.translate(horizontalOffset, verticalOffset);
		p.rotate(rotationAngle);
		p.scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
		p.translate(-iw/2, -ih/2);
		p.drawPixmap(image);
	}

	void swipeEvent(QWidget w, SwipeGesture gesture, dynamic data) {
		OUT <<< "swipe" <<< EOL;
	}

	void panEvent(QWidget w, QPanGesture gesture, dynamic data) {
		OUT <<< "pan" <<< EOL;
		//QPointF delta = gesture.delta();
		//horizontalOffset += delta.x();
		//verticalOffset += delta.y();
		w.update();
	}
}

void main(String[] args)
{
	Application app = new Application(args);
	Widget w = new Widget();
	Pixmap image = new Pixmap("sample/redhand.png");
	image.setPosition(-100, -100);
	w.add(image);
	w.addEvent("swipe", swipe_event, null);
	w.addEvent("pinch", pinch_event, null);
	w.addEvent("pan", pan_event, null);
	w.addEvent("paint", paint_event, image);
	w.resize(600, 600);
	//w.setWindowOpacity(0.7);
	w.show();
	app.exec();
}
