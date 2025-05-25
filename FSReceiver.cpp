#include"FSReceiver.h"
#include"dlgBrowseFolder.h"
#include<QSet>
#include<QString>

FS_Receiver::FS_Receiver(QObject* parent)
    :AbstractReceiver(parent)
{
    QString path=dlgBrowseFolder::getDirectory();
    if(!path.isEmpty())
    {
        if(std::filesystem::is_directory(path.toStdString()))
        {
            dir_path=path.toStdString();
            emit AbstractReceiver::sgReadyToRead();
        }
        else
        {
            std::cout << "vaild directory!!!";
        }
    }
    else
    {
        std::cout<<"empty dir";
    }
}

bool FS_Receiver::pause()
{

}

void FS_Receiver::read()
{
    for(const auto& file_name:std::filesystem::recursive_directory_iterator(this->dir_path))
    {
        std::cout<<file_name<<std::endl;
        pack(file_name);
    }
}

void FS_Receiver::pack(const std::filesystem::path& f)
{
    if(!std::filesystem::is_regular_file(f)||!is_valid_extension(f))
    {
        return;
    }
    else
    {
        emit MatPackage(new Mat_Packet(cv::imread(f.string()),UniqueID::getNext()));
    }
}

bool FS_Receiver::is_valid_extension(const std::filesystem::path& p) {
    static const QSet<QString> exts{".jpg", ".png", ".bmp", ".h264"};
    const QString ext = QString::fromStdString(p.extension().string()).toLower();
    return exts.contains(ext);
}
