// dlgBrowseFolder.cpp
#include "dlgBrowseFolder.h"
#include <QVBoxLayout>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QHeaderView>

dlgBrowseFolder::dlgBrowseFolder(QWidget* parent)
    : QDialog(parent),
    m_dirModel(new QFileSystemModel(this)),
    m_treeView(new QTreeView(this)),
    m_buttons(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    setupUI();

    // 设置模型过滤仅显示目录
    m_dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    m_dirModel->setRootPath(QDir::homePath());

    // 初始化树视图
    m_treeView->setModel(m_dirModel);
    m_treeView->setRootIndex(m_dirModel->index(QDir::homePath()));
    m_treeView->hideColumn(1); // 隐藏类型列
    m_treeView->hideColumn(2); // 隐藏大小列
    m_treeView->hideColumn(3); // 隐藏日期列
    m_treeView->header()->hide();

    // 连接信号
    connect(m_treeView, &QTreeView::clicked, this, &dlgBrowseFolder::onDirectorySelected);
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString dlgBrowseFolder::getDirectory(QWidget* parent)
{
    dlgBrowseFolder dlg(parent);
    if(dlg.exec() == QDialog::Accepted) {
        return dlg.selectedDir();
    }
    return QString();
}

QString dlgBrowseFolder::selectedDir() const
{
    return m_currentDir;
}

void dlgBrowseFolder::onDirectorySelected(const QModelIndex &index)
{
    m_currentDir = m_dirModel->filePath(index);
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void dlgBrowseFolder::setupUI()
{
    setWindowTitle("选择文件夹");
    setMinimumSize(400, 500);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_treeView);
    layout->addWidget(m_buttons);

    // 设置按钮初始状态
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);

    m_treeView->setExpandsOnDoubleClick(true);
}
