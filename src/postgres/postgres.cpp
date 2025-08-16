#include <algorithm>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "postgres.h"

namespace postgres {

using namespace std::literals;
using pqxx::operator"" _zv;

// // // --- AUTHOR --- // // // --- AUTHOR --- // // // --- AUTHOR --- // // //

Author AuthorRepositoryImpl::GetAuthorFromRow(const pqxx::row &row) const {
    return Author{AuthorId::FromString(row.at("id"s).as<std::string>()), row.at("name"s).as<std::string>()};
}

void AuthorRepositoryImpl::Save(const domain::Author& author) {
    work_.exec_params(
        R"(
            INSERT INTO authors (id, name) VALUES ($1, $2)
            ON CONFLICT (id) DO UPDATE SET name=$2;
        )"_zv,
        author.GetId().ToString(), 
        author.GetName()
    );
}

void AuthorRepositoryImpl::Edit(const AuthorId& id, const std::string& new_name) {
    work_.exec_params(R"(UPDATE authors SET name=$2 WHERE id=$1;)", id.ToString(), new_name);
}

void AuthorRepositoryImpl::Delete(const AuthorId& id) {
    work_.exec_params(R"(DELETE FROM authors WHERE id=$1;)"_zv, id.ToString());
}

std::vector<Author> AuthorRepositoryImpl::GetAllAuthors() const {
    pqxx::result result = work_.exec(R"(SELECT * FROM authors ORDER BY name;)"_zv);

    std::vector<Author> authors;
    authors.reserve(result.size());
    for (const pqxx::row& row : result) {
        authors.emplace_back(GetAuthorFromRow(row));
    }
    return authors;
}

std::optional<Author> AuthorRepositoryImpl::GetAuthorByName(const std::string& name) const {
    try {
        const pqxx::row& row = work_.exec_params1(R"(SELECT * FROM authors WHERE name=$1;)"_zv, name);
        return GetAuthorFromRow(row);
    } catch (pqxx::unexpected_rows &) {
        return std::nullopt;
    }
}

std::optional<Author> AuthorRepositoryImpl::GetAuthorById(const AuthorId& id) const {
    try {
        const pqxx::row& row = work_.exec_params1(R"(SELECT * FROM authors WHERE id=$1;)"_zv, id.ToString());
        return GetAuthorFromRow(row);
    } catch (pqxx::unexpected_rows &) {
        return std::nullopt;
    }
}

// // // --- AUTHOR --- // // // --- AUTHOR --- // // // --- AUTHOR --- // // //
//
//
//
// // // --- BOOK --- // // // --- BOOK --- // // // --- BOOK --- // // //

Book BookRepositoryImpl::GetBookFromRow(const pqxx::row& row, bool with_author_name) const {
    
    if (with_author_name) {
        return Book{
            BookId::FromString(row.at("book_id"s).as<std::string>()),
            AuthorId::FromString(row.at("author_id"s).as<std::string>()),
            row.at("title"s).as<std::string>(), 
            row.at("publication_year"s).as<int>(),
            row.at("name").as<std::string>()
        };
    }
    return Book{
        BookId::FromString(row.at("book_id"s).as<std::string>()),
        AuthorId::FromString(row.at("author_id"s).as<std::string>()),
        row.at("title"s).as<std::string>(), 
        row.at("publication_year"s).as<int>()
    };
}

void BookRepositoryImpl::Save(const Book& book) {
    work_.exec_params(
        R"(
            INSERT INTO books (id, author_id, title, publication_year) VALUES ($1, $2, $3, $4)
            ON CONFLICT (id) DO UPDATE SET author_id=$2, title=$3, publication_year=$4;
        )"_zv,
        book.GetId().ToString(),
        book.GetAuthorId().ToString(),
        book.GetTitle(),
        book.GetPublicationYear()
    );
}

void BookRepositoryImpl::Edit(const BookId& id, const std::string& title, int publication_year) {
    work_.exec_params(
        R"(
            UPDATE books
            SET title=$2, publication_year=$3
            WHERE id=$1;
        )"_zv,
        id.ToString(),
        title, publication_year
    );
}

void BookRepositoryImpl::Delete(const BookId& id) {
    work_.exec_params(R"(DELETE FROM books WHERE id=$1;)"_zv, id.ToString());
}

std::optional<Book> BookRepositoryImpl::GetBookById(const BookId& id) {
    try {
        const pqxx::row& row = work_.exec_params1(
            R"(
                SELECT 
                    books.id AS book_id,
                    author_id,
                    authors.name AS name,
                    title,
                    publication_year
                FROM books
                INNER JOIN authors ON authors.id = author_id
                WHERE books.id=$1;
			)"_zv,
            id.ToString()
        );
        return GetBookFromRow(row, true);
    } catch (pqxx::unexpected_rows &) {
        return std::nullopt;
    }
}

std::vector<Book> BookRepositoryImpl::GetBooksByTitle(const std::string& title) {
    pqxx::result result = work_.exec_params(
        R"(
            SELECT 
                id AS book_id,
                author_id,
                title,
                publication_year
            FROM books
            WHERE title=$1;
		)"_zv,
        title
    );
    
    std::vector<domain::Book> books;
    books.reserve(result.size());
    for (const pqxx::row& row : result) {
        books.emplace_back(GetBookFromRow(row, false));
    }
    return books;
}

std::vector<Book> BookRepositoryImpl::GetAllBooks() {
    pqxx::result result = work_.exec_params(
        R"(
            SELECT
                books.id AS book_id,
                author_id,
                authors.name AS name,
                title,
                publication_year
            FROM books
            INNER JOIN authors ON authors.id = author_id
            ORDER BY title, name, publication_year;
		)"_zv
    );
    std::vector<domain::Book> books;
    books.reserve(result.size());
    for (const pqxx::row& row : result) {
        books.emplace_back(GetBookFromRow(row, true));
    }
    return books;
}

std::vector<Book> BookRepositoryImpl::GetBooksByAuthorId(const AuthorId& author_id) const {
    pqxx::result result = work_.exec_params(
        R"(
            SELECT
                id AS book_id,
                author_id,
                title,
                publication_year
            FROM books
            WHERE author_id=$1
            ORDER BY publication_year, title;
		)"_zv,
        author_id.ToString()
    );
    std::vector<domain::Book> books;
    books.reserve(result.size());
    for (const pqxx::row& row : result) {
        books.emplace_back(GetBookFromRow(row, false));
    }
    return books;
}

void BookRepositoryImpl::DeleteBooksByAuthorId(const AuthorId& author_id) {
    work_.exec_params(R"(DELETE FROM books WHERE author_id=$1;)"_zv, author_id.ToString());
}

// // // --- BOOK --- // // // --- BOOK --- // // // --- BOOK --- // // //
//
//
//
// // // --- BOOK_TAG --- // // // --- BOOK_TAG --- // // // --- BOOK_TAG --- // // //

void BookTagRepositoryImpl::Save(const BookTag& book_tag) {
    work_.exec_params(
        R"(
            INSERT INTO book_tags (book_id, tag) VALUES ($1, $2);
        )"_zv,
        book_tag.GetBookId().ToString(),
        book_tag.GetTag()
    );
}

void BookTagRepositoryImpl::DeleteByBookId(const BookId& book_id) {
    work_.exec_params(R"(DELETE FROM book_tags WHERE book_id=$1;)"_zv, book_id.ToString());
}

std::vector<BookTag> BookTagRepositoryImpl::GetBookTags(const BookId& book_id) const {
    auto result = work_.exec_params(
        R"(
            SELECT * FROM book_tags
            WHERE book_id=$1
            ORDER BY tag;
		)"_zv,
        book_id.ToString()
    );
    std::vector<BookTag> book_tags;
    book_tags.reserve(result.size());
    for (const pqxx::row& row : result) {
        book_tags.emplace_back(BookTag{
            domain::BookId::FromString(row.at("book_id"s).as<std::string>()),
            row.at("tag"s).as<std::string>()
        });
    }
    return book_tags;
}

// // // --- BOOK_TAG --- // // // --- BOOK_TAG --- // // // --- BOOK_TAG --- // // //

Database::Database(pqxx::connection connection)
    : connection_{std::move(connection)} {
    pqxx::work work{connection_};
    
    // Создаем таблицу авторов
    work.exec(R"(
        CREATE TABLE IF NOT EXISTS authors (
            id UUID CONSTRAINT author_id_constraint PRIMARY KEY,
            name varchar(100) UNIQUE NOT NULL
        );
    )"_zv);

    // Создаем таблицу кнги
    work.exec(R"(
        CREATE TABLE IF NOT EXISTS books (
            id UUID CONSTRAINT book_id_constraint PRIMARY KEY,
            author_id UUID NOT NULL REFERENCES authors(id),
            title varchar(100) NOT NULL,
            publication_year integer NOT NULL
        );
    )"_zv);

    // Создаем таблицу книжных тегов
    work.exec(R"(
        CREATE TABLE IF NOT EXISTS book_tags (
            book_id UUID REFERENCES books (id),
            tag varchar(30) NOT NULL
        );
    )"_zv);

    // коммитим изменения
    work.commit();
}

}  // namespace postgres