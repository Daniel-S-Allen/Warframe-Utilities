#include "WarframeFissureUtil.h"
#include "AcrylicWindow.h"
#include "JSONReciever.h"

WarframeFissureUtil::WarframeFissureUtil(QWidget* parent, bool frameless)
	: QGoodWindow(parent, frameless)

{	
	ui.setupUi(this);
	ui.fissure_refresh->loadIcon(":/WarframeFissureUtil/Resources/refresh_icon.svg", 2);
	ui.fissure_refresh->setNormalColor(QColor::fromRgb(155, 155, 155));
	ui.fissure_refresh->setHoverColor(QColor::fromRgb(220, 220, 220));
	ui.fissure_refresh->setPressColor(QColor::fromRgb(55, 55, 220));
	ui.fissure_refresh->rotateOnPress(2000, false);

	ui.closebutton->loadIcon(":/WarframeFissureUtil/Resources/close_icon_black.svg");
	ui.closebutton->setNormalColor(QColor::fromRgb(155, 155, 155));
	ui.closebutton->setHoverColor(QColor::fromRgb(220, 220, 220));
	ui.closebutton->setPressColor(QColor::fromRgb(220, 55, 55));
	
	UpdateFissureTable();
	QObject::connect(ui.fissure_refresh, &QIconButton::mouseClick, this, &WarframeFissureUtil::UpdateFissureTable);
	QObject::connect(ui.fissureRelicTypeBox, &QComboBox::currentTextChanged, this, &WarframeFissureUtil::UpdateFissureTableFilters);
	QObject::connect(ui.fissureGamemodeBox, &QComboBox::currentTextChanged, this, &WarframeFissureUtil::UpdateFissureTableFilters);
	QObject::connect(ui.fissureFactionBox, &QComboBox::currentTextChanged, this, &WarframeFissureUtil::UpdateFissureTableFilters);
	QObject::connect(ui.fissureSteelPathBox, &QCheckBox::stateChanged, this, &WarframeFissureUtil::UpdateFissureTableFilters);
	QObject::connect(ui.fissureVoidStormBox, &QCheckBox::stateChanged, this, &WarframeFissureUtil::UpdateFissureTableFilters);

	
	QObject::connect(ui.closebutton, &QIconButton::mouseClick, this, [this]()->void{this->close(); });
}
void WarframeFissureUtil::setGlass(bool useGlass)
{
	HWND hwnd = (HWND)this->winId();
	HMODULE hUser = GetModuleHandle(L"user32.dll");
	if (hUser)
	{
		pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
		if (setWindowCompositionAttribute)
		{
			ACCENT_POLICY accent = { useGlass ? ACCENT_ENABLE_BLURBEHIND : ACCENT_DISABLED, 0, 0xFF000000, 0 };
			WINDOWCOMPOSITIONATTRIBDATA data;
			data.Attrib = WCA_ACCENT_POLICY;
			data.pvData = &accent;
			data.cbData = sizeof(accent);
			setWindowCompositionAttribute(hwnd, &data);
		}
	}
}
void WarframeFissureUtil::clearWidgets(QLayout* layout) {
	if (!layout)
		return;
	while (auto item = layout->takeAt(0)) {
		delete item->widget();
		clearWidgets(item->layout());
	}
}
void WarframeFissureUtil::UpdateFissureTableFilters(){
	QString tier = ui.fissureRelicTypeBox->currentText();
	QString mode = ui.fissureGamemodeBox->currentText();
	QString faction = ui.fissureFactionBox->currentText();
	int sp = ui.fissureSteelPathBox->checkState();
	int vs = ui.fissureVoidStormBox->checkState();
	proxyModel->UpdateFilters(tier, mode, faction, sp, vs);
}
WarframeFissureUtil::~WarframeFissureUtil()
{}
void WarframeFissureUtil::FilterFissures(QString type, QString tier, bool sp, bool vs) {
	
}
void WarframeFissureUtil::UpdateFissureTable() {
	QList<Fissure> fissures = Fissure::currentFissures();
	if (model == nullptr)
		model = new FissureTableModel(fissures, ui.fissure_table);
	else {
		model->Clear();
		model->SetData(Fissure::currentFissures());
	}
	if (proxyModel == nullptr) {
		proxyModel = new FissureFilterProxyModel(model);
		proxyModel->setSourceModel(model);
		proxyModel->setDynamicSortFilter(true);
		ui.fissure_table->setModel(proxyModel);
	}
	QList<QString> relicTypes;
	QList<QString> missionTypes;
	QList<QString> enemyTypes;
	for (Fissure& fissure : fissures) {
		//tier
		if(!relicTypes.contains(fissure.tier) && ui.fissureRelicTypeBox->findText(fissure.tier) == -1){
			relicTypes.append(fissure.tier);
		}
		if (!missionTypes.contains(fissure.type) && ui.fissureGamemodeBox->findText(fissure.type) == -1) {
			missionTypes.append(fissure.type);
		}
		if (!enemyTypes.contains(fissure.enemy) && ui.fissureFactionBox->findText(fissure.enemy) == -1) {
			enemyTypes.append(fissure.enemy);
		}
	}
	relicTypes.sort();
	missionTypes.sort();
	enemyTypes.sort();
	ui.fissureRelicTypeBox->addItems(relicTypes);
	ui.fissureGamemodeBox->addItems(missionTypes);
	ui.fissureFactionBox->addItems(enemyTypes);

	
	UpdateFissureTableFilters();
}
