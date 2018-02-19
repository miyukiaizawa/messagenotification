#include "messagenotification.h"
#include <QtWidgets/QApplication>
#include <filesystem>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  std::string pwd = (std::string)std::tr2::sys::path(argv[0]).parent_path().string();
  QTextCodec* tc = QTextCodec::codecForLocale();
  QString dbname = QString(tc->toUnicode((pwd + "\\Logs.db").c_str()));

  DatabaseInfo dbinfo("QSQLITE", "localhost", dbname, "", "");

  MessageNotification w;

  w.show();

  if (!w.createConnetion(&dbinfo)) {
    //問題発生。DB読み込み直すようにする事。
  }
  w.run();

  return a.exec();
}
