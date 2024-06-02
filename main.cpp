#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDebug>

class ApiRequester : public QObject {
    Q_OBJECT

public:
    ApiRequester(QObject *parent = nullptr) : QObject(parent), manager(new QNetworkAccessManager(this)) {
        connect(manager, &QNetworkAccessManager::finished, this, &ApiRequester::onReplyFinished);
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ApiRequester::sendRequest);
        timer->start(1000); // 1000 milliseconds = 1 second
    }

    void sendRequest() {
        QNetworkRequest request(QUrl("https://api.tapswap.ai/api/player/submit_taps"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("x-cv", "608");
        request.setRawHeader("Sec-Fetch-Dest", "empty");
        request.setRawHeader("Connection", "keep-alive");
        request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
        request.setRawHeader("Content-Id", "1457764608");
        request.setRawHeader("Sec-Fetch-Site", "cross-site");
        request.setRawHeader("Origin", "https://app.tapswap.club");
        request.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 17_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148");
        request.setRawHeader("Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOjE0NTc3Njg5MzIsImlhdCI6MTcxNzM0MjQwNCwiZXhwIjoxNzE3MzQ2MDA0fQ.bANkLAOios8OK6M15f6phTEZg4HIzYJQyDp1LTm3N7A");
        request.setRawHeader("Sec-Fetch-Mode", "cors");
        request.setRawHeader("x-bot", "no");
        request.setRawHeader("Host", "api.tapswap.ai");
        request.setRawHeader("Referer", "https://app.tapswap.club/");
        request.setRawHeader("x-app", "tapswap_server");
        request.setRawHeader("Accept-Language", "en-GB,en;q=0.9");
        request.setRawHeader("Accept", "*/*");

        QJsonObject json;
        json["taps"] = 1;
        json["time"] = 1717235953601;

        QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

        // Optional: handle errors for individual requests
        connect(reply, &QNetworkReply::errorOccurred, this, &ApiRequester::onErrorOccurred);
    }

private slots:
    void onReplyFinished(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            qDebug() << "Response:" << response;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    }

    void onErrorOccurred(QNetworkReply::NetworkError error) {
        qDebug() << "Network Error:" << error;
    }

private:
    QNetworkAccessManager *manager;
    QTimer *timer;
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ApiRequester requester;

    return a.exec();
}

