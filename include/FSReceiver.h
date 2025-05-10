#ifndef FSRECEIVER_H
#define FSRECEIVER_H
#include"AbstractReceiver.h"

#include<filesystem>

class FS_Receiver:public AbstractReceiver
{
    Q_OBJECT
public:
    FS_Receiver(QObject* parent=nullptr);
    virtual bool start()override;
    virtual bool pause()override;
    virtual void read()override;
private:
    void pack(const std::filesystem::path&);

    bool is_valid_extension(const std::filesystem::path& p);

private:
    std::filesystem::path dir_path;
};
#endif // FSRECEIVER_H
