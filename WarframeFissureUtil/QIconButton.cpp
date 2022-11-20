#include "QIconButton.h"
#include <QPainter>
#include <QScreen>
#include <QApplication>
#include <windows.h>

QIconButton::QIconButton(QWidget* parent) : QWidget(parent)
{
	this->hovering = false;
	this->animationTimer = new QTimer(this);
	this->frameTime = 1000/QApplication::primaryScreen()->refreshRate();
	this->playRotateAnimation = false;
	this->setMouseTracking(true);

	this->iconRotation = 0;
	connect(animationTimer, &QTimer::timeout, this, &QIconButton::updateAnimation);
}
void QIconButton::setNormalColor(QColor color){
	this->normalColor = color;
}
void QIconButton::setHoverColor(QColor color) {
	this->hoverColor = color;
}
void QIconButton::setPressColor(QColor color) {
	this->pressColor = color;
}
void QIconButton::loadIcon(const QString& filename, int scaling){
	this->svgRenderer = new QSvgRenderer(this);
	this->svgRenderer->load(filename);
	delete staticPixmap;
	iconScaling = scaling;
	staticPixmap = new QImage(this->width()*scaling, this->width() * scaling, QImage::Format_ARGB32);
	staticPixmap->fill(Qt::transparent);
	QPainter painter(staticPixmap);
	svgRenderer->render(&painter);
	painter.end();
	delete svgRenderer;

}
void QIconButton::resizeEvent(QResizeEvent* event)
{
	pixmapInvalid = true;
}
void QIconButton::rotateOnPress(double duration, int repeat)
{
	iconRotation = 0;
	rotateDuration = duration;
	if (useRealTime)
		lastFrameTime = std::chrono::system_clock::now();
	if(duration == 0){
		playRotateAnimation = false;
		animationTimer->stop();
		animationTimer->setInterval(0);
	} else {
		playRotateAnimation = true;
		maxRepeats = repeat;
		animationRepeats = repeat;
		animationTimer->setInterval(frameTime);
	}
}
void QIconButton::enterEvent(QEnterEvent* event){
	hovering = true; 
	update();
}
void QIconButton::mousePressEvent(QMouseEvent* event){
	if (rect().contains(event->pos())) {
		mousedown = true;
		update();
		emit mouseDown();
	}
}
void QIconButton::mouseReleaseEvent(QMouseEvent* event) {
	if (mousedown) {
		mousedown = false;
		if (rect().contains(event->pos())){
			if (playRotateAnimation) {
				if (useRealTime)
					lastFrameTime = std::chrono::system_clock::now();
				animationRepeats = maxRepeats;
				animationTimer->start();
				updateAnimation();
			}
			emit mouseClick();
		}
	}
	emit mouseUp();
}
void QIconButton::leaveEvent(QEvent* event) {
	hovering = false;
	mousedown = false;
	update();
}
void QIconButton::updateAnimation(){
	double timeDifference = 0;
	if (useRealTime) {
		auto timeSinceLastFrame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastFrameTime);
		timeDifference = timeSinceLastFrame.count();
	} else {
		timeDifference = frameTime;
	}
	double percentOfFullRotation = timeDifference / rotateDuration;
	iconRotation += 360 * percentOfFullRotation;
	if (iconRotation >= 360) {
		iconRotation -= 360;
		if (animationRepeats >= 0) {
			if (animationRepeats == 0) {
				animationTimer->stop();
				iconRotation = 0;
			}
			animationRepeats--;
		}
	}
	if (useRealTime)
		lastFrameTime = std::chrono::system_clock::now();
	update();
}

void QIconButton::paintEvent(QPaintEvent* event)
{
	if(pixmapInvalid){
		delete pixmap;
		//pixmap = new QPixmap(this->width() * iconScaling, this->width() * iconScaling, QImage::Format_ARGB32);
		pixmap = new QImage(this->width() * iconScaling, this->width() * iconScaling, QImage::Format_ARGB32);
		pixmap->fill(Qt::transparent);
		pixmapInvalid = false;
	}
	QPainter mapPainter(pixmap);
	mapPainter.drawImage(pixmap->rect(), *staticPixmap);
	mapPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	mapPainter.fillRect(pixmap->rect(), hovering ? (mousedown ? pressColor : hoverColor) : normalColor);
	mapPainter.end();

	QPainter buttonPainter(this);
	QPointF center(width() / 2.0, height() / 2.0);
	buttonPainter.setRenderHint(QPainter::SmoothPixmapTransform);
	buttonPainter.setRenderHint(QPainter::Antialiasing);
	buttonPainter.translate(center);
	buttonPainter.rotate(iconRotation);
	//buttonPainter.drawPixmap(-rect().center().x(), -rect().center().y(), rect().width(), rect().height(), *pixmap);
	QRect target(-rect().center().x(), -rect().center().y(), width(), height());
	buttonPainter.drawImage(target, *pixmap);
	buttonPainter.resetTransform();
	//buttonPainter.drawRect(0,0,width()-1,height()-1);
	buttonPainter.end();
}
void QIconButton::setSize(const QSize& size)
{
	this->msize = size;
	this->setBaseSize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}
QSize QIconButton::getSize()
{
	return msize;
}