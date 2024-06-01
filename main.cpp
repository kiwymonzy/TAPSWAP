#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QDateTime>

class TapswapClient : public QObject
{
    Q_OBJECT

public:
    explicit TapswapClient(QObject *parent = nullptr);

public slots:
    void sendRequest();
    void handleResponse();

private:
    QNetworkAccessManager *manager;
};

TapswapClient::TapswapClient(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    // Setup the timer
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TapswapClient::sendRequest);
    timer->start(1000); // Send request every second
}

void TapswapClient::sendRequest()
{
    QUrl url("https://api.tapswap.ai/api/player/submit_taps");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("x-cv", "608");
    request.setRawHeader("Sec-Fetch-Dest", "empty");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("Content-Id", "1457764608");
    request.setRawHeader("Sec-Fetch-Site", "cross-site");
    request.setRawHeader("Origin", "https://app.tapswap.club");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 17_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148");
    request.setRawHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOjE0NTc3Njg5MzIsImlhdCI6MTcxNzIzNTg3NiwiZXhwIjoxNzE3MjM5NDc2fQ.LL7NkEvDvyNxoCmDP4DLCtE73-rVzmAsxyq7wUa4Ukw");
    request.setRawHeader("Sec-Fetch-Mode", "cors");
    request.setRawHeader("x-bot", "no");
    request.setRawHeader("Host", "api.tapswap.ai");
    request.setRawHeader("Referer", "https://app.tapswap.club/");
    request.setRawHeader("x-app", "tapswap_server");
    request.setRawHeader("Accept-Language", "en-GB,en;q=0.9");
    request.setRawHeader("Accept", "*/*");

    QJsonObject json;
    json["taps"] = 10;
    json["time"] = 1717235953601;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &TapswapClient::handleResponse);
}

void TapswapClient::handleResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();
        if (jsonObject.contains("stat")) {
            QJsonObject statObject = jsonObject["stat"].toObject();
            if (statObject.contains("taps")) {
                int taps = statObject["taps"].toInt();
                QString currentTime = QDateTime::currentDateTime().toString("HH:mm:ss");
                qDebug() << currentTime << "Taps:" << taps;
            }
        }
    } else {
        qWarning() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TapswapClient client;

    return a.exec();
}

#include "main.moc"
