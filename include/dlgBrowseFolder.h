// dlgBrowseFolder.h
#ifndef DLGBROWSEFOLDER_H
#define DLGBROWSEFOLDER_H

#include <QDialog>
#include <QFileSystemModel>

class QTreeView;
class QDialogButtonBox;

class dlgBrowseFolder : public QDialog {
    Q_OBJECT
public:
    explicit dlgBrowseFolder(QWidget* parent = nullptr);

    // 静态方法快速获取目录
    static QString getDirectory(QWidget* parent = nullptr);

    QString selectedDir() const;

private slots:
    void onDirectorySelected(const QModelIndex &index);

private:
    void setupUI();

    QFileSystemModel* m_dirModel;
    QTreeView* m_treeView;
    QDialogButtonBox* m_buttons;
    QString m_currentDir;
};

#endif // DLGBROWSEFOLDER_H
