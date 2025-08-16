#pragma once

#include <memory>

#include "../domain/author_fwd.h"
#include "../domain/book_fwd.h"
#include "../domain/book_tag_fwd.h"

namespace app {

class UnitOfWork {
public:
    virtual void Commit() = 0;
    virtual domain::AuthorRepository& GetAuthorRepository() = 0;
    virtual domain::BookRepository& GetBookRepository() = 0;
    virtual domain::BookTagRepository& GetBookTagRepository() = 0;

public:
  ~UnitOfWork() = default;
};

class UnitOfWorkFactory {
public:
    virtual std::unique_ptr<UnitOfWork> CreateUnitOfWork() = 0;

protected:
    ~UnitOfWorkFactory() = default;
};

} // namespace app