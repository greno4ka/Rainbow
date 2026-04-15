#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

    QTranslator *translator = nullptr;

signals:
    void language_change(const QString &language);
    void theme_change(bool isDark);
    void multisampling_change(bool enabled);
    void fullscreen_change(bool enabled);

public:
    explicit SettingsWindow(QTranslator *newTranslator, QWidget *parent = nullptr);
    ~SettingsWindow();

    void loadSettings();
    void saveSettings();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_comboBox_activated(int value);
    void on_comboBox_2_activated(int index);
    void on_checkBox_multisampling_stateChanged(int state);
    void on_checkBox_fullscreen_stateChanged(int state);
    void on_pushButton_save_clicked();

private:
    Ui::SettingsWindow *ui;

    QString configPath;
    QString settingsFilePath;

    QString languageFromIndex(int index);
    int indexFromLanguage(QString &lang);
};

#endif // SETTINGSWINDOW_H
