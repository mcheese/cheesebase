cmake_minimum_required(VERSION 3.1)

set(SRC
  storage.cc
  cache.cc
  allocator.cc
  block_alloc.cc
  block_locks.cc
  keycache.cc
  murmurhash3.cc
  core.cc
  cheesebase.cc
  parser.cc
  model/model.cc
  query/eval.cc
  query/db_session.cc
  query/eval/expr.cc
  query/eval/sfw.cc
  query/eval/from.cc
  seri/string.cc
  seri/object.cc
  seri/array.cc
  seri/btree/btree.cc
  seri/btree/common.cc
  seri/btree/leaf.cc
  seri/btree/internal.cc
  seri/btree/read.cc
)

add_library(cheesebase ${SRC})
target_link_libraries(cheesebase ${Boost_LIBRARIES})

