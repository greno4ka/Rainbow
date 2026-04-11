#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    
    // Set up settings file path
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configPath);  // Ensure the directory exists
    settingsFilePath = configPath + "/settings.ini";
}

SettingsWindow::~SettingsWindow()
{
    delete translator;

    delete ui;
}

void SettingsWindow::setTranslator(QTranslator *newTranslator)
{
    translator = newTranslator;
}

void SettingsWindow::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // Load current settings when window is shown
    loadSettings();
}

void SettingsWindow::on_comboBox_activated(int value)
{
    switch (value) {
    case 0:
        if (! translator->load(QString("rainbow_") + QString("en")) )
            QMessageBox::critical(0,tr("Error: 0xDEADBEE"), tr("Can't change language."));
        break;
    case 1:
        if (! translator->load(QString("rainbow_") + QString("ru")) )
            QMessageBox::critical(0,tr("Error: 0xDEADBEE"), tr("Can't change language."));
        break;
    case 2:
        if (! translator->load(QString("rainbow_") + QString("fr")) )
            QMessageBox::critical(0,tr("Error: 0xDEADBEE"), tr("Can't change language."));
        break;
    default:
        break;
    }
    ui->retranslateUi(this);
    emit language_change();
}

void SettingsWindow::on_comboBox_2_activated(int index)
{
    // index 0 is Dark theme, index 1 is Light theme
    bool isDark = (index == 0);
    emit theme_change(isDark);
}

void SettingsWindow::loadSettings()
{
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    
    // Load language setting
    int languageIndex = settings.value("language", 0).toInt();
    ui->comboBox->setCurrentIndex(languageIndex);
    
    // Load theme setting
    int themeIndex = settings.value("theme", 0).toInt();
    ui->comboBox_2->setCurrentIndex(themeIndex);
    
    // Load multisampling setting
    bool multisamplingEnabled = settings.value("multisampling", true).toBool();
    ui->checkBox_multisampling->setChecked(multisamplingEnabled);
    
    // Load fullscreen setting
    bool fullscreenEnabled = settings.value("fullscreen", false).toBool();
    ui->checkBox_fullscreen->setChecked(fullscreenEnabled);
}

void SettingsWindow::saveSettings()
{
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    
    // Save language setting
    settings.setValue("language", ui->comboBox->currentIndex());
    
    // Save theme setting
    settings.setValue("theme", ui->comboBox_2->currentIndex());
    
    // Save multisampling setting
    settings.setValue("multisampling", ui->checkBox_multisampling->isChecked());
    
    // Save fullscreen setting
    settings.setValue("fullscreen", ui->checkBox_fullscreen->isChecked());
    
    settings.sync();
}

void SettingsWindow::on_checkBox_multisampling_stateChanged(int state)
{
    emit multisampling_change(state == Qt::Checked);
}

void SettingsWindow::on_checkBox_fullscreen_stateChanged(int state)
{
    emit fullscreen_change(state == Qt::Checked);
}

void SettingsWindow::on_pushButton_save_clicked()
{
    saveSettings();
    QMessageBox::information(this, tr("Settings"), tr("Settings saved successfully."));
}

