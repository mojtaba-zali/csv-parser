#include <chrono>
#include <ctime>
#include <string>
#include <vector>

class Transaction {
public:
  std::string acc_nb;
  std::string fname;
  std::string lname;
  double amt;
  std::string merchant;
  time_t timestamp;
  Transaction(std::string acc_nb, std::string fname, std::string lname,
              double amt, std::string merchant, time_t timestamp)
      : acc_nb(acc_nb), fname(fname), lname(lname), amt(amt),
        merchant(merchant), timestamp(timestamp) {}
};

class TransactionList {
private:
  std::vector<Transaction> transactions;

public:
  TransactionList();
  void parseFile(std::string);
  Transaction parseRecord(std::string);
  std::vector<Transaction>::iterator begin();
  std::vector<Transaction>::iterator end();
  std::vector<Transaction>& operator*();
  std::vector<Transaction>* operator->();
  TransactionList sort();
};

class TransactionRecComp {
public:
  bool operator()(Transaction, Transaction);
};

class put_transaction {
public:
    put_transaction(Transaction t) : t(t) {}
    std::ostream& operator()(std::ostream&) const;
private:
    Transaction t;
};
inline std::ostream& operator<<(std::ostream&, put_transaction);
