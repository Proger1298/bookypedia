#pragma once

#include "../util/tagged_uuid.h"

namespace domain {

class Book;

class BookRepository;

namespace detail {
struct BookTag {};
}  // namespace detail

using BookId = util::TaggedUUID<detail::BookTag>;

}  // namespace domain