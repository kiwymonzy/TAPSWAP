#ifndef APIREQUESTER_H
#define APIREQUESTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class ApiRequester : public QObject {
    Q_OBJECT

public:
    ApiRequester(QObject *parent = nullptr);

public slots:
    void sendRequest();

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onErrorOccurred(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *manager;
    QTimer *timer;
};

#endif // APIREQUESTER_H
