#include <bits/stdc++.h>

using namespace std;

class Author
{
private :
    char authorID[31];
    char authorName[51];
    char authorAddress[51];
    char authorMobile[12];
    struct indexRec
    {
        char authorID[31];
        long long offset ;
    };
    struct secIndexRec
    {
        char authorName[51];
        int rf ;
    };
    struct secListRec
    {
        char authorID[31];
        int rf ;
        int next ;
    };
    vector<indexRec> index;
    vector<secIndexRec> secIndex;
    vector<secListRec> secList;
public :
    void add ();
    void print();
    void printByName();
    void delet();
    int binarySearch(char key[]);
    void loadIndex();
    void saveIndex();
    void sortIndex();
    void sortSecIndex();
    void initializeList();
    int secBinarySearch(char []);
};
void Author::add()
{
    fstream file ;
    file.open("Authors.txt",ios::app|ios::out|ios::binary);
    cout << "Enter Author ID : "; //primary key
    cin >> authorID;
    cout << "Enter Author Name : "; //sec. key
    cin >>  authorName;
    cout <<  "Enter Author Address : " ;
    cin >> authorAddress;
    cout << "Enter Author Mobile : ";
    cin >> authorMobile;
    int found = binarySearch(authorID);
    if(found!=-1)
    {
        cout <<"Operation Failed, Author exists in the file!!\n";
    }
    else
    {
        short len = strlen(authorID)+strlen(authorName)+strlen(authorAddress)+strlen(authorMobile)+4;
        file.seekp(0,ios::end);
        int adress = file.tellp();
        file.write((char*)&len,sizeof(len));
        file.put(' ');
        file.write((char*)&authorID,strlen(authorID));
        file.put('|');
        file.write((char*)&authorName,strlen(authorName));
        file.put('|');
        file.write((char*)&authorAddress,strlen(authorAddress));
        file.put('|');
        file.write((char*)&authorMobile,strlen(authorMobile));
        file.put('|');
        indexRec temp ;
        strcpy(temp.authorID,authorID);
        temp.offset=adress;
        index.push_back(temp);
        sortIndex();
        initializeList();
        int foundAt = secBinarySearch(authorName);
        if(foundAt == -1)
        {
            secIndexRec temp2;
            strcpy(temp2.authorName,authorName);
            temp2.rf = binarySearch(authorID);
            secIndex.push_back(temp2);
        }
        else
        {
            int rf = secIndex[foundAt].rf;
            while(1)

            {
                if(secList[rf].next == -1)
                {
                    secList[rf].next=  binarySearch(authorID);
                    break;
                }
                else rf = secList[rf].next;
            }
        }
        sortSecIndex();
        saveIndex();
        file.close();
    }
}
void Author::sortIndex()
{
    int len = index.size()-1;
    indexRec temp;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (strcmp(index[j].authorID,index[j + 1].authorID)>0)
            {
                temp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = temp;
            }
        }
}
void Author::sortSecIndex()
{
    int len = secIndex.size()-1;
    secIndexRec temp;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (strcmp(secIndex[j].authorName,secIndex[j + 1].authorName)>0)
            {
                temp = secIndex[j];
                secIndex[j] = secIndex[j + 1];
                secIndex[j + 1] = temp;
            }
        }
}
void Author::initializeList()
{
    vector<secListRec>tempSLR;
    for(int i =  0; i<index.size(); i++)
    {
        secListRec temp ;
        strcpy(temp.authorID, index[i].authorID);
        temp.next = -1;
        temp.rf = i;
        tempSLR.push_back(temp);
    }
    for(int i = 0 ; i < secList.size(); i++)
    {
        for(int j = 0 ; j < tempSLR.size(); j++)
        {
            if(strcmp(secList[i].authorID,tempSLR[j].authorID)==0)
            {
                tempSLR[j].next=secList[i].next;
                if(i!=j)
                {
                    for(int k = 0 ; k < tempSLR.size(); k++)
                    {
                        if(tempSLR[k].next==i)
                            tempSLR[k].next=j;
                    }
                }
            }
        }
    }
    secList = tempSLR ;
}
int Author::binarySearch(char key[])
{
    int sz = index.size();
    int low = 0, high = sz - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (strcmp(index[middle].authorID, key) == 0)
            return middle;
        else if (strcmp(index[middle].authorID,key) < 0)
            low = middle + 1;
        else
            high = middle - 1;
    }
    return-1;
}
int Author::secBinarySearch(char key[])
{
    int sz = secIndex.size();
    int low = 0, high = sz - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (strcmp(secIndex[middle].authorName, key) == 0)
            return middle;
        else if (strcmp(secIndex[middle].authorName,key) < 0)
            low = middle + 1;
        else
            high = middle - 1;
    }
    return-1;
}
void Author::delet()
{
    char AID[31];
    cout << "Enter Author ID to delete : ";
    cin >> AID;
    fstream file("Authors.txt", ios::binary | ios::out | ios::in);
    int address = binarySearch(AID);
    if (address == -1)
    {
        cout << "Author not found\n";
        return;
    }
    file.seekp(index[address].offset +2, ios::beg);
    file << '*';
    string temp;
    for (int i = address; i<index.size()-1; i++)
        index[i] = index[i + 1];
    index.pop_back();
    saveIndex();
    file.close();
}
void Author::print()
{
    char AID[31];
    cout << "Enter the author id you want to display : ";
    cin >> AID ;
    ifstream file;
    file.open("Authors.txt", ios::binary | ios::in);
    int address = binarySearch(AID);
    if (address == -1)
    {
        cout << "this author does not exists\n";
        return;
    }
    short len;
    file.seekg(index[address].offset, ios::beg);
    file.read((char*)&len,sizeof(len));
    file.seekg(1,ios::cur);
    file.getline(authorID,30,'|');
    file.getline(authorName,50,'|');
    file.getline(authorAddress,50,'|');
    file.getline(authorMobile,11,'|');
    cout <<"Author ID : "<<authorID<<"\nAuthor name : " <<authorName<<"\nAuthor address : " <<authorAddress <<"\nAuthor mobile : "<<authorMobile<<endl;
    file.close();
}

void Author::printByName()
{
//Suspended
    fstream file ;
    file.open("Authors.txt",ios::in);
    char AName [51];
    cin >> AName;
    int found = secBinarySearch(AName);
    if(found==-1)cout <<"No authors by this name\n";
    else
    {
        int rf = secIndex[found].rf;
        int i = 1;
        while(1)
        {
            int pos = secList[rf].rf;
            if(binarySearch(index[pos].authorID)!=-1)
            {
                file.seekg(index[pos].offset,ios::beg);
                short len;
                file.read((char*)&len,sizeof(len));
                file.seekg(1,ios::cur);
                file.getline(authorID,30,'|');
                file.getline(authorName,50,'|');
                file.getline(authorAddress,50,'|');
                file.getline(authorMobile,11,'|');
                cout <<"Author "<<i++<<" ID : "<<authorID<<"\nAuthor name : " <<authorName<<"\nAuthor address : " <<authorAddress <<"\nAuthor mobile : "<<authorMobile<<endl;
            }
            if(secList[rf].next == -1)break;
            else rf = secList[rf].next;
        }
        if(i==1)cout <<"No authors by this name\n";
    }
    file.close();
}
void Author::loadIndex()
{
    ifstream file;
    file.open("AuthorsIndex.txt",ios::in);
    while(true)
    {
        indexRec temp;
        file.read((char*)&temp, sizeof(temp));
        if (file.eof())
            break;
        index.push_back(temp);
    }
    file.close();
    file.open("secAuthorsIndex.txt",ios::in);
    while(true)
    {
        secIndexRec temp;
        file.read((char*)&temp,sizeof(temp));
        if(file.eof())break;
        secIndex.push_back(temp);
    }
    file.close();
    file.open("secAuthorsList.txt",ios::in);
    while(true)
    {
        secListRec temp;
        file.read((char*)&temp,sizeof(temp));
        if(file.eof())break;
        secList.push_back(temp);
    }
    file.close();
}
void Author::saveIndex()
{
    ofstream file("AuthorsIndex.txt", ios::trunc);
    for (int i = 0; i<index.size(); i++)
    {
        indexRec temp = index[i];
        file.write((char*)&temp, sizeof(temp));
    }
    file.close();
    file.open("secAuthorsIndex.txt",ios::trunc);
    for(int i = 0 ; i<secIndex.size(); i++)
    {
        secIndexRec temp = secIndex[i];
        file.write((char*)&temp,sizeof(temp));
    }
    file.close();
    file.open("secAuthorsList.txt",ios::trunc);
    for(int i = 0 ; i<secList.size(); i++)
    {
        secListRec temp = secList[i];
        file.write((char*)&temp,sizeof(temp));
    }
    file.close();
}
//------------------------------------------------------------------------------------------\\

class Book
{
private :
    char bookID[14];
    char authorID[31];
    char bookTitle[51];
    float bookPrice;
    //int next ;
    struct indexRec
    {
        char bookID[14];
        long long offset ;
    };
    struct secIndexRec
    {
        char authorID[31];
        int rf ;
    };
    struct secListRec
    {
        char bookID[14];
        int rf ;
        int next ;
    };
    vector<indexRec> index;
    vector<secIndexRec> secIndex;
    vector<secListRec> secList;
public :
    void add ();
    void print();
    void printByAuthor();
    void delet(char[]);
    int binarySearch(char []);
    void loadIndex();
    void saveIndex();
    void sortIndex();
    void sortSecIndex();
    void initializeList();
    int secBinarySearch(char []);
    vector<string> getByAuthor(char[]);
};
vector<string>Book::getByAuthor(char AID[31])
{
    fstream file ;
    file.open("Books.txt",ios::in);
    vector<string> vec;
    int found = secBinarySearch(AID);
    int rf = secIndex[found].rf;
    int i = 1;
    while(1)
    {
        int pos = secList[rf].rf;
        if(binarySearch(index[pos].bookID)!=-1)
        {
            file.seekg(index[pos].offset,ios::beg);
            short len;
            file.read((char*)&len,sizeof(len));
            file.seekg(1,ios::cur);
            file.getline(bookID,13,'|');
            vec.push_back(bookID);
        }
        if(secList[rf].next == -1)break;
        else rf = secList[rf].next;
    }
    return vec;
    file.close();
}
void Book::add()
{
    fstream file ;
    file.open("Books.txt",ios::app|ios::out|ios::binary);
    cout << "Enter Book ID : "; //primary key
    cin >> bookID;
    cout << "Enter Author ID : "; //sec. key
    cin >>  authorID;
    cout <<  "Enter Book Title : " ;
    cin >> bookTitle;
    cout << "Enter Book Price : ";
    cin >> bookPrice;
    int found = binarySearch(authorID);
    Author tempAuthor;
    tempAuthor.loadIndex();
    int authorFound = tempAuthor.binarySearch(authorID);
    if(found!=-1)
    {
        cout <<"Operation Failed, Book exists !!\n";
    }
    else if(authorFound == -1)
    {
        cout <<"Author not found please add the author first!!\n";
    }
    else
    {
        short len = strlen(bookID)+strlen(authorID)+strlen(bookTitle)+sizeof(bookPrice)+3;
        file.seekp(0,ios::end);
        int adress = file.tellp();
        file.write((char*)&len,sizeof(len));
        file.put(' ');
        file.write((char*)&bookID,strlen(bookID));
        file.put('|');
        file.write((char*)&authorID,strlen(authorID));
        file.put('|');
        file.write((char*)&bookTitle,strlen(bookTitle));
        file.put('|');
        file.write((char*)&bookPrice,sizeof(bookPrice));
        indexRec temp ;
        strcpy(temp.bookID,bookID);
        temp.offset=adress;
        index.push_back(temp);
        sortIndex();
        initializeList();
        int foundAt = secBinarySearch(authorID);
        if(foundAt == -1)
        {
            secIndexRec temp2;
            strcpy(temp2.authorID,authorID);
            temp2.rf = binarySearch(bookID);
            secIndex.push_back(temp2);
        }
        else
        {
            int rf = secIndex[foundAt].rf;
            while(1)

            {
                if(secList[rf].next == -1)
                {
                    secList[rf].next=  binarySearch(bookID);
                    break;
                }
                else rf = secList[rf].next;
            }
        }
    }
    sortSecIndex();
    saveIndex();
    file.close();
}
void Book::sortIndex()
{
    int len = index.size()-1;
    indexRec temp;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (strcmp(index[j].bookID,index[j + 1].bookID)>0)
            {
                temp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = temp;
            }
        }
}
void Book::sortSecIndex()
{
    int len = secIndex.size()-1;
    secIndexRec temp;
    for (int i = 0; i<len; i++)
        for (int j = 0; j<len - i; j++)
        {
            if (strcmp(secIndex[j].authorID,secIndex[j + 1].authorID)>0)
            {
                temp = secIndex[j];
                secIndex[j] = secIndex[j + 1];
                secIndex[j + 1] = temp;
            }
        }
}
void Book::initializeList()
{
    vector<secListRec>tempSLR;
    for(int i =  0; i<index.size(); i++)
    {
        secListRec temp ;
        strcpy(temp.bookID, index[i].bookID);
        temp.next = -1;
        temp.rf = i;
        tempSLR.push_back(temp);
    }
    for(int i = 0 ; i < secList.size(); i++)
    {
        for(int j = 0 ; j < tempSLR.size(); j++)
        {
            if(strcmp(secList[i].bookID,tempSLR[j].bookID)==0)
            {
                tempSLR[j].next=secList[i].next;
                if(i!=j)
                {
                    for(int k = 0 ; k < tempSLR.size(); k++)
                    {
                        if(tempSLR[k].next==i)
                            tempSLR[k].next=j;
                    }
                }
            }
        }
    }
    secList = tempSLR ;
}
int Book::secBinarySearch(char key[])
{
    int sz = secIndex.size();
    int low = 0, high = sz - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (strcmp(secIndex[middle].authorID, key) == 0)
            return middle;
        else if (strcmp(secIndex[middle].authorID,key) < 0)
            low = middle + 1;
        else
            high = middle - 1;
    }
    return-1;
}
int Book::binarySearch(char key[])
{
    int sz = index.size();
    int low = 0, high = sz - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (strcmp(index[middle].bookID, key) == 0)
            return middle;
        else if (strcmp(index[middle].bookID,key) < 0)
            low = middle + 1;
        else
            high = middle - 1;
    }
    return-1;
}
void Book::delet(char B[14] = "01234567891111")
{
    char BID[14];
    if(B[0]=='0')
    {
        cout << "Enter Book ID to delete : ";
        cin >> BID;
    }
    else
        strcpy(BID,B);
    fstream file("Books.txt", ios::binary | ios::out | ios::in);
    int address = binarySearch(BID);
    if (address == -1)
    {
        cout << "Book not found\n";
        return;
    }
    file.seekp(index[address].offset +2, ios::beg);
    file << '*';
    for (int i = address; i<index.size()-1; i++) //shift up to delete from index
        index[i] = index[i + 1];
    index.pop_back();
    saveIndex();
    file.close();
}
void Book::print()
{
    char BID[14];
    cout << "Enter the book id you want to display : ";
    cin >> BID ;
    ifstream file;
    file.open("Books.txt", ios::binary | ios::in);

    int address = binarySearch(BID);
    if (address == -1)
    {
        cout << "this book does not exists\n";
        return;
    }
    short len;
    file.seekg(index[address].offset, ios::beg);
    file.read((char*)&len,sizeof(len));
    file.seekg(1,ios::cur);
    file.getline(bookID,13,'|');
    file.getline(authorID,30,'|');
    Author tempAuthor;
    tempAuthor.loadIndex();
    if(tempAuthor.binarySearch(authorID)==-1)
    {
        cout << "Book not found\n";
        delet(bookID);
        return;
    }
    file.getline(bookTitle,50,'|');
    file.read((char*)&bookPrice,sizeof(bookPrice));
    cout <<"Book ID : "<<bookID<<"\nAuthor ID : " <<authorID <<"\nBook title : " <<bookTitle <<"\nBook price : "<<bookPrice<<endl;
    file.close();
}
void Book::printByAuthor()
{
    fstream file ;
    file.open("Books.txt",ios::in);
    char AID [31];
    cin >> AID;
    Author tempAuthor;
    tempAuthor.loadIndex();
    if(tempAuthor.binarySearch(AID)==-1)
    {
        cout << "No books for this author id\n";
        return;
    }
    int found = secBinarySearch(AID);
    if(found==-1)cout <<"No books for this author id\n";
    else
    {
        int rf = secIndex[found].rf;
        int i = 1;
        while(1)
        {
            int pos = secList[rf].rf;
            if(binarySearch(index[pos].bookID)!=-1)
            {
                file.seekg(index[pos].offset,ios::beg);
                short len;
                file.read((char*)&len,sizeof(len));
                file.seekg(1,ios::cur);
                file.getline(bookID,13,'|');
                file.getline(authorID,30,'|');
                file.getline(bookTitle,50,'|');
                file.read((char*)&bookPrice,sizeof(bookPrice));
                cout <<"Book "<<i++<<" ID : "<<bookID<<"\nAuthor ID : " <<authorID <<"\nBook title : " <<bookTitle <<"\nBook price : "<<bookPrice<<endl;
            }
            if(secList[rf].next == -1)break;
            else rf = secList[rf].next;
        }
        if(i==1)cout <<"No books for this author\n";
    }

    file.close();
}
void Book::loadIndex()
{
    ifstream file;
    file.open("BooksIndex.txt",ios::in);
    while(true)
    {
        indexRec temp;
        file.read((char*)&temp, sizeof(temp));
        if (file.eof())
            break;
        index.push_back(temp);
    }
    file.close();
    file.open("secBooksIndex.txt",ios::in);
    while(true)
    {
        secIndexRec temp;
        file.read((char*)&temp,sizeof(temp));
        if(file.eof())break;
        secIndex.push_back(temp);
    }
    file.close();
    file.open("secList.txt",ios::in);
    while(true)
    {
        secListRec temp;
        file.read((char*)&temp,sizeof(temp));
        if(file.eof())break;
        secList.push_back(temp);
    }
    file.close();
}
void Book::saveIndex()
{
    ofstream file("BooksIndex.txt", ios::trunc);
    for (int i = 0; i<index.size(); i++)
    {
        indexRec temp = index[i];
        file.write((char*)&temp, sizeof(temp));
    }
    file.close();
    file.open("secBooksIndex.txt",ios::trunc);
    for(int i = 0 ; i<secIndex.size(); i++)
    {
        secIndexRec temp = secIndex[i];
        file.write((char*)&temp,sizeof(temp));
    }
    file.close();
    file.open("secList.txt",ios::trunc);
    for(int i = 0 ; i<secList.size(); i++)
    {
        secListRec temp = secList[i];
        file.write((char*)&temp,sizeof(temp));
    }
    file.close();
}
int main()
{
    cout << "1) Add New Book\n";
    cout << "2) Add New Author\n";
    cout << "3) Delete Book by ID\n";
    cout << "4) Delete Author by ID\n";
    cout << "5) find Book by ID\n";
    cout << "6) find Book(s) by Author_ID\n";
    cout << "7) find Author by ID\n";
    cout << "8) find Author(s) by name\n";
    cout << "9) Exit\n";
    int choice;
    Author author;
    author.loadIndex();
    Book book;
    book.loadIndex();
    do
    {
        cout << "What do you want to do ? ";
        cin >>choice ;
        switch (choice)
        {
        case 1 :
            book.add();
            break;
        case 2:
            author.add();
            break ;
        case 3 :
            book.delet();
            break;
        case 4 :
            author.delet();
            break ;
        case 5 :
            book.print();
            break;
        case 6 :
            cout << "Enter Author ID : ";
            book.printByAuthor();
            break ;
        case 7 :
            author.print();
            break ;
        case 8 :
            cout << "Enter Author Name : ";
            author.printByName();
            break ;
        case 9 :
            book.saveIndex();
            author.saveIndex();
            break;
        default :
            cout << "INVALID INPUT! Try again : ";
            break;
        }
    }
    while(choice != 9);
    return 0;
}
