// Licensed under the Apache License 2.0 (see LICENSE file).
#pragma once

#include "../model/model.h"
#include "../types.h"
#include "value.h"

namespace cheesebase {
namespace disk {

class StringW : public ValueW {
public:
  virtual ~StringW() = default;

  StringW(Transaction& ta, model::String str);
  StringW(Transaction& ta, Addr addr);
  Writes getWrites() const override;
  void destroy() override;

private:
  model::String str_;
  std::vector<Block> blocks_;
};

class StringR : public ValueR {
public:
  virtual ~StringR() = default;

  StringR(Database& db, Addr addr);
  model::Value getValue() override;
};

} // namespace disk
} // namespace cheesebase
