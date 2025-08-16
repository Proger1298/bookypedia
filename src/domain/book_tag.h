#pragma once

#include <optional>
#include <string>

#include "book.h"

namespace domain {

class BookTag {
public:
    BookTag(BookId book_id, const std::string& tag)
    : book_id_(std::move(book_id)), tag_(std::move(tag)) 
    {

    }

    const BookId& GetBookId() const noexcept {
        return book_id_;
    }

    const std::string& GetTag() const noexcept {
        return tag_;
    }

private:
    BookId book_id_;
    std::string tag_;
};

class BookTagRepository {
public:
    virtual void Save(const BookTag& book_tag) = 0;
    virtual void DeleteByBookId(const BookId& book_id) = 0;

    virtual std::vector<BookTag> GetBookTags(const BookId& book_id) const = 0;

protected:
    ~BookTagRepository() = default;
};

}  // namespace domain