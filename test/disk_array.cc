#include "catch.hpp"
#include "seri/object.h"
#include "seri/array.h"
#include "parser.h"
#include "model/json_print.h"
#include <boost/filesystem.hpp>

using namespace cheesebase;

TEST_CASE("array") {
  boost::filesystem::remove("test.db");
  Database db("test.db");

  SECTION("insert") {
    auto val = parseJson(R"( [ 1, 2, 3, { "A": "a", "B": "b" }, 5 ])");
    Addr root;
    {
      auto ta = db.startTransaction();
      disk::ObjectW tree(ta);
      root = tree.addr();
      tree.insert(ta.key("arr"), val, disk::Overwrite::Upsert);
      ta.commit(tree.getWrites());
    }
    {
      disk::ObjectR tree(db, root);
      auto read = tree.getChildValue("arr");
      REQUIRE(val == read);
    }
  }

  SECTION("append") {
    Addr root;
    {
      auto ta = db.startTransaction();
      disk::ArrayW arr(ta);
      root = arr.addr();
      ta.commit(arr.getWrites());
    }
    {
      auto ta = db.startTransaction();
      disk::ArrayW arr(ta, root);
      auto i1 = arr.append(model::Value(std::string("a")));
      auto i2 = arr.append(model::Value(std::string("b")));
      auto i3 = arr.append(model::Value(std::string("c")));
      REQUIRE(i1 == Key(0));
      REQUIRE(i2 == Key(1));
      REQUIRE(i3 == Key(2));
      ta.commit(arr.getWrites());
    }
    {
      disk::ArrayR arr(db, root);
      auto read = arr.getValue();
      auto val = parseJson(R"( [ "a", "b", "c" ] )");
      REQUIRE(val == read);
    }
    {
      auto ta = db.startTransaction();
      disk::ArrayW arr(ta, root);
      arr.remove(Key(1));
      arr.insert(Key(4), model::Value(true), disk::Overwrite::Insert);
      arr.append(model::Value(false));
      ta.commit(arr.getWrites());
    }
    {
      disk::ArrayR arr(db, root);
      auto read = arr.getValue();
      auto val = parseJson(R"( [ "a", missing, "c", missing, true, false ] )");
      REQUIRE(val == read);
    }
  }
}
