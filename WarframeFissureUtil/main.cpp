#include "WarframeFissureUtil.h"
#include <QtWidgets/QApplication>
#include "AcrylicWindow.h"
#include "src/phantomstyle.h"
#include "predefs.h"
int main(int argc, char* argv[])
{
	bool useGlass = true;
	QApplication a(argc, argv);
	WarframeFissureUtil w(nullptr);
	w.setGlass(true);
	w.setMouseTracking(true);
	w.setCaptionButtonsHandled(true);
	w.setMargins(20, 20, 0, 20);
	QApplication::setStyle(new PhantomStyle);
	w.show();
	return a.exec();
}
