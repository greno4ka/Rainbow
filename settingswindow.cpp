#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QTranslator *newTranslator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    translator = newTranslator;

    configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settingsFilePath = configPath + "/settings.ini";
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // Load current settings when window is shown
    loadSettings();
}

void SettingsWindow::on_comboBox_activated(int value)
{
    // Load language setting
    if (!translator->load("rainbow_" + languageFromIndex(value))) {
        QMessageBox::critical(this, tr("Error: 0xDEADBEE"), tr("Can't change language."));
        return;
    }

    // Install language setting
    qApp->removeTranslator(translator);
    qApp->installTranslator(translator);

    // Finally use language setting to rewrite all text
    ui->retranslateUi(this);

    // Tell others to do it too!
    emit language_change(languageFromIndex(value));
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
    
    QString lang = settings.value("language", "en").toString();
    ui->comboBox->setCurrentIndex(indexFromLanguage(lang));
    
    int themeIndex = settings.value("theme", 0).toInt();
    ui->comboBox_2->setCurrentIndex(themeIndex);
    
    bool multisamplingEnabled = settings.value("multisampling", true).toBool();
    ui->checkBox_multisampling->setChecked(multisamplingEnabled);

    bool fullscreenEnabled = settings.value("fullscreen", false).toBool();
    ui->checkBox_fullscreen->setChecked(fullscreenEnabled);
}

void SettingsWindow::saveSettings()
{
    QSettings settings(settingsFilePath, QSettings::IniFormat);

    settings.setValue("language", languageFromIndex(ui->comboBox->currentIndex()));
    settings.setValue("theme", ui->comboBox_2->currentIndex());
    settings.setValue("multisampling", ui->checkBox_multisampling->isChecked());
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

    ui->label_done->setText("Done!");
    ui->label_done->setStyleSheet("color: green; font-weight: bold;");

    QTimer::singleShot(3000, this, [this]() {
        ui->label_done->clear();
    });
}

QString SettingsWindow::languageFromIndex(int index)
{
    switch (index) {
    case 0: return "en";
    case 1: return "ru";
    case 2: return "fr";
    default: return "en";
    }
}

int SettingsWindow::indexFromLanguage(QString &lang)
{
    if (lang == "ru") return 1;
    if (lang == "fr") return 2;
    return 0; // en default
}