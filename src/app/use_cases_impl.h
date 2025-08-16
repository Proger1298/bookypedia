#pragma once

#include "use_cases.h"
#include "unit_of_work.h"

namespace app {

class UseCasesImpl : public UseCases {
public:
    explicit UseCasesImpl(UnitOfWorkFactory &unit_of_work_factory)
    : unit_of_work_factory_{unit_of_work_factory}
    {

    }

    // // // --- AUTHOR --- // // //

    void AddAuthor(const std::string& name) override;
    bool EditAuthor(const AuthorId& id, const std::string& new_name) override;
    bool DeleteAuthor(const AuthorId& id) override;
    
    std::optional<domain::Author> GetAuthorByName(const std::string& name) const override;
    std::optional<domain::Author> GetAuthorById(const AuthorId& id) const override;

    std::vector<domain::Author> GetAllAuthors() const override;

    // // // --- AUTHOR --- // // //
    //
    //
    //
    // // // --- BOOK --- // // //
    
    void AddBookByAuthorId(
        const domain::AuthorId& author_id,
        const std::string& title,
        int publication_year,
        const std::vector<std::string>& tags
    ) override;
    void AddBookByAuthorName(
        const std::string& author_name,
        const std::string &title,
        int publication_year,
        const std::vector<std::string> &tags
    ) override;
    bool EditBook(
        const BookId& id,
        const std::string& title, 
        int publication_year,
        const std::vector<std::string>& tags
    ) override;
    bool DeleteBook(const BookId& id) override;

    std::optional<domain::Book> GetBook(const BookId& id) const override;
    std::vector<domain::Book> GetBooksByTitle(const std::string& title) const override;
    std::vector<domain::Book> GetAllBooks() const override;
    std::vector<domain::Book> GetBooksByAuthorId(const domain::AuthorId& author_id) const override;

private:
    UnitOfWorkFactory& unit_of_work_factory_;
};

}  // namespace app
