#include "messagenotification.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseInfo dbinfo("QSQLITE", "localhost", "E:\\DB\\Logs.db", "", "");
    MessageNotification w;

    w.show();

    if (!w.createConnetion(&dbinfo)) {
      //��蔭���BDB�ǂݍ��ݒ����悤�ɂ��鎖�B
    }
    w.run();

    return a.exec();
}
