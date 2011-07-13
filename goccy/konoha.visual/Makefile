CC = g++
QTDIR ?= /usr/local/Trolltech/Qt-4.7.3
INCPATH = -I$(QTDIR)/mkspecs/macx-g++ -I. -I$(QTDIR)/lib/QtCore.framework/Versions/4/Headers -I$(QTDIR)/include/QtCore -I$(QTDIR)/lib/QtGui.framework/Versions/4/Headers -I$(QTDIR)/include/QtGui -I$(QTDIR)/include -I. -I. -F$(QTDIR)/lib -I/usr/local/include/Box2D -I/usr/local/include/opencv2 -I/usr/local/include/opencv
LFLAGS = -headerpad_max_install_names -arch x86_64 -Xarch_x86_64 -mmacosx-version-min=10.5
#OPENCV_LIBS = `pkg-config opencv --libs`
OPENCV_LIBS = -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ts -lopencv_video
OPENCV_FLAGS = #`pkg-config opencv --cflags`
LIBS = -F$(QTDIR)/lib -L$(QTDIR)/lib -framework QtGui -L$(QTDIR)/lib -F$(QTDIR)/lib -framework QtCore $(OPENCV_LIBS)
DEFINES = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -DK_USING_BOX2D -DK_USING_OPENCV
CFLAGS = -O0 -g3 -pipe -g -gdwarf-2 -Wall -W -arch x86_64 -Xarch_x86_64 -mmacosx-version-min=10.5 $(DEFINES) -I/usr/local/include/ -I./include/ -I$(QTDIR)/include -Wall -W -fPIC $(INCPATH) $(OPENCV_FLAGS) -DNDEBUG
LDLIBS = $(LFLAGS) $(LIBS) -lkonoha1 -lBox2D

pkgname = visual
library = "$(pkgname).dylib"
objs = build/visual.o\
	build/moc_visual.o\
	build/Application.o\
	build/Ellipse.o\
	build/Item.o\
	build/Rect.o\
	build/TimeLine.o\
	build/Transform.o\
	build/Connector.o\
	build/Group.o\
	build/Painter.o\
	build/Scene.o\
	build/Timer.o\
	build/View.o\
	build/Color.o\
	build/Brush.o\
	build/World.o\
	build/Texture.o\
	build/Text.o\
	build/Contact.o\
	build/Shadow.o\
	build/Camera.o\
	build/Point.o\
	build/MouseEvent.o

.PHONY: all
all: $(library)
#	cat interface/hierarchy.k interface/main.k interface/widget.k interface/gesture.k interface/physics.k interface/rect.k interface/ellipse.k interface/text.k interface/pixmap.k interface/joint.k interface/view.k interface/color.k > qt.k

build/visual.o: src/visual.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/moc_visual.o: src/moc_visual.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Application.o: src/Application.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Brush.o: src/Brush.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Camera.o: src/Camera.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Connector.o: src/Connector.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Contact.o: src/Contact.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Color.o: src/Color.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Ellipse.o: src/Ellipse.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Group.o: src/Group.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/MouseEvent.o: src/MouseEvent.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Rect.o: src/Rect.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Item.o: src/Item.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Texture.o: src/Texture.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Text.o: src/Text.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/TimeLine.o: src/TimeLine.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Transform.o: src/Transform.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Painter.o: src/Painter.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Point.o: src/Point.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Timer.o: src/Timer.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Shadow.o: src/Shadow.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/Scene.o: src/Scene.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/View.o: src/View.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

build/World.o: src/World.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

$(library): $(objs)
	$(CC) -dynamiclib -o $@ $^ $(LDLIBS)

src/moc_visual.cpp: include/visual.hpp
	$(QTDIR)/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ $^ -o $@

.PHONY: clean
clean:
	$(RM) -rf $(objs) $(library) *~
