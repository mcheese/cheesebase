// Licensed under the Apache License 2.0 (see LICENSE file).

// Provides memory pages to read and write from. Recently requested pages are
// cached. Writable pages are marked and later written back to disk.

#pragma once

#include "common.h"
#include "sync.h"

#include <unordered_map>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <fstream>

namespace cheesebase {

DEF_EXCEPTION(FileError);

struct CachePage {
  RwMutex mutex;
  gsl::span<Byte> data;
  boost::interprocess::mapped_region region;
  PageNr page_nr{ static_cast<PageNr>(-1) };
  bool changed{ false };
  CachePage* less_recent;
  CachePage* more_recent;
};

enum class OpenMode {
  create_new,    // Creates new DB if it does not exist.
  create_always, // Creates new DB, always. Overwrite existing DB.
  open_existing, // Opens DB if it exists.
  open_always    // Opens DB, always. Creates new DB if it does not exist.
};

// Locked reference of a page.
template <class View, class Lock>
class PageRef {
public:
  PageRef() = default;
  PageRef(View page, Lock lock) : page_(page), lock_(std::move(lock)){};

  MOVE_ONLY(PageRef);

  View get() const { return page_; };
  View operator*() const { return page_; };
  const View* operator->() const { return &page_; };

private:
  const View page_;
  Lock lock_;
};

using ReadRef = PageRef<PageReadView, ShLock<RwMutex>>;
using WriteRef = PageRef<PageWriteView, ExLock<RwMutex>>;

class Cache {
public:
  Cache(const std::string& filename, OpenMode mode, size_t nr_pages);
  ~Cache();

  ReadRef readPage(PageNr page_nr);
  WriteRef writePage(PageNr page_nr);
  void flush();

private:
  // return specific page, creates it if not found
  template <class Lock>
  std::pair<CachePage&, Lock> getPage(PageNr page_nr);

  // return an unused page, may free the least recently used page
  std::pair<CachePage&, ExLock<RwMutex>>
  GetFreePage(const ExLock<RwMutex>& map_lck);

  // mark p as most recently used (move to front of list)
  void bumpPage(CachePage& p, const ExLock<Mutex>& lck);

  // ensure write to disk and remove from map
  void freePage(CachePage& p, const ExLock<RwMutex>& page_lck,
                const ExLock<RwMutex>& map_lck);

  uint64_t extendFile(uint64_t size);

  boost::interprocess::file_mapping file_;
  std::ofstream fstream_;
  uint64_t size_{ 0 };
  Mutex pages_mtx_;
  std::unique_ptr<CachePage[]> pages_;
  CachePage* least_recent_;
  CachePage* most_recent_;

  RwMutex map_mtx_;
  std::unordered_map<PageNr, CachePage&> map_;
};

} // namespace cheesebase