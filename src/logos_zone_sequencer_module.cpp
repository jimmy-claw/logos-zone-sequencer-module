#include "logos_zone_sequencer_module.h"
#include "zone_sequencer.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

LogosZoneSequencerModule::LogosZoneSequencerModule() {}
LogosZoneSequencerModule::~LogosZoneSequencerModule() {}

void LogosZoneSequencerModule::initLogos(LogosAPI* api) {
    logosAPI = api;
    qInfo() << "ZoneSequencer: initLogos called";
    QFile f("/tmp/zone_cpp_debug.txt");
    if (f.open(QIODevice::Append)) { QTextStream(&f) << "initLogos called\n"; }
}

void LogosZoneSequencerModule::set_node_url(const QString& url) {
    m_nodeUrl = url;
    qInfo() << "ZoneSequencer: node URL =" << url;
    QFile f("/tmp/zone_cpp_debug.txt");
    if (f.open(QIODevice::Append)) { QTextStream(&f) << "set_node_url: " << url << "\n"; }
}

void LogosZoneSequencerModule::set_signing_key(const QString& hex) {
    m_signingKey = hex;
    qInfo() << "ZoneSequencer: signing key set, len=" << hex.length();
    QFile f("/tmp/zone_cpp_debug.txt");
    if (f.open(QIODevice::Append)) { QTextStream(&f) << "set_signing_key len=" << hex.length() << "\n"; }
}

QString LogosZoneSequencerModule::publish(const QString& data) {
    QFile f("/tmp/zone_cpp_debug.txt");
    if (f.open(QIODevice::Append)) {
        QTextStream(&f) << "publish called, signingKey.isEmpty=" << m_signingKey.isEmpty()
                        << " len=" << m_signingKey.length() << "\n";
    }
    if (m_signingKey.isEmpty()) {
        return QStringLiteral("Error: signing key not set");
    }
    char* result = zone_publish(
        m_nodeUrl.toUtf8().constData(),
        m_signingKey.toUtf8().constData(),
        data.toUtf8().constData());
    if (!result) {
        return QStringLiteral("Error: zone_publish returned null");
    }
    QString txHash = QString::fromUtf8(result);
    zone_free_string(result);
    return txHash;
}
