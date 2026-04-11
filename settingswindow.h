#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QTranslator>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

    QTranslator *translator;

signals:
    void language_change();
    void theme_change(bool isDark);
    void multisampling_change(bool enabled);
    void fullscreen_change(bool enabled);

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    void setTranslator(QTranslator *newTranslator);
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
    QString settingsFilePath;
};

#endif // SETTINGSWINDOW_H
