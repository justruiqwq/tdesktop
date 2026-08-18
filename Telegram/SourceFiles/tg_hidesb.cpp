#include "tg_hidesb.h"

#include "core/application.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

namespace TgHideSb {
namespace {

bool Loaded = false;
QSet<quint64> Ids;

[[nodiscard]] QString FilePath() {
	return cWorkingDir() + u"tdata/tg_hidesb.txt"_q;
}

void Load() {
	Ids.clear();
	auto file = QFile(FilePath());
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		auto stream = QTextStream(&file);
		while (!stream.atEnd()) {
			const auto line = stream.readLine().trimmed();
			if (line.isEmpty()) {
				continue;
			}
			auto ok = false;
			const auto id = line.toULongLong(&ok);
			if (ok && id) {
				Ids.insert(id);
			}
		}
	}
	Loaded = true;
}

void Save() {
	QDir().mkpath(cWorkingDir() + u"tdata"_q);
	auto file = QFile(FilePath());
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		auto stream = QTextStream(&file);
		for (const auto id : Ids) {
			stream << id << '\n';
		}
	}
}

} // namespace

void Ensure() {
	if (!Loaded) {
		Load();
	}
}

bool IsHidden(quint64 bareId) {
	if (!bareId) {
		return false;
	}
	Ensure();
	return Ids.contains(bareId);
}

void Add(quint64 bareId) {
	if (!bareId) {
		return;
	}
	Ensure();
	if (!Ids.contains(bareId)) {
		Ids.insert(bareId);
		Save();
	}
}

void Remove(quint64 bareId) {
	Ensure();
	if (Ids.remove(bareId)) {
		Save();
	}
}

QSet<quint64> All() {
	Ensure();
	return Ids;
}

} // namespace TgHideSb
