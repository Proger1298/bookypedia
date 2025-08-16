#pragma once

#include <pqxx/pqxx>
#include <vector>

#include "../domain/author.h"
#include "../domain/book.h"
#include "../domain/book_tag.h"

#include "../app/unit_of_work.h"

namespace postgres {

using namespace domain;

class AuthorRepositoryImpl : public AuthorRepository {
public:
    explicit AuthorRepositoryImpl(pqxx::work& work)
    : work_{work}
    {

    }

    void Save(const Author& author) override;
    void Edit(const AuthorId& id, const std::string& new_name) override;
    void Delete(const AuthorId& id) override;

    std::vector<Author> GetAllAuthors() const override;
    std::optional<Author> GetAuthorByName(const std::string& name) const override;
    std::optional<Author> GetAuthorById(const AuthorId& id) const override;

private:
    pqxx::work& work_;

    Author GetAuthorFromRow(const pqxx::row& row) const;
};

class BookRepositoryImpl : public domain::BookRepository {
public:
    explicit BookRepositoryImpl(pqxx::work& work)
    : work_{work}
    {
        
    }

    void Save(const Book& book) override;
    void Edit(const BookId& id, const std::string& title, int publication_year) override;
    void Delete(const BookId& id) override;

    std::optional<Book> GetBookById(const BookId& id) override;
    std::vector<Book> GetBooksByTitle(const std::string& title) override;
    std::vector<Book> GetAllBooks() override;
    std::vector<Book> GetBooksByAuthorId(const AuthorId& author_id) const override;

    void DeleteBooksByAuthorId(const AuthorId& author_id) override;

private:
    pqxx::work& work_;

    Book GetBookFromRow(const pqxx::row& row, bool with_author_name) const;
};

class BookTagRepositoryImpl : public BookTagRepository {
public:
    explicit BookTagRepositoryImpl(pqxx::work &work) 
    : work_{work}
    {

    }
    
    void Save(const BookTag& book_tag) override;
    void DeleteByBookId(const BookId& book_id) override;

    std::vector<BookTag> GetBookTags(const BookId& book_id) const override;

private:
    pqxx::work& work_;
};

class UnitOfWorkImpl : public app::UnitOfWork {
public:
    explicit UnitOfWorkImpl(pqxx::connection& connection) 
    : work_(connection)
    {

    }
    
    void Commit() override {
        work_.commit();
    }

    domain::AuthorRepository& GetAuthorRepository() override {
        return authors_;
    }

    domain::BookRepository& GetBookRepository() override {
        return books_;
    }

    domain::BookTagRepository& GetBookTagRepository() override {
        return book_tags_;
    }

private:
    pqxx::work work_;
    AuthorRepositoryImpl authors_{work_};
    BookRepositoryImpl books_{work_};
    BookTagRepositoryImpl book_tags_{work_};
};

class UnitOfWorkFactoryImpl : public app::UnitOfWorkFactory {
public:
    explicit UnitOfWorkFactoryImpl(pqxx::connection &connection)
    : connection_(connection)
    {

    }
    
    std::unique_ptr<app::UnitOfWork> CreateUnitOfWork() override {
        return std::make_unique<UnitOfWorkImpl>(connection_);
    }

private:
    pqxx::connection &connection_;
};

class Database {
public:
    explicit Database(pqxx::connection connection);

    UnitOfWorkFactoryImpl& GetUnitOfWorkFactoryFactory() {
        return uow_factory_;
    }

private:
    pqxx::connection connection_;
    UnitOfWorkFactoryImpl uow_factory_{connection_};
};

}  // namespace postgres