#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include "apirequester.h"

ApiRequester::ApiRequester(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this)) {
    // Set up the periodic request
    handleLogin();
    connect(&timer, &QTimer::timeout, this, &ApiRequester::sendRequest);
    timer.start(1000); // every 1 second
}

void ApiRequester::sendRequest() {
    QNetworkRequest request(QUrl("https://api.tapswap.ai/api/player/submit_taps"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("x-cv", "621");
    request.setRawHeader("Sec-Fetch-Dest", "empty");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("Content-Id", "1457722168");
    request.setRawHeader("Sec-Fetch-Site", "cross-site");
    request.setRawHeader("Origin", "https://app.tapswap.club");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 17_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148");
    request.setRawHeader("Authorization", bearerToken.toUtf8());
    request.setRawHeader("Sec-Fetch-Mode", "cors");
    request.setRawHeader("x-bot", "no");
    request.setRawHeader("Host", "api.tapswap.ai");
    request.setRawHeader("Referer", "https://app.tapswap.club/");
    request.setRawHeader("x-app", "tapswap_server");
    request.setRawHeader("Accept-Language", "en-GB,en;q=0.9");
    request.setRawHeader("Accept", "*/*");

    QJsonObject json;
    json["taps"] = 2;
    json["time"] = 1717766092867;

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, &ApiRequester::onReplyFinished);
}


void ApiRequester::onReplyFinished() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response, &parseError);

        if (parseError.error == QJsonParseError::NoError && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject.contains("player") && jsonObject["player"].isObject()) {
                QJsonObject playerObject = jsonObject["player"].toObject();
                if (playerObject.contains("shares")) {
                    int shares = playerObject["shares"].toInt();
                    QString currentTime = QDateTime::currentDateTime().toString("HH:mm:ss");
                    qDebug() << "__________________________________________";
                    qDebug() << "Current Time:" << currentTime;
                    qDebug() << "Shares:" << shares;
                    qDebug() << "------------------------------------------";
                    qDebug() << " ";
                }
                if (playerObject.contains("energy")) {
                    int energy = playerObject["energy"].toInt();
                    if (energy < 100) {
                        QThread::sleep(30);
                    }
                }
            }
        } else {
            qDebug() << "JSON Parse Error:" << parseError.errorString();
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
        handleLogin();
    }
    reply->deleteLater();
}

void ApiRequester::handleLogin() {
    QNetworkRequest request(QUrl("https://api.tapswap.ai/api/account/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("x-cv", "621");
    request.setRawHeader("Sec-Fetch-Dest", "empty");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Sec-Fetch-Site", "cross-site");
    request.setRawHeader("Origin", "https://app.tapswap.club");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 17_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148");
    request.setRawHeader("Sec-Fetch-Mode", "cors");
    request.setRawHeader("x-bot", "no");
    request.setRawHeader("Host", "api.tapswap.ai");
    request.setRawHeader("x-app", "tapswap_server");
    request.setRawHeader("Referer", "https://app.tapswap.club/");
    request.setRawHeader("Accept-Language", "en-GB,en;q=0.9");
    request.setRawHeader("Accept", "*/*");

    QJsonObject json;
    json["init_data"] = "query_id=AAHkyeNWAAAAAOTJ41ZkR7XB&user=%7B%22id%22%3A1457768932%2C%22first_name%22%3A%22%C3%97%CD%9C%C3%972.0kiwy%22%2C%22last_name%22%3A%22%22%2C%22username%22%3A%22kiwymonzy%22%2C%22language_code%22%3A%22en%22%2C%22allows_write_to_pm%22%3Atrue%7D&auth_date=1717766083&hash=aa84cfc69d19b37054bffaa3660f10c9167313a7efbb9e18c5cf7811a5266173";
    json["referrer"] = "";
    json["bot_key"] = "app_bot_0";
    json["chr"] = 40996; // Assuming 'chr' is a key in the JSON object

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, &ApiRequester::onLoginReplyFinished);
}


void ApiRequester::onLoginReplyFinished() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response, &parseError);

        if (parseError.error == QJsonParseError::NoError && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << "GOOD";
            if (jsonObject.contains("access_token")) {
                QString accessToken = jsonObject["access_token"].toString();
                bearerToken = "Bearer " + accessToken;
                qDebug() << "New Access Token:" << bearerToken;
            }
        } else {
            qDebug() << "JSON Parse Error:" << parseError.errorString();
        }
    } else {
        qDebug() << "Error: HERE" << reply->errorString();
    }
    reply->deleteLater();
}
