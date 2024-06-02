#include <QCoreApplication>
#include "apirequester.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ApiRequester requester;

    return a.exec();
}
