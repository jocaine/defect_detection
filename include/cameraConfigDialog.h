#ifndef CAMERACONFIGDIALOG_H
#define CAMERACONFIGDIALOG_H

#include <QDialog>

class QSpinBox;
class QDialogButtonBox;

class CameraConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit CameraConfigDialog(QWidget* parent = nullptr);
    static int getDeviceId(bool* ok = nullptr, QWidget* parent = nullptr);

private:
    QSpinBox* m_deviceIdSpin;
    QDialogButtonBox* m_buttons;
};

#endif // CAMERACONFIGDIALOG_H
