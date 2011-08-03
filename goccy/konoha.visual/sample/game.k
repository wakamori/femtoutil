using konoha.visual.*;

MAX_STONE_SIZE = 64;
MAX_STEP_SIZE = 64;

class Stone {
	TimeLine rotateTimeLine;
	Group stone;
	Ellipse forground;
	Ellipse background;
	int angle;
	int x;
	int y;
	int pos;

	Stone() {
		_angle = 0;
		_rotateTimeLine = new TimeLine();
		setTurnProperty();
		_forground = createForground();
		_background = createBackground();
		_stone = new Group();
		_stone.addToGroup(_forground);
		_stone.addToGroup(_background);
	}

	Ellipse createForground() {
		Ellipse forground = new Ellipse();
		Rect r = new Rect(-15, -15, 30, 30);
		forground.setRect(r);
		Color black = new Color("black");
		forground.setColor(black);
		forground.setZValue(1);
		return forground;
	}

	Ellipse createBackground() {
		Ellipse background = new Ellipse();
		Rect r = new Rect(-15, -15, 30, 30);
		background.setRect(r);
		Color white = new Color("white");
		background.setColor(white);
		background.setZValue(0);
		return background;
	}

	void setTurnProperty() {
		_rotateTimeLine.setDuration(950);
		_rotateTimeLine.setCurveShape(TimeLine.LinearCurve);
		Connector.connect(rotateTimeLine, TimeLine.ValueChanged, delegate(this, turnEffect));
	}
	
	void setColor(String color) {
		if (color == "white") {
			_forground.setZValue(0);
			_background.setZValue(1);
		}
	}

	void setPosition(int i, int j) {
		pos = j * 8 + i;
		i = (i + 1) * 50 + 25;
		j = (j + 1) * 50 + 25;
		stone.translate(i, j);
		x = i;
		y = j;
	}

	void turn(void)	{
		rotateTimeLine.start();
		angle = 0;
	}

	Group getStone(void) {
		return stone;
	}
	
	void turnEffect(float val) {
		if (angle > 180) return;
		Transform t = new Transform();
		t.translate(x, y);
		t.rotate(angle, Visual.YAxis);
		stone.setTransform(t, false);
		if (angle % 90 == 0 && (angle / 90) % 2 != 0) {
			if (forground.zValue < background.zValue) {
				forground.setZValue(1);
				background.setZValue(0);
			} else {
				forground.setZValue(0);
				background.setZValue(1);
			}
		}
		angle += 18;
	}
}

class Board {
	//Group board;
	Rect[] board;
	
	Board() {
		board = [];
		Color green = new Color("green");
		//board = new Group();
		int board_x = 50;
		int board_y = 50;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				Rect r = new Rect(0, 0, 50, 50);
				r.setColor(green);
				r.translate(board_x + 50 * i, board_y + 50 * j);
				board <<< r;
				//board.addToGroup(r);
			}
		}
		//board.translate(50, 50);
	}

	Rect[] getBoard(void) {
		return board;
	}
}

class GameStep {
	Stone put_stone;
	int[] turn_pos;

	GameStep(Stone stone, int[] turn_pos) {
		_put_stone = stone.copy();
		_turn_pos = [];
		for (int i = 0; i < |turn_pos|; i++) {
			_turn_pos.add(turn_pos[i]);
		}
	}
}

class GameWindow {
	Scene window;
	
	GameWindow(int width, int height) {
		window = new Scene();
		window.setItemIndexMethod(Scene.NoIndex);
		Color c = new Color("gray");
		window.setBackgroundColor(c);
		window.setSceneRect(0, 0, width, height);
	}
	
	void addBoard(Board b) {
		Rect[] panel = b.getBoard();
		for (int i = 0; i < |panel|; i++) {
			window.addItem(panel[i]);
		}
	}
	
	void addStone(Stone s) {
		window.addItem(s.getStone());
	}

	void addStones(Stone[] s) {
		for (int i = 0; i < |s|; i++) {
			window.addItem(s[i].getStone());
		}
	}
	
	void show() {
		View view = new View(window);
		view.setViewportUpdateMode(View.FullViewportUpdate);
		view.setRenderHint(Painter.Antialiasing);
		view.show();
	}
}

class GameEngine {
	GameStep[] step;
	Stone[] all_stone;
	Application app;
	GameWindow window;
	int step_count;
	int timer_id;
	
	GameEngine() {
		step = [];
		for (int i = 0; i < MAX_STONE_SIZE; i++) {
			all_stone.add(null);
		}
		step_count = 0;
		timer_id = 0;
		app = null;
		scene = null;
		init();
	}

	void init(String[] args) {
		app = new Application([""]);
		window = new GameWindow(500, 500);
		Board board = new Board();
		window.addBoard(board);
		Stone[] stones = createInitStones();
		window.addStones(stones);
	}

	Stone[] createInitStones() {
		Stone[] ret = [];
		for (int i = 0; i < 4; i++) {
			ret.add(new Stone());
		}
		ret[0].setColor("white");
		ret[0].setPosition(3, 3);
		ret[1].setPosition(4, 3);
		ret[2].setPosition(3, 4);
		ret[3].setColor("white");
		ret[3].setPosition(4, 4);
		all_stone[27] = ret[0];
		all_stone[28] = ret[1];
		all_stone[35] = ret[2];
		all_stone[36] = ret[3];
		return ret;
	}

	void addStep(int color_, int x, int y, int[] a) {
		Stone stone = new Stone();
		String color = (color_ == 2) ? "black" : "white";
		stone.setColor(color);
		stone.setPosition(x, y);
		int stone_pos = y * 8 + x;
		if (all_stone[stone_pos] == null) {
			all_stone[stone_pos] = stone;
		}
		int[] turn_pos = [];
		for (int i = 0; i < |a|; i++) {
			int pos = a[i];
			//int pos_x = pos / 8;
			//int pos_y = pos % 8;
			if (pos != stone_pos) {
				turn_pos <<< pos;
			}
		}
		step <<< new GameStep(stone, turn_pos);
	}
	
	void emulate() {
		step_count = 0;
		window.show();
		Timer t = new Timer(1000, delegate(this, runStep));
		timer_id = t.start();
		app.exec();
	}

	void runStep(TimerEvent event) {
		if (step_count < |step| && step[step_count] != null) {
			GameStep s = step[step_count];
			Stone stone = s.put_stone;
			window.addStone(stone);
			int[] turn_pos = s.turn_pos;
			for (int i = 0; i < |turn_pos|; i++) {
				all_stone[turn_pos[i]].turn();
			}
			step_count++;
		} else {
			//killTimer(timer_id);
		}
	}
}
