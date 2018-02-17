#include "messagenotification.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseInfo dbinfo("QSQLITE", "localhost", "E:\\DB\\Logs.db", "", "");
    MessageNotification w;

    w.show();

    if (!w.createConnetion(&dbinfo)) {
      //問題発生。DB読み込み直すようにする事。
    }
    w.run();

    return a.exec();
}
