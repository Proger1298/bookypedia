#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../domain/author_fwd.h"
#include "../domain/book_fwd.h"
#include "../domain/book_tag_fwd.h"

namespace app {

using namespace domain;

class UseCases {
public:
    // // // --- AUTHOR --- // // //

    virtual void AddAuthor(const std::string& name) = 0;
    virtual bool EditAuthor(const AuthorId& id, const std::string& new_name) = 0;
    virtual bool DeleteAuthor(const AuthorId& id) = 0;
    
    virtual std::optional<domain::Author> GetAuthorByName(const std::string& name) const = 0;
    virtual std::optional<domain::Author> GetAuthorById(const AuthorId& id) const = 0;

    virtual std::vector<domain::Author> GetAllAuthors() const = 0;

    // // // --- AUTHOR --- // // //
    //
    //
    //
    // // // --- BOOK --- // // //
    
    virtual void AddBookByAuthorId(
        const domain::AuthorId& author_id,
        const std::string& title,
        int publication_year,
        const std::vector<std::string>& tags
    ) = 0;
    virtual void AddBookByAuthorName(
        const std::string& author_name,
        const std::string &title,
        int publication_year,
        const std::vector<std::string> &tags
    ) = 0;
    virtual bool EditBook(
        const BookId& id,
        const std::string& title, 
        int publication_year,
        const std::vector<std::string>& tags
    ) = 0;
    virtual bool DeleteBook(const BookId& id) = 0;

    virtual std::optional<domain::Book> GetBook(const BookId& id) const = 0;
    virtual std::vector<domain::Book> GetBooksByTitle(const std::string& title) const = 0;
    virtual std::vector<domain::Book> GetAllBooks() const = 0;
    virtual std::vector<domain::Book> GetBooksByAuthorId(const domain::AuthorId& author_id) const = 0;

    // // // --- BOOK --- // // //

protected:
    ~UseCases() = default;
};

}  // namespace app
