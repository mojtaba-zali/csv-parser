#include "transaction.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

TransactionList::TransactionList() {}

////////////////////////////////////
// 2.1 parseFile & parseRecord
////////////////////////////////////

Transaction TransactionList::parseRecord(std::string rs)
{
        std::istringstream iss(rs);
        std::string acc_nb{}, fname{}, lname{}, merchant{};
        double amt;
        time_t timestamp;
        std::string token1{}, token2{};

        getline(iss, acc_nb, '|');
        getline(iss, fname, '|');
        getline(iss, lname, '|');
        getline(iss, token1, '|');
        
        amt = std::stod(token1);
        getline(iss, merchant, '|');

        getline(iss, token1, '|');
        getline(iss, token2, '|');
        struct std::tm tm;
        std::istringstream ss(token1 + " " + token2);
        ss >> std::get_time(&tm, "%d.%m.%Y %H:%M:%S");
        timestamp = mktime(&tm);
        Transaction transaction(acc_nb, fname, lname, amt, merchant, timestamp);
        return transaction;
}

void TransactionList::parseFile(std::string filename)
{
    std::ifstream file(filename);
    if(!file)
    {
        throw std::runtime_error("File cannot be opened!");
        exit(-1);
    }
    std::string row{};
    while(std::getline(file, row))
    {
        transactions.push_back(parseRecord(row));
    }    
}

////////////////////////////////////
// 2.2 iterators & other operators
////////////////////////////////////

std::vector<Transaction>::iterator TransactionList::begin()
{ 
    return std::vector<Transaction>::iterator(&transactions.front());
}
std::vector<Transaction>::iterator TransactionList::end()
{ 
    return std::vector<Transaction>::iterator(&transactions.back());
}
std::vector<Transaction>& TransactionList::operator*()
{
    return transactions;
}
std::vector<Transaction>* TransactionList::operator->()
{
    return &transactions;
}

////////////////////////////////////
// 2.3 TransactionRecComp
////////////////////////////////////

bool TransactionRecComp::operator()(Transaction t1, Transaction t2)
{
    if (t1.acc_nb > t2.acc_nb)
        return true;
    return false;
}

////////////////////////////////////
// 2.4 custom IO manipulator
////////////////////////////////////

std::ostream& put_transaction::operator()(std::ostream &os) const
{
    os << std::right << std::setw(15) << "Account Nb:" << std::right << std::setw(50) << t.acc_nb << std::endl;
    os << std::right << std::setw(15) << "First Name:" << std::right << std::setw(50) << t.fname << std::endl;
    os << std::right << std::setw(15) << "Last Name:"  << std::right << std::setw(50) << t.lname << std::endl;

    std::cout.imbue(std::locale("de_DE.UTF-8"));
    os << std::right << std::setw(15) << "Amount:"
       << std::right << std::setw(51) << std::showbase
       << std::put_money(t.amt*100., true) << std::endl;
    
    os << std::right << std::setw(15) << "Merchant:"   << std::right << std::setw(50) << t.merchant << std::endl;

    std::tm * ptm = std::localtime(&t.timestamp);
    char format_time[32];
    std::strftime(format_time, 32, "%d.%m.%y;%H:%M:%S", ptm);
    os << std::right << std::setw(15) << "Date Time:"
       << std::right << std::setw(50) << format_time << std::endl;
    return os;
}
inline std::ostream& operator<<(std::ostream& os, put_transaction pt)
{
    return pt(os);
}

////////////////////////////////////
// 2.5 sort function
////////////////////////////////////

TransactionList TransactionList::sort()
{
    TransactionList sorted = *this;
    std::sort(sorted.begin(), sorted.end(), TransactionRecComp());
    return sorted;
}

int main()
{
    TransactionList tl;
    tl.parseFile("transactions.txt");   
    TransactionList sorted = tl.sort();
    for (auto itr = sorted.begin() ; itr != sorted.end(); ++itr) {
        std::cout << put_transaction(*itr) << std::endl;
    }       
    return 0;
}
