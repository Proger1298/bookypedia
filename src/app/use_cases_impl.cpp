#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

#include "use_cases_impl.h"

#include "../domain/author.h"
#include "../domain/book.h"
#include "../domain/book_tag.h"

namespace app {

using namespace domain;

// // // --- AUTHOR --- // // //

void UseCasesImpl::AddAuthor(const std::string& name) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    uow_transaction->GetAuthorRepository().Save(Author{AuthorId::New(), name});
    uow_transaction->Commit();
}

bool UseCasesImpl::EditAuthor(const AuthorId& id, const std::string& new_name) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::optional<Author> author = uow_transaction->GetAuthorRepository().GetAuthorById(id);
    if (author.has_value()) {
        uow_transaction->GetAuthorRepository().Edit(author->GetId(), new_name);
        uow_transaction->Commit();
        return true;
    }
    uow_transaction->Commit();
    return false;
}

bool UseCasesImpl::DeleteAuthor(const AuthorId& id) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::optional<Author> author = uow_transaction->GetAuthorRepository().GetAuthorById(id);
    if (author.has_value()) {
        for (const Book& book : uow_transaction->GetBookRepository().GetBooksByAuthorId(author->GetId())) {
            uow_transaction->GetBookTagRepository().DeleteByBookId(book.GetId());
        }
        uow_transaction->GetBookRepository().DeleteBooksByAuthorId(author->GetId());
        uow_transaction->GetAuthorRepository().Delete(author->GetId());
        uow_transaction->Commit();
        return true;
    }
    uow_transaction->Commit();
    return false;
}
    
std::optional<domain::Author> UseCasesImpl::GetAuthorByName(const std::string& name) const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::optional<Author> author = uow_transaction->GetAuthorRepository().GetAuthorByName(name);
    uow_transaction->Commit();
    return author;
}

std::optional<domain::Author> UseCasesImpl::GetAuthorById(const AuthorId& id) const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::optional<Author> author = uow_transaction->GetAuthorRepository().GetAuthorById(id);
    uow_transaction->Commit();
    return author;
}

std::vector<domain::Author> UseCasesImpl::GetAllAuthors() const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::vector<domain::Author>& authors = uow_transaction->GetAuthorRepository().GetAllAuthors();
    uow_transaction->Commit();
    return authors;
}

// // // --- AUTHOR --- // // //
//
//
//
// // // --- BOOK --- // // //

void UseCasesImpl::AddBookByAuthorId(
    const domain::AuthorId& author_id,
    const std::string& title,
    int publication_year,
    const std::vector<std::string>& tags
) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    BookId book_id = BookId::New();
    uow_transaction->GetBookRepository().Save(Book{book_id, author_id, title, publication_year});
    for (const std::string& tag : tags) {
        uow_transaction->GetBookTagRepository().Save(BookTag{book_id, tag});
    }
    uow_transaction->Commit();
}

void UseCasesImpl::AddBookByAuthorName(
    const std::string& author_name,
    const std::string &title,
    int publication_year,
    const std::vector<std::string> &tags
) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    AuthorId author_id = AuthorId::New();
    uow_transaction->GetAuthorRepository().Save(Author{author_id, author_name});

    BookId book_id = BookId::New();
    uow_transaction->GetBookRepository().Save(Book{book_id, author_id, title, publication_year});

    for (const std::string& tag : tags) {
        uow_transaction->GetBookTagRepository().Save(BookTag{book_id, tag});
    }
    uow_transaction->Commit();
}

bool UseCasesImpl::EditBook(
    const BookId& id,
    const std::string& title, 
    int publication_year,
    const std::vector<std::string>& tags
) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::optional<Book> book = uow_transaction->GetBookRepository().GetBookById(id);
    if (book.has_value()) {
        uow_transaction->GetBookRepository().Edit(book->GetId(), title, publication_year);
        const std::vector<BookTag>& book_tags = uow_transaction->GetBookTagRepository().GetBookTags(book->GetId());
        if (!book_tags.empty()) {
            uow_transaction->GetBookTagRepository().DeleteByBookId(book->GetId());
        }
        for (const std::string& tag : tags) {
            uow_transaction->GetBookTagRepository().Save(BookTag{book->GetId(), tag});
        }
        uow_transaction->Commit();
        return true;
    }
    uow_transaction->Commit();
    return false;
}

bool UseCasesImpl::DeleteBook(const BookId& id) {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    const std::optional<Book> book = uow_transaction->GetBookRepository().GetBookById(id);
    if (book.has_value()) {
        uow_transaction->GetBookTagRepository().DeleteByBookId(book->GetId());
        uow_transaction->GetBookRepository().Delete(book->GetId());
        uow_transaction->Commit();
        return true;
    }
    uow_transaction->Commit();
    return false;
}

std::optional<domain::Book> UseCasesImpl::GetBook(const BookId& id) const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    std::optional<Book> book = uow_transaction->GetBookRepository().GetBookById(id);
    if (book.has_value()) {
        std::vector<BookTag> book_tags = uow_transaction->GetBookTagRepository().GetBookTags(id);
        std::vector<std::string> tags;
        tags.reserve(book_tags.size());
        std::transform(
            book_tags.begin(),
            book_tags.end(),
            std::inserter(tags, tags.end()),
            [](const BookTag& book_tag) { return book_tag.GetTag(); }
        );
        book->SetTags(tags);
        uow_transaction->Commit();
        return book;
    }
    uow_transaction->Commit();
    return book;
}

std::vector<domain::Book> UseCasesImpl::GetBooksByTitle(const std::string& title) const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    std::vector<domain::Book> books = uow_transaction->GetBookRepository().GetBooksByTitle(title);
    uow_transaction->Commit();
    return books;
}

std::vector<domain::Book> UseCasesImpl::GetAllBooks() const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    std::vector<domain::Book> books = uow_transaction->GetBookRepository().GetAllBooks();
    uow_transaction->Commit();
    return books;
}

std::vector<domain::Book> UseCasesImpl::GetBooksByAuthorId(const domain::AuthorId& author_id) const {
    std::unique_ptr<UnitOfWork> uow_transaction = unit_of_work_factory_.CreateUnitOfWork();
    std::vector<domain::Book> books = uow_transaction->GetBookRepository().GetBooksByAuthorId(author_id);
    uow_transaction->Commit();
    return books;
}

// // // --- BOOK --- // // //

}  // namespace app
