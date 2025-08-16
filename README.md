# 📚 Bookypedia - Book Management System

## Описание проекта

Bookypedia — это консольное приложение для управления базой данных книг с использованием PostgreSQL.<br> 
Программа позволяет добавлять, редактировать, удалять и просматривать книги и авторов, а также добавлять теги к книгам для удобного поиска.<br>

_Проект разрабатывался для освоения:_

- Паттернов "Репозиторий" и "Unit of Work";
- Работы с базой данных, в том числе понятия транзакции;

_Основные возможности:_
- Добавление, редактирование и удаление авторов
- Добавление, редактирование и удаление книг
- Присвоение тегов книгам
- Поиск книг по названию, автору или тегам

_Используемые паттерны и технологии:_
- Паттерны "Репозиторий" и "Unit of Work"
- C++20
- PostgreSQL (через libpqxx)
- Многопоточность и транзакции
- CMake для сборки

## Инструкция по развёртыванию и системные требования

Для ведения таблицы должен быть развернут PostgreSQL, например, так:
```
sudo docker run -d --name postgres-container -p 30432:5432 -e TZ=UTC -e POSTGRES_PASSWORD=Mys3Cr3t ubuntu/postgres:14-22.04_beta
```
А также создана база данных (например, bookypedia)

Для запуска локально:
```
mkdir build
cd build
conan install .. --build=missing -s build_type=Debug -s compiler.libcxx=libstdc++11
cmake -DCMAKE_BUILD_TYPE=Debug(Release) ..
cmake --build .
BOOKYPEDIA_DB_URL=postgres://postgres:Mys3Cr3t@172.17.0.2:5432/bookypedia ./bookypedia
```

_Системные требоания_:
- Linux (Ubuntu 22.04)

_Зависимости_:
- [С++20](https://en.cppreference.com/w/cpp/20)
- [PostgreSQL](https://www.postgresql.org/) 15+ version requires
- [libpqxx](https://github.com/jtv/libpqxx)
- [Boost](https://www.boost.org/users/history/version_1_78_0.html) 1.78+ version requires
- [GCC](https://gcc.gnu.org/) 11+ version requires
- [CMake](https://cmake.org) 3.18 version requires
- [Conan](https://conan.io/) 1.* version requires