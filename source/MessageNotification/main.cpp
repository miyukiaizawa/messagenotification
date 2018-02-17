#include "messagenotification.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseInfo dbinfo("QSQLITE", "localhost", "E:\\DB\\Logs.db", "", "");
    MessageNotification w;

    w.show();

    if (!w.createConnetion(&dbinfo)) {
      //ñ‚ëËî≠ê∂ÅBDBì«Ç›çûÇ›íºÇ∑ÇÊÇ§Ç…Ç∑ÇÈéñÅB
    }
    w.run();

    return a.exec();
}
