#include "cameraConfigDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>

CameraConfigDialog::CameraConfigDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("摄像头配置");
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("选择摄像头设备ID:", this));

    m_deviceIdSpin = new QSpinBox(this);
    m_deviceIdSpin->setMinimum(0);
    m_deviceIdSpin->setMaximum(10);
    layout->addWidget(m_deviceIdSpin);

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(m_buttons);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

int CameraConfigDialog::getDeviceId(bool* ok, QWidget* parent) {
    CameraConfigDialog dlg(parent);
    if (dlg.exec() == QDialog::Accepted) {
        if (ok) *ok = true;
        return dlg.m_deviceIdSpin->value();
    }
    if (ok) *ok = false;
    return -1;
}
