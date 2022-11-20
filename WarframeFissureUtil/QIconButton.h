#pragma once
#include <QPushButton>
#include <QMouseEvent>
#include <QSvgRenderer>
#include <QPixmap>
#include <QEnterEvent>
#include <QEvent>
#include <QTimer>
#include <chrono>
#include <QProperty>
class QIconButton :
	public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QSize msize READ getSize WRITE setSize DESIGNABLE true)
	Q_PROPERTY(QColor normalColor WRITE setNormalColor DESIGNABLE true)
	Q_PROPERTY(QColor hoverColor WRITE setHoverColor DESIGNABLE true)
	Q_PROPERTY(QColor pressColor WRITE setPressColor DESIGNABLE true)
private:
	int paddingLeft{};
	int paddingRight{};
	int paddingTop{};
	int paddingBottom{};

	QSize msize;
	QSvgRenderer* svgRenderer;
	QColor normalColor;
	QColor hoverColor;
	QColor pressColor;
	QImage* pixmap = nullptr;
	QImage* staticPixmap = nullptr;
	int iconScaling;
	
	double frameTime;
	QTimer* animationTimer;
	double rotateDuration;
	double iconRotation;
	int maxRepeats;
	int animationRepeats;
	std::chrono::time_point<std::chrono::system_clock> lastFrameTime;

	bool mousedown = false;
	bool useRealTime = false;
	bool hovering = false;
	bool pixmapInvalid;
	bool playRotateAnimation;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	
public:
	QIconButton(QWidget* parent);
	~QIconButton(){
		delete pixmap;
		delete staticPixmap;
		delete animationTimer;
	}
	void setSize(const QSize& size);
	QSize getSize();
	void setNormalColor(QColor color);
	void setHoverColor(QColor color);
	void setPressColor(QColor color);
	void loadIcon(const QString& filename, int scaling = 2);
	void rotateOnPress(double duration, int repeat = -1);

public slots:
	void updateAnimation();

	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* e) override;

	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;
signals:
	void mouseDown();
	void mouseUp();
	void mouseClick();
};

