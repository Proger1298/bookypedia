#pragma once

#include "../util/tagged_uuid.h"

namespace domain {

class Author;

class AuthorRepository;

namespace detail {
struct AuthorTag {};
}  // namespace detail

using AuthorId = util::TaggedUUID<detail::AuthorTag>;

}  // namespace domain