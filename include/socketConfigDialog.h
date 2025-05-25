#ifndef SOCKETCONFIGDIALOG_H
#define SOCKETCONFIGDIALOG_H

#include <QDialog>
#include <QHostAddress>

class QLineEdit;
class QSpinBox;
class QDialogButtonBox;

class SocketConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit SocketConfigDialog(QWidget* parent = nullptr);
    static bool getConnectionParams(QHostAddress* host, quint16* port, QWidget* parent = nullptr);

private:
    QLineEdit* m_hostEdit;
    QSpinBox* m_portSpin;
    QDialogButtonBox* m_buttons;
};

#endif // SOCKETCONFIGDIALOG_H
