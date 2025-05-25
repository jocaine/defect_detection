#include "socketConfigDialog.h"

#include<QLineEdit>
#include <QVBoxLayout>
#include<QSpinBox>
#include<QDialogButtonBox>
#include <QFormLayout>
#include <QRegExpValidator>

SocketConfigDialog::SocketConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("网络配置");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* form = new QFormLayout;
    m_hostEdit = new QLineEdit("127.0.0.1", this);
    m_hostEdit->setValidator(new QRegExpValidator(QRegExp("^\\d{1,3}(\\.\\d{1,3}){3}$"), this));
    form->addRow("服务器地址:", m_hostEdit);

    m_portSpin = new QSpinBox(this);
    m_portSpin->setRange(1, 65535);
    m_portSpin->setValue(8080);
    form->addRow("端口号:", m_portSpin);

    mainLayout->addLayout(form);

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(m_buttons);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

bool SocketConfigDialog::getConnectionParams(QHostAddress* host, quint16* port, QWidget* parent) {
    SocketConfigDialog dlg(parent);
    if (dlg.exec() == QDialog::Accepted) {
        bool valid = true;
        QHostAddress address(dlg.m_hostEdit->text());
        if (address.isNull()) {
            valid = false;
        }
        *host = address;
        *port = static_cast<quint16>(dlg.m_portSpin->value());
        return valid;
    }
    return false;
}
