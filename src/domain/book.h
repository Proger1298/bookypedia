#pragma once

#include <string>
#include <optional>
#include <vector>

#include "../util/tagged_uuid.h"
#include "book_fwd.h"
#include "author.h"

namespace domain {

using BookId = util::TaggedUUID<detail::BookTag>;

class Book {
public:
    Book(BookId id, AuthorId author_id, std::string title, int publication_year)
    : id_(std::move(id)), author_id_(std::move(author_id)), 
    title_(std::move(title)), publication_year_(publication_year) 
    {

    }

    Book(BookId id, AuthorId author_id, std::string title, int publication_year, const std::string& author_name) 
    : id_(std::move(id)), author_id_(std::move(author_id)), 
    title_(std::move(title)), publication_year_(publication_year), author_name_{std::move(author_name)}
    {

    }

    const BookId& GetId() const noexcept {
        return id_;
    }

    const AuthorId& GetAuthorId() const noexcept {
        return author_id_;
    }

    const std::string& GetTitle() const noexcept {
        return title_;
    }

    int GetPublicationYear() const noexcept {
        return publication_year_;
    }

    std::optional<std::string> GetAuthorName() const noexcept {
        return author_name_;
    }

    const std::vector<std::string>& GetTags() const noexcept {
        return tags_;
    }
    
    void SetTags(const std::vector<std::string>& tags) {
        tags_ = std::move(tags);
    }

private:
    BookId id_;
    AuthorId author_id_;
    std::string title_;
    int publication_year_;
    std::optional<std::string> author_name_;
    std::vector<std::string> tags_;
};

class BookRepository {
public:
    virtual void Save(const Book& book) = 0;
    virtual void Edit(const BookId& id, const std::string& title, int publication_year) = 0;
    virtual void Delete(const BookId& id) = 0;

    virtual std::optional<Book> GetBookById(const BookId& id) = 0;
    virtual std::vector<Book> GetBooksByTitle(const std::string& title) = 0;
    virtual std::vector<Book> GetAllBooks() = 0;
    virtual std::vector<Book> GetBooksByAuthorId(const AuthorId& author_id) const = 0;

    virtual void DeleteBooksByAuthorId(const AuthorId& author_id) = 0;
    
protected:
    ~BookRepository() = default;
};

}  // namespace domain
