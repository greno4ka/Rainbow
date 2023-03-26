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

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    void setTranslator(QTranslator *newTranslator);

private slots:
    void on_comboBox_activated(int value);

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
