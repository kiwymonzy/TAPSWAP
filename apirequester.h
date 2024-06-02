#ifndef APIREQUESTER_H
#define APIREQUESTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class ApiRequester : public QObject
{
    Q_OBJECT
public:
    explicit ApiRequester(QObject *parent = nullptr);

public slots:
    void sendRequest();
    void onReplyFinished();
    void onLoginReplyFinished();

private:
    void handleLogin();

    QNetworkAccessManager *manager;
    QTimer timer;
    QString bearerToken;
};

#endif // APIREQUESTER_H
