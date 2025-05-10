#include"FSReceiver.h"

#include<QSet>
#include<QString>

FS_Receiver::FS_Receiver(QObject* parent)
    :AbstractReceiver(parent)
{
}

bool FS_Receiver::start()
{
    std::string path_str="/home/tlr/files";
    //std::cout << "请文件路径：";
    //std::cin >> path_str;
    if (path_str.empty())
    {
        std::cout << "vaild input!!!";
        return false;
    }

    if (!std::filesystem::is_directory(path_str))
    {
        std::cout << "vaild directory!!!";
        return false;
    }

    dir_path=path_str;
    return true;
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
        emit MatPackage(Mat_Packet(cv::imread(f.string()),UniqueID::getNext()));
    }
}

bool FS_Receiver::is_valid_extension(const std::filesystem::path& p) {
    static const QSet<QString> exts{".jpg", ".png", ".bmp", ".h264"};
    const QString ext = QString::fromStdString(p.extension().string()).toLower();
    return exts.contains(ext);
}
