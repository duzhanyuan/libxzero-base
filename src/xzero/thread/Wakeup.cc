// This file is part of the "libxzero" project
//   (c) 2009-2015 Christian Parpart <https://github.com/christianparpart>
//   (c) 2014-2015 Paul Asmuth <https://github.com/paulasmuth>
//
// libxzero is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License v3.0.
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <xzero/thread/Wakeup.h>

namespace xzero {

Wakeup::Wakeup() : gen_(0) {
}

void Wakeup::waitForNextWakeup() {
  waitForWakeup(generation());
}

void Wakeup::waitForFirstWakeup() {
  waitForWakeup(0);
}

void Wakeup::waitForWakeup(long oldgen) {
  std::unique_lock<std::mutex> l(mutex_);

  condvar_.wait(l, [this, oldgen] {
    return gen_.load() > oldgen;
  });
}

void Wakeup::onWakeup(long generation, std::function<void()> callback) {
  std::unique_lock<std::mutex> l(mutex_);

  if (gen_.load() > generation) {
    l.unlock();
    callback();
    return;
  }

  callbacks_.push_back(callback);
}

long Wakeup::generation() const {
  return gen_.load();
}

void Wakeup::wakeup() {
  std::list<std::function<void ()>> callbacks;

  std::unique_lock<std::mutex> l(mutex_);
  gen_++;
  callbacks.splice(callbacks.begin(), callbacks_);
  l.unlock();

  condvar_.notify_all();

  for (const auto& callback: callbacks) {
    callback();
  }
}

} // namespace xzero

