// expense.h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;
tm stringToDate(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d.%m.%Y");
     

    // Adjust the year (because %y assumes years since 1900)
    tm.tm_year += (tm.tm_year < 100) ? 100 : 0;  // Adjust for years like 24 to mean 2024
    return tm;
}

class Expense  {


public:
    string date;
    double amount;
    string category;
    int id;
    static string username;
    Expense(string date, double amount, string category)
        : date(date), amount(amount), category(category) {}

    static void setUsername(const string &user) {
        username = user;
    }
    static void calculateTotalAmount( vector<Expense>& expenses, const string& startDate, const string& endDate) {
        Expense::loadExpensesFromCSV(expenses);
        tm startTm = stringToDate(startDate);
        tm endTm = stringToDate(endDate);
        double total = 0.0; 

        for (const auto& expense : expenses) {
            tm expenseDate = stringToDate(expense.date);
            

            if ((mktime(&expenseDate) >= mktime(&startTm)) && (mktime(&expenseDate) <= mktime(&endTm))) {
                total += expense.amount;
            }
        }

        cout<<"your total expense between "<<startDate<<" "<<"and "<<endDate<<" is "<<total<<endl;
    }
    static double forReport3( vector<Expense>& expenses, const string& startDate, const string& endDate) {
        Expense::loadExpensesFromCSV(expenses);
        tm startTm = stringToDate(startDate);
        tm endTm = stringToDate(endDate);
        double total = 0.0; 

        for (const auto& expense : expenses) {
            tm expenseDate = stringToDate(expense.date);
            

            if ((mktime(&expenseDate) >= mktime(&startTm)) && (mktime(&expenseDate) <= mktime(&endTm))) {
                total += expense.amount;
            }
        }

       return total;
    }
    static void addExpense(vector<Expense>& expenses, const string& date, double amount, const string& category) {
        expenses.emplace_back(date, amount, category);
        saveToCsv(date,amount,category);
    }

    static void viewExpenses(vector<Expense>& expenses) {
        Expense::id_giver(expenses);
        // Display table headers
        cout << left << setw(5) << "ID"
            << setw(15) << "Date"
            << setw(15) << "Amount"
            << setw(25) << "Category" << endl;

        // Display a separating line
        cout << string(60, '-') << endl;

        // Display each expense in table format
        for (const auto& expense : expenses) {
            cout << left << setw(5) << expense.id 
                << setw(15) << expense.date 
                << setw(15) << fixed << setprecision(2) << expense.amount 
                << setw(25) << expense.category << endl;
        }
    }

    static void forReport11(vector<Expense>&expenses,tm startDate,tm endDate){
        Expense::loadExpensesFromCSV(expenses);
        
        for (const auto& expense : expenses) {
            tm expenseDate = stringToDate(expense.date);
            if ((mktime(&expenseDate) >= mktime(&startDate)) && (mktime(&expenseDate) <= mktime(&endDate))) {
                cout << left 
                    << setw(30) << expense.date 
                    << setw(30) << fixed << setprecision(2) << expense.amount 
                    << setw(40) << expense.category << endl;
            }
        }
    }

    static void editExpense(vector<Expense>& expenses) {
        int counter = Expense::get_counter();
        ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Expense.csv", ios::trunc);
        if (file.is_open()) {
            file << "Date,Amount,Category\n";
            for(int i = 0 ;i<counter;i++){
                file << expenses[i].date << ',' <<expenses[i].amount << ',' << expenses[i].category<< '\n';
            }
            file.close();
            cout<<"Edited Successfully"<<endl;
        }else{
             cerr << "Unable to open file." << endl;
        }
    }

    static void deleteExpense(vector<Expense>& expenses) {
        int counter = Expense::get_counter()-1;
        ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Expense.csv", ios::trunc);
        if (file.is_open()) {
        
            file << "Date,Amount,Category\n";
            for(int i = 0 ;i<counter;i++){
                file << expenses[i].date << ',' <<expenses[i].amount << ',' << expenses[i].category<< '\n';
                    
            }
        
            file.close();
        }else{
            cerr << "Unable to open file." << endl;
        }
        Expense::id_giver(expenses);
        cout<<"Deleted Successfully"<<endl;
    }

    static double getTotalExpenses(vector<Expense>& expenses,string str) {
        int counter = Expense::get_counter();
        Expense::loadExpensesFromCSV(expenses);
        double total_expense = 0.0;
        for(int i =0;i<counter;i++){
            //total_expense+=expenses[i].amount;
            if(expenses[i].category == str){
                total_expense+=expenses[i].amount;
            }
            
        }
        return total_expense;
        
    }


    static void loadExpensesFromCSV(vector<Expense>& expenses) {
        expenses.clear();
        ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_Expense.csv");
        

        string line;
        getline(file, line); // Skip the header line

        while (getline(file, line)) {
            string date, amountStr, category;
            stringstream ss(line);
            getline(ss, date, ',');
            getline(ss, amountStr, ',');
            getline(ss, category, ',');

            double amount = stod(amountStr);
            expenses.emplace_back(date, amount, category);
        }

        file.close();
    }

    static void saveExpensesToCSV(vector<Expense>& expenses) {
        ofstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_Expense.csv", ios::app);
        if (file.is_open()) {
            int counter = Expense::get_counter();
            if(counter == 0){
                file << expenses[0].date << ',' << expenses[0].amount << ',' << expenses[0].category << '\n';
            }else{
                for(int i = 0 ;i<counter;i++){
                    file << expenses[i].date << ',' << expenses[i].amount << ',' << expenses[i].category << '\n';
                    
                }
            }
                
            file.close();
        } else {
            std::cerr << "Unable to open file for writing: " << username + "/expense.csv" << std::endl;
        }
    }
    static void saveToCsv(string date,double amount,string category){//careful no &
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Expense.csv",ios::app);
            if(file.is_open()){
                file << date << ',' << amount << ',' << category<< '\n';
                file.close();
            } else {
                std::cerr << "Unable to open file for writing: " << username + "/expense.csv" << std::endl;
            }
    }
    static void id_giver(vector<Expense>&expenses){
            int counter = Expense::get_counter();
            Expense::loadExpensesFromCSV(expenses);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                expenses[i].id = idd;
                idd++;
                
            }

    }

    static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Expense.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
    }
};


string Expense::username;
