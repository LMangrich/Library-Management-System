#include <iostream>
#include <limits>
#include <stdlib.h>
#define TMAX 100
using namespace std;

typedef struct
{
    string enrollment, ISBN, data;
    int lendCode;
} Student;

typedef struct
{
    string title, author, ISBN;
    int copyQuantity, lendQuantity;
} Book;

void menu(int opcao, int &numberOfBooks, Book book[], int &numberOfCode, Student student[]);
void registerBook(Book &book);
bool bookCheck(int numberOfBooks, Book book[]);
void sortISBN(int numberOfBooks, Book book[]);
void bookConsult(int numberOfBooks, Book book[]);
void deleteBook(int &numberOfBooks, Book book[]);
void borrowInformation(Student student[]);
void bookDevolution(int numberOfBooks, Book book[], int numberOfCode, Student student[]);
void borrowBook(int &numberOfBooks, Book book[], int &numberOfCode, Student student[]);
void reportAllBooks(int numberOfBooks, Book book[]);
void reportActiveLoans(int numberOfBooks, Book book[], int numberOfCode, Student student[]);

void registerBook(Book &book)
{
    cin.ignore();
    do
    {
        cout << "Name of the book: " << endl;
        getline(cin, book.title);
    }
    while (book.title.size() == 0);
    do
    {
        cout << "Author of the book: " << endl;
        getline(cin, book.author);
    }
    while (book.author.size() == 0);
    do
    {
        cout << "ISBN - 13 digits: " << endl;
        getline(cin, book.ISBN);
    }
    while (book.ISBN.length() != 13 || book.ISBN.find_first_not_of("0123456789") != string::npos);
    do
    {
        cout << "How many copies does the book have? " << endl;
        cin >> book.copyQuantity;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number: " << endl;
            cin >> book.copyQuantity;
        }
    }
    while (book.copyQuantity < 0);
    book.lendQuantity = book.copyQuantity;
}

bool bookCheck(int numberOfBooks, Book book[])
{
    for (int i = 0; i < numberOfBooks - 1; i++)
    {
        for (int j = i + 1; j < numberOfBooks; j++)
        {
            if (book[i].ISBN == book[j].ISBN)
                return true;

        }
    }
    return false;
}

void sortISBN(int numberOfBooks, Book book[])
{
    for (int i = 0; i < numberOfBooks - 1; i++)
    {
        Book aux;
        int minimun = i;
        for (int j = i + 1; j < numberOfBooks; j++)
            if (book[j].ISBN < book[minimun].ISBN)
                minimun = j;

        aux = book[i];
        book[i] = book[minimun];
        book[minimun] = aux;
    }
}

void bookConsult(int numberOfBooks, Book book[])
{
    string isbn;
    bool found = false;
    cin.ignore();
    do
    {
        cout << "Which book would you like to consult (ISBN - 13 digits)?" << endl;
        getline(cin, isbn);
    }
    while (isbn.length() != 13 || isbn.find_first_not_of("0123456789") != string::npos);
    for (int i = 0; i < numberOfBooks; i++)
    {
        if (isbn == book[i].ISBN)
        {
            cout << "The title of the book is: " <<book[i].title << endl
                 << "The author of the book is: " << book[i].author << endl;
            cout << "The book has " << book[i].copyQuantity << " copies" << endl;
            cout << "The book has " << book[i].lendQuantity << " copies available to lend";
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Title inexistent";
    }
}

void deleteBook (int &numberOfBooks, Book book[])
{
    string isbn;
    bool found = false;
    cin.ignore();
    do
    {
        cout << "Which book would you like to delete (ISBN - 13 digits) ?" << endl;
        getline(cin, isbn);
    }
    while (isbn.length() != 13 || isbn.find_first_not_of("0123456789") != string::npos);
    for (int i = 0; i < numberOfBooks; i++)
    {
        if (isbn == book[i].ISBN)
        {
            found = true;
            if (i == numberOfBooks - 1)
            {
                numberOfBooks--;
                cout << "Deletion performed";
                break;
            }
            else
            {
                book[i] = book[numberOfBooks - 1];
                numberOfBooks--;
                sortISBN(numberOfBooks, book);
                cout << "Deletion performed";
                break;
            }
        }
    }
    if (!found)
        cout << "Title inexistent";
}

void borrowInformation (Student &student)
{
    do
    {
        cout << "What is your enrollment? " << endl;
        getline(cin, student.enrollment);
    }
    while (student.enrollment.length() != 3 || student.enrollment.find_first_not_of("0123456789") != string::npos);
    do
    {
        cout << "What is the devolution data?" << endl;
        getline(cin, student.data);
    }
    while (student.data.length() < 8 || student.data.find_first_not_of("0123456789/") != string::npos);
}

void borrowBook(int &numberOfBooks, Book book[], int &numberOfCode, Student student[])
{
    bool found = false;
    string isbn;
    cin.ignore();
    do
    {
        cout << "Which book would you like to borrow? (ISBN - 13 digits) " << endl;
        getline(cin, isbn);
    }
    while (isbn.length() != 13 || isbn.find_first_not_of("0123456789") != string::npos);

    for (int i = 0; i < numberOfBooks; i++)
    {
        if (isbn == book[i].ISBN)
        {
            found = true;
            if (book[i].lendQuantity > 0)
            {
                borrowInformation(student[numberOfCode]);
                student[numberOfCode].ISBN = book[i].ISBN;
                book[i].lendQuantity -= 1;
                student[numberOfCode].lendCode = numberOfCode;
                cout << "Borrowing completed - Code -> " << numberOfCode;
                numberOfCode++;
            }
            else
            {
                cout << "Copy unavailable for borrowing";
            }
            break;
        }
    }
    if (!found)
    {
        cout << "Title inexistent";
    }
}

void bookDevolution(int numberOfBooks, Book book[], int numberOfCode, Student student[])
{
    int code;
    bool found = false;
    cout << "What is the loan code? " << endl;
    cin >> code;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number: " << endl;
        cin >> code;
    }
    for (int i = 0; i < numberOfCode; i++)
    {
        if (code == i)
        {
            found = true;
            if (student[i].enrollment == "-1")
            {
                cout << "Return already completed";
                break;
            }
            else
            {
                for (int y = 0; y < numberOfBooks; y++)
                {
                    if (book[y].ISBN == student[i].ISBN)
                    {
                        book[y].lendQuantity += 1;
                        break;
                    }
                }
                student[i].enrollment = "-1";
                cout << "Return processed";
                break;
            }
        }
    }
    if (!found)
    {
        cout << "Inexistent code " << endl;
    }
}

void reportAllBooks(int numberOfBooks, Book book[])
{
    for (int i = 0; i < numberOfBooks; i++)
    {
        cout << "Book " << i + 1 << " :" << endl;
        cout << "Title: " << book[i].title << endl
             << "Author : " << book[i].author << endl;
        cout << "ISBN : " << book[i].ISBN << endl;
        cout << "The book has " << book[i].copyQuantity << " copies" << endl;
        cout << "The book has " << book[i].lendQuantity << " copies to borrow" << endl;
    }
    system("pause");
}

void reportActiveLoans(int numberOfBooks, Book book[], int numberOfCode, Student student[])
{
    for (int i = 0; i < numberOfBooks; i++)
    {
        if (book[i].lendQuantity < book[i].copyQuantity)
        {
            cout << "-----------------------------------------------------------------------------------------" << endl;
            cout << "Book lended : " << endl;
            cout << "Title: " << book[i].title << endl
                 << "Author: " << book[i].author << endl;
            cout << "ISBN: " << book[i].ISBN << endl;
            cout << "The book has " << book[i].copyQuantity << " copies" << endl;
            cout << "The book has " << book[i].lendQuantity << " copies to borrow" << endl;
            for (int y = 0; y < numberOfCode; y++)
            {
                if (student[y].ISBN == book[i].ISBN && student[y].lendCode != -1)
                {
                    cout << "-----------------------------------------------------------------------------------------" << endl;
                    cout << "Lend code: " << student[y].lendCode << endl;
                    cout << "Student enrollment: " << student[y].enrollment << endl
                         << "Devolution data: " << student[y].data << endl;
                }
            }
        }
    }
    system("pause");
}


void menu(int option, int &numberOfBooks, Book book[], int &numberOfCode, Student student[])
{
    switch (option)
    {
    case 1:
        registerBook(book[numberOfBooks]);
        if (bookCheck(numberOfBooks, book) == true)
        {
            cout << "Title already exists" << endl;
        }
        else
        {
            cout << "Title inclusion confirmed" << endl;
            numberOfBooks++;
            sortISBN(numberOfBooks, book);
        }
        break;
    case 2:
        bookConsult(numberOfBooks, book);
        break;
    case 3:
        deleteBook(numberOfBooks, book);
        break;
    case 4:
        borrowBook(numberOfBooks, book, numberOfCode, student);
        break;
    case 5:
        bookDevolution(numberOfBooks, book, numberOfCode, student);
        break;
    case 6:
        reportAllBooks(numberOfBooks, book);
        break;
    case 7:
        reportActiveLoans(numberOfBooks, book, numberOfCode, student);
        break;
    case 8:
        break;
    }
}

int main()
{
    Student student[TMAX];
    Book book[TMAX];
    int option;
    int numberOfBooks = 0, numberOfCode = 0;
    do
    {
        do
        {
            cout << "Choose one of the options: " << endl;
            cout << "1 - Register a new book/title for its collection " << endl
                 << "2 - Consult a new book" << endl;
            cout << "3 - Delete a book from its collection " << endl
                 << "4 - Loan a copy " << endl;
            cout << "5 - Devolution of a copy " << endl
                 << "6 - Report (collection books) " << endl
                 << "7 - Report (active loans) " << endl
                 << "8 - Leave " << endl;
            cin >> option;
        }
        while (option < 1 || option > 8);
        system("cls");

        menu(option, numberOfBooks, book, numberOfCode, student);

        cin.ignore();
        system("cls");
    }
    while(option != 8);
    return 0;
}
