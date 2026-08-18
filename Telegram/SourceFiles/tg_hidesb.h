#pragma once

#include <QtCore/QSet>

namespace TgHideSb {

void Ensure();
[[nodiscard]] bool IsHidden(quint64 bareId);
void Add(quint64 bareId);
void Remove(quint64 bareId);
[[nodiscard]] QSet<quint64> All();

} // namespace TgHideSb
