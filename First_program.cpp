#include <iostream>
#include <regex>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <sys/stat.h> // For mkdir
#include <sys/types.h> // For mode_t
#include <unistd.h> // For access
#include <cstring> // For strerror
#include <errno.h> // For errno
#include "investment.h"
#include "expense.h"
#include "budget.h"
#include <map>
#include <algorithm>
#include <cctype>
#include <ctime>
#include "net_worth.h"
#include "reminder.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include "recurring.h"
#include "savingGoal.h"
#include "income.h"
vector<Investment>investments;
vector<Expense>expenses;
vector<Income>incomes;
map<string, Budgeting> budgets;
map<string, Budgeting> lowerCaseMap;
vector<Assets>assets;
vector<Liabilities>liabilities;
vector<Reminder>reminders;
vector<Recurring> transactions;
vector <SavingGoal> goals;
using namespace std;
regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
string toLowerCase(const string &str){
    string lowerStr = str;
    transform(lowerStr.begin(),lowerStr.end(),lowerStr.begin(),::tolower);
    return lowerStr;
}
void forLowerCasemap(){
    // Iterate over the original map
    for (const auto& pair : budgets) {
        // Convert the key to lowercase
        string lowerKey = toLowerCase(pair.first);
        
        // Insert the new key-value pair into the lowercase map
        lowerCaseMap[lowerKey] = pair.second;
    }
}

void checkReminders() {
    Reminder::id_giver(reminders);
    time_t now = time(nullptr); // Get the current times
    tm* currentTime = localtime(&now); // Convert to local time structure
    for (auto& reminder : reminders) {
        if (difftime(mktime(currentTime), mktime(&reminder.dueDate)) >= 0) {
            cout << "Reminder: " << reminder.description << " is due!\n";

            if (reminder.isRecurring == "true"||reminder.isRecurring == "True" ) {
                reminder.updateDueDate1();
            }else{
                reminders.erase(remove_if(reminders.begin(),reminders.end(),[&reminder](const Reminder& tx){return tx.id == reminder.id;}),reminders.end());
            }
        }
    }
    Reminder::writeToCSVforUpdating(reminders);
}

void calculateTotalExpense(){
    string startDate,endDate;
    while (true) {
        cout << "Please enter start date (dd.mm.yy): ";
        cin >> startDate;

        // Check if the input matches the date pattern
        if (regex_match(startDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter end date (dd.mm.yy): ";
        cin >> endDate;

        // Check if the input matches the date pattern
        if (regex_match(endDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    
    Expense::calculateTotalAmount(expenses,startDate,endDate);

}

void calculateTotalNetWorth(){
    string startDate,endDate;
    double totalAssets,totalLia,total;
    while (true) {
        cout << "Please enter start date (dd.mm.yy): ";
        cin >> startDate;

        // Check if the input matches the date pattern
        if (regex_match(startDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter end date (dd.mm.yy): ";
        cin >> endDate;

        // Check if the input matches the date pattern
        if (regex_match(endDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    
    totalAssets = Assets::calculateTotalAsset(assets,startDate,endDate);
    totalLia = Liabilities::calculateTotalLiability(liabilities,startDate,endDate);
    total = totalAssets - totalLia;
    cout<<"Your total Networth Between "<<startDate<<" and "<<endDate<<" is "<<total<<endl;
    if(total < 0){
        cout<<"Warning your liabilities are greater than your assets .You better manage your money more wisefully"<<endl;
    }
    

}
void calculateTotalIncome(){
    string startDate,endDate;
    while (true) {
        cout << "Please enter start date (dd.mm.yy): ";
        cin >> startDate;

        // Check if the input matches the date pattern
        if (regex_match(startDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter end date (dd.mm.yy): ";
        cin >> endDate;

        // Check if the input matches the date pattern
        if (regex_match(endDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    
    Income::calculateTotalIncome(incomes,startDate,endDate);

}
void calculateTotalProfit(){
    string startDate,endDate;
    while (true) {
        cout << "Please enter start date (dd.mm.yy): ";
        cin >> startDate;

        // Check if the input matches the date pattern
        if (regex_match(startDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter end date (dd.mm.yy): ";
        cin >> endDate;

        // Check if the input matches the date pattern
        if (regex_match(endDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    Investment::calculateTotalProfit(investments,startDate,endDate);

}
void calculateTotalCurrentValue(){
    string startDate,endDate;
    while (true) {
        cout << "Please enter start date (dd.mm.yy): ";
        cin >> startDate;

        // Check if the input matches the date pattern
        if (regex_match(startDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter end date (dd.mm.yy): ";
        cin >> endDate;

        // Check if the input matches the date pattern
        if (regex_match(endDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    Investment::calculate_total_value(investments,startDate,endDate);

}
void addInvestmentFromUser(vector <Investment>&investments){
    string purchased_date;
    int units;
    string name;
    double purchased_price;
    double current_price;
    
    string username;
    while (true) {
        cout << "Please enter the date of purchase (dd.mm.yy): ";
        cin >> purchased_date;

        // Check if the input matches the date pattern
        if (regex_match(purchased_date, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter units: ";
        cin >> units;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(units<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    cout<<"enter investment name ";cin.ignore();getline(cin,name);
    while (true) {
        cout << "Please enter purchased price: ";
        cin >> purchased_price;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(purchased_price<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    while (true) {
        cout << "Please enter current price: ";
        cin >> current_price;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(current_price<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    Investment::addInvestment(investments,purchased_date,units,name,purchased_price,current_price);
    cout<<"Investment added successfully"<<endl;
                              
}
void addExpenseFromUser(vector<Expense>& expenses) {
    string date, category;
    double amount;
    //double remainingBeforeRemaining;
    while (true) {
        cout << "Please enter the date (dd.mm.yy): ";
        cin >> date;

        // Check if the input matches the date pattern
        if (regex_match(date, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter amount: ";
        cin >> amount;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(amount<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    cout << "Enter category(Note that you can't edit your category once you have set it.You can only delete it): "; cin.ignore(); getline(cin, category);
    string lowerCategory = toLowerCase(category);
    Budgeting::loadFromCSV(budgets);
    forLowerCasemap();

    auto it = lowerCaseMap.find(lowerCategory);

    if (it != lowerCaseMap.end()) {
        // Key found
        it->second.checkAndResetBudget();
        if(amount <= it->second.remainingBalance){
            
            Expense::addExpense(expenses, date, amount, category);
            cout << "Expense added successfully." << endl;
            
            it->second.remainingBalance -= amount;
            if(it->second.remainingBalance <= it->second.limit){
                cout<<"Your are reaching the limit balance your budget is becoming nearer to zero"<<endl;
            }
            
            
            Budgeting::writeToCSVforUpdating(lowerCaseMap);
            
        }else{
            cout<<"Your targeted budget is already zero"<<endl;
        }

    } else {
        // Key not found
        Expense::addExpense(expenses, date, amount, category);
        cout << "Expense added successfully." << endl;
        
    }

    /*Expense::addExpense(expenses, date, amount, category);
    cout << "Expense added successfully." << endl;*/
}
void addIncomeFromUser(vector<Income>& incomes) {
    string date, source;
    double amount;

    while (true) {
        cout << "Please enter the date (dd.mm.yy): ";
        cin >> date;

        // Check if the input matches the date pattern
        if (regex_match(date, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter amount: ";
        cin >> amount;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(amount<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    cout << "Enter source: "; cin.ignore(); getline(cin, source);

    Income::addIncome(incomes, date, amount, source);
    cout << "Income added successfully." << endl;
}
void addBudgetFromUser(map<string, Budgeting>& budgets) {
        string name, type;
        double amt,lmt;
        time_t setDate;

        cout << "Enter Budget Name: ";
        cin.ignore();
        getline(cin, name);
        while (true) {
            cout << "Please enter amount: ";
            cin >> amt;

            // Check if the input is valid
            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Incorrect data. Please enter a valid amount.\n";
            } else {
                if(amt<=0){
                    cout<<"must be greater than 0"<<endl;
                }else{
                    break;
                }
            }
        }
        while (true) {
            cout << "Please enter the limit(we will sent you alert if you reach limit)(limit must be less than amount): ";
            cin >> lmt;

            // Check if the input is valid
            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Incorrect data. Please enter a valid amount.\n";
            } else {
                if(lmt<=0){
                    cout<<"must be greater than 0"<<endl;
                }else if (lmt > amt){
                    cout<<"Your data is nonsense bro yours is higher than your amount "<<endl;
                }else{
                    break;
                }
            }
        }
        while (true) {
            cout << "Enter Budget Type (Monthly/Yearly/None): ";
            cin>>type;

            // Check if the input is valid
            if(type == "Monthly"||type == "Yearly" || type =="None"){
                break;
            }else{
                cout<<"please only enter Monthly or Yearly or None"<<endl;
            }
        }
        
        
        string low = toLowerCase(name);
        
        Budgeting budget;
        
        budget.setBudget(amt, type,lmt);
        budget.checkType();
        

        budgets[low] = budget; // Add the budget to the map
        cout << "Budget added successfully!\n";

        // Write the budget to the CSV file
        Budgeting::writeToCSV(low,budget);
       
}
void addReminderFromUser(){
    string des;
    string dueDate;
    tm due;
    string option;
    int frequency;
    cout<<"Enter the description : ";
    cin.ignore();
    getline(cin,des);
    while (true) {
        cout << "Please enter the Due date (dd.mm.yy): ";
        cin >> dueDate;

        // Check if the input matches the date pattern
        if (regex_match(dueDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while(true){
        cout<<"Is reminder recurring or not (true or false only) : ";
        cin>>option;

        if(option == "true"||option == "True"||option == "false"||option == "False"){
            break;
        }else{
            cout<<"(only  true or True or False or false are allowed)Try again"<<endl; 
        }
    }
    
    if(option == "true" || option == "True"){
        while (true) {
            cout << "Please enter frequency: ";
            cin >> frequency;

            // Check if the input is valid
            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Incorrect data. Please enter a valid amount.\n";
            } else {
                if(frequency<=0){
                    cout<<"must be greater than 0"<<endl;
                }else{
                    break;
                }
            }
        }
        
    }else{
        frequency = 0;
    }
    due = stringToDate(dueDate);
    Reminder::addReminder(reminders,des,due,option,frequency);
    
    
     
}

void addTransactionFromUser(){
    string des;
    string dueDate;
    string endDate;
    tm end;
    tm due;
    string option;
    int frequency;
    double amt;
    cout<<"Enter the description : ";
    cin.ignore();
    getline(cin,des);
    while (true) {
        cout << "Please enter the Due date (dd.mm.yy): ";
        cin >> dueDate;

        // Check if the input matches the date pattern
        if (regex_match(dueDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter amount: ";
        cin >> amt;

         // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(amt<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                    break;
            }
        }
    }
    
    
    
    while(true){
        cout<<"Does this transaction has end date? (true or false): ";
        cin>>option;

        if(option == "true"||option == "True"||option == "false"||option == "False"){
            break;
        }else{
            cout<<"(only  true or True or False or false are allowed)Try again"<<endl; 
        }
    }


    if(option == "true" || option == "True"){
        while (true) {
            cout << "Please enter the end date (dd.mm.yy): ";
            cin >> endDate;

            // Check if the input matches the date pattern
            if (regex_match(endDate, date_pattern)) {
                break; // Exit the loop if the input is valid
            } else {
                cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
            }
        }
        while (true) {
            cout << "Please enter  frequency: ";
            cin >> frequency;

         // Check if the input is valid
            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Incorrect data. Please enter a valid amount.\n";
            } else {
                if(frequency<=0){
                    cout<<"must be greater than 0"<<endl;
                }else{
                    break;
                }
            }
        }
    }else{
        endDate = "31.12.99";
        while (true) {
            cout << "Please enter  frequency: ";
            cin >> frequency;

         // Check if the input is valid
            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Incorrect data. Please enter a valid amount.\n";
            } else {
                if(frequency<=0){
                    cout<<"must be greater than 0"<<endl;
                }else{
                    break;
                }
            }
        }
        
    }
    due = stringToDate0(dueDate);
    end = stringToDate0(endDate);
    Recurring::addTransaction(transactions,des,due,end,amt,frequency,option);

}


void addGoalFromUser(){
    string des,deadline;
    int pr;
    double t,c,p;
    cout<<"Enter Description : ";
    cin.ignore();
    getline(cin,des);
    while (true) {
        cout << "Please enter the deadline (dd.mm.yy): ";
        cin >> deadline;

        // Check if the input matches the date pattern
        if (regex_match(deadline, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter target price: ";
        cin >> t;

         // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(t<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    while (true) {
        cout << "Please enter current price(if u have 0 enter 0.0) : ";
        cin >> c;

         // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(c<0){
                cout<<"must be greater than 0"<<endl;
            }else{
                    break;
            }
        }
    }
    
    tm dead1 = stringToDate(deadline);
    SavingGoal::setGoal(goals,des,dead1,t,c);
}
void editExpense(vector<Expense>& expenses) {
    Expense::id_giver(expenses);
    int id;
    string blank;
    int counter = Expense::get_counter();
    while (true) {
        cout << "Enter the id of Expense you want to change : ";
        cin >> id;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid data.\n";
        } else {
            if(id<0){
                cout<<"must be greater than 0"<<endl;
            }else if(id>=counter){
                cout<<"this id is unavailable"<<endl;
            }else{
                break;
            }
        }
    }   
    
        
    cout<<"Which part do you want to change? 1. Date or 2. Amount (please only enter 1 or 2)";
    cin>>blank;
    if(blank == "1"){
        string date11;
        while (true) {
            cout << "Please enter the new date (dd.mm.yy): ";
            cin >> date11;

            // Check if the input matches the date pattern
            if (regex_match(date11, date_pattern)) {
                break; // Exit the loop if the input is valid
            } else {
                cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
            }
        }
        expenses[id].date=date11;
        Expense::editExpense(expenses);
    }else if(blank == "2"){
        double amount11;
        string lowerCategory = toLowerCase(expenses[id].category);
        Budgeting::loadFromCSV(budgets);
        forLowerCasemap();
        while (true) {
            cout << "Please enter amount: ";
            cin >> amount11;

            // Check if the input is valid
            if (cin.fail()) {
                cin.clear(); // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Incorrect data. Please enter a valid amount.\n";
            } else {
                if(amount11<=0){
                    cout<<"must be greater than 0"<<endl;
                }else{
                    break;
                }
            }
        }
        auto it = lowerCaseMap.find(lowerCategory);
        if (it != lowerCaseMap.end()) {
            // Key found
            if(it->second.checkBudget()!= true){
                expenses[id].amount=amount11;
                Expense::editExpense(expenses);
                Expense::loadExpensesFromCSV(expenses);
                double total_expense_for_specific_category = Expense::getTotalExpenses(expenses,expenses[id].category);
                it->second.remainingBalance = it->second.amount - total_expense_for_specific_category;
                Budgeting::writeToCSVforUpdating(lowerCaseMap);
            }else if(it->second.checkBudget()== true){
                cout<<"Your budget has reseted you can't edit expense of previous month";
                it->second.checkAndResetBudget();
                Budgeting::writeToCSVforUpdating(lowerCaseMap);
            }
        }else{
            expenses[id].amount=amount11;
            Expense::editExpense(expenses);

        }
    }else{
        cout<<"Invalid choice.Try again."<<endl;
    }
    
}
void deleteExpense(vector<Expense>& expenses) {
    Expense::id_giver(expenses);
    int counter = Expense::get_counter();
    int index;
    while (true) {
        cout << "Enter the id of Expense you want to delete : ";
        cin >> index;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid data.\n";
        } else {
            if(index<0){
                cout<<"must be greater than 0"<<endl;
            }else if(index>=counter){
                cout<<"this id is unavailable"<<endl;
            }else{
                break;
            }
        }
    }   
    char opt;
    cout<<"are you sure want to delete? y or n ";
    cin>>opt;
    if(opt == 'y'){
        if(index <0 || index>= counter){
            cout<<"invalid index"<<endl;
        }else{ //if index is true ,check that index's category exists in budget's name or not if exists it then checks la a twin htl mhr shi lr ma shi lr sitt 
            string lowerCategory = toLowerCase(expenses[index].category);
            Budgeting::loadFromCSV(budgets);
            forLowerCasemap();
            auto it = lowerCaseMap.find(lowerCategory);
            if (it != lowerCaseMap.end()) {
                // Key found
                if(it->second.checkBudget()!= true/*la a twin htl mhr shi*/){
                    it->second.remainingBalance += expenses[index].amount;
                    expenses.erase(expenses.begin() + index);
                    Budgeting::writeToCSVforUpdating(lowerCaseMap);
                    Expense::deleteExpense(expenses);
                }else if(it->second.checkBudget()== true/*la a twin mhr ma shi tot d tine phyat budget ko update lote sayar ma lo*/){
                    expenses.erase(expenses.begin() + index);
                    Expense::deleteExpense(expenses);
                    it->second.checkAndResetBudget();
                    Budgeting::writeToCSVforUpdating(lowerCaseMap);
                }
            }else{
                expenses.erase(expenses.begin() + index);
                Expense::deleteExpense(expenses);

            }
            /*expenses.erase(expenses.begin() + index);
            counter = Expense::get_counter()-1;
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Expense.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Date,Amount,Category\n";
                for(int i = 0 ;i<counter;i++){
                    file << expenses[i].date << ',' <<expenses[i].amount << ',' << expenses[i].category<< '\n';
                    
                }
        
                file.close();
            }else {
                cerr << "Unable to open file." << endl;
            }
            Expense::id_giver(expenses);
            cout<<"Deleted Successfully"<<endl;*/
        }   
    }   
}
void deleteBudget(){
    Budgeting::loadFromCSV(lowerCaseMap);
    
    char str[800];
    while (true) {
        cout << "Enter the name of budget you want to delete (please enter the name correctly because it is case sensitive): ";
        
        cin>>str;

        // Check if the key exists in the map
        if (lowerCaseMap.find(str) != lowerCaseMap.end()) {
            break;  // If key is found, break the loop
        } else {
            cout << "Key not found! Please try again." << endl;
        }
        
    }   
    
    Budgeting::deleteBudgets(lowerCaseMap,str);
}
void updateSpecificAmountInBudget(){
    Budgeting::loadFromCSV(lowerCaseMap);
    char str[800];
    double new_amt;
    while (true) {
        cout << "Enter the name of budget you want to update (please enter the name correctly because it is case sensitive): ";
        
        cin>>str;

        // Check if the key exists in the map
        if (lowerCaseMap.find(str) != lowerCaseMap.end()) {
            break;  // If key is found, break the loop
        } else {
            cout << "Key not found! Please try again." << endl;
        }
        
    }   
    
    while (true) {
        cout << "Please enter amount: ";
        cin >> new_amt;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(new_amt<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    auto it = lowerCaseMap.find(str);
    if (it != lowerCaseMap.end()) {
        // Key found
        if(it->second.checkBudget()!= true){
            double k = it->second.amount - it->second.remainingBalance;
            it->second.original_amt = new_amt;
            it->second.amount = new_amt;
            it->second.remainingBalance = it->second.amount - k;
            Budgeting::writeToCSVforUpdating(lowerCaseMap);
        }else if(it->second.checkBudget()== true){
            it->second.amount = new_amt;
            it->second.remainingBalance = new_amt;
            it->second.original_amt = new_amt;
            it->second.checkAndResetBudget();
            Budgeting::writeToCSVforUpdating(lowerCaseMap);
        }
    }
}

void addAssetsFromUser(vector<Assets>&assets){
    string name,date;
    double amt;
    while (true) {
        cout << "Please enter the  date (dd.mm.yy): ";
        cin >> date;

        // Check if the input matches the date pattern
        if (regex_match(date, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    cout<<"Enter Assets Name: ";
    cin.ignore();
    getline(cin,name);
    while (true) {
        cout << "Please enter amount: ";
        cin >> amt;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(amt<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }
    Assets::addAssets(assets,date,amt,name);
}

void addLiabilitiesFromUser(vector<Liabilities>&liabilities){
    string name,date;
    double amt;
    while (true) {
        cout << "Please enter the  date (dd.mm.yy): ";
        cin >> date;

        // Check if the input matches the date pattern
        if (regex_match(date, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    cout<<"Enter Liability Name: ";
    cin.ignore();
    getline(cin,name);
    while (true) {
        cout << "Please enter  amount: ";
        cin >> amt;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Incorrect data. Please enter a valid amount.\n";
        } else {
            if(amt<=0){
                cout<<"must be greater than 0"<<endl;
            }else{
                break;
            }
        }
    }

    Liabilities::addLiabilities(liabilities,date,amt,name);
}


void generateFinancialReport(){
   
    string startDate,endDate;
    while (true) {
        cout << "Please enter start date (dd.mm.yy): ";
        cin >> startDate;

        // Check if the input matches the date pattern
        if (regex_match(startDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }
    while (true) {
        cout << "Please enter end date (dd.mm.yy): ";
        cin >> endDate;

        // Check if the input matches the date pattern
        if (regex_match(endDate, date_pattern)) {
            break; // Exit the loop if the input is valid
        } else {
            std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
        }
    }

    tm startTm = stringToDate2(startDate);
    tm endTm = stringToDate2(endDate);
    
    cout<<"______________________________________________________________________________________________________________________________"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"Financial Report "<<endl;
    cout<<endl;
    cout<<" Between "<<startDate<<" and "<<endDate<<endl; 
    cout<<endl;
    cout<<endl;
    cout<<"______________________________________________________________________________________________________________________________"<<endl;

    cout<<endl;cout<<endl;cout<<endl;
    cout<<"1. Income OverView"<<endl;
    cout<<endl;cout<<endl;
    
    
    // Display table headers
    cout << left 
        << setw(30) << "Date"
        << setw(30) << "Amount"
        << setw(40) << "Source" << endl;

    // Display a separating line
    cout << string(75, '-') << endl;

    Income::forReport(incomes,startTm,endTm);
    cout<<"______________________________________________________________________________________________________________________________"<<endl;
    cout<<endl;
    double I = Income::forReport2(incomes,startDate,endDate);
    cout<<left<<setw(60)<<"Total Income : "<<I<<endl;cout<<endl;
    cout<<"2. Expense Overview"<<endl;
    cout<<endl;cout<<endl;
    cout << left 
        << setw(30) << "Date"
        << setw(30) << "Amount"
        << setw(40) << "Category" << endl;
    cout << string(75, '-') << endl;
    Expense::forReport11(expenses,startTm,endTm);
    cout<<"______________________________________________________________________________________________________________________________"<<endl;
    cout<<endl;
    double e = Expense::forReport3(expenses,startDate,endDate);
    cout<<left<<setw(60)<<"Total Expense : "<<e<<endl;cout<<endl;
    cout<<endl;cout<<endl;
    cout<<"3. Investment Overview"<<endl;
    cout<<endl;cout<<endl;
    cout << left 
        << setw(15) << "Date"
        << setw(10) << "Units"
        << setw(15) << "Name"
        << setw(18) << "Purchased Price"
        << setw(15) << "Current Price"
        << setw(18) << "Total Purchased"
        << setw(15) << "Total Value"
        << setw(22) << "Return On Investment" << endl;

            // Display a separating line
    cout << string(133, '-') << endl;
    Investment::forReport1(investments,startTm,endTm);
    cout<<"______________________________________________________________________________________________________________________________"<<endl;
    cout<<endl;
    double p = Investment::forReport2(investments,startDate,endDate);
    cout<<left<<setw(60)<<"Total Profits : "<<p<<endl;cout<<endl;
    cout<<endl;
    cout<<"4. Net Worth Overview"<<endl;
    cout<<endl;
    cout<<"Assets"<<endl;
    cout << left
        << setw(15) << "Date"
        << setw(25) << "Name"
        << setw(15) << "Amount" << endl;
    cout << string(65, '-') << endl;  // Table separator
    Assets::forReport(assets,startTm,endTm);
    double total_assets = Assets::calculateTotalAsset(assets,startDate,endDate);
    cout<<"Total assets : "<<total_assets<<endl;
    cout<<"Liabilities"<<endl;
    cout << left
        << setw(15) << "Date"
        << setw(25) << "Name"
        << setw(15) << "Amount" << endl;
    cout << string(65, '-') << endl;  // Table separator
    Liabilities::forReport(liabilities,startTm,endTm);
    double total_l = Liabilities::calculateTotalLiability(liabilities,startDate,endDate);
    cout<<"Total liabilities : "<<total_l<<endl;
    cout<<"______________________________________________________________________________________________________________________________"<<endl;
    cout<<"Total Net Worth : "<<total_assets - total_l<<endl;cout<<endl;cout<<endl;
    cout<<"7. Summary"<<endl;
    cout<<"Total Income : "<<I<<endl;
    cout<<"Total Expense : "<<e<<endl;
    cout<<"Net Income : "<<I - e<<endl;
    cout<<"Total profit on investments : "<<p<<endl;
    cout<<"Net Worth : "<<total_assets - total_l<<endl;
    cout<<"_ _ _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _  _ _ "<<endl;
 




}

void menubox(){
    
    int option;
    cout << "==============================\n";
    cout << "   Welcome to Our Program\n";
    cout << "==============================\n";
    cout << "1. Income\n";
    cout << "2. Expense\n";
    cout << "3. Budget\n";
    cout << "4. Saving Goal\n";
    cout << "5. Investment\n";
    cout << "6. Recurring Transaction\n";
    cout << "7. Reminder\n";
    cout << "8. Net Worth\n";
    cout << "9. Financial Report\n";
    cout << "10. Exit\n";
    cout << "==============================\n";
    cout << "Please select an option (1-10): ";
    cin>>option;
    switch(option){
        case 1:
            int op1;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Add Income "<<endl;
            cout<<" 2. View Income "<<endl;
            cout<<" 3. Get Total Income "<<endl;
            cout<<" 4. Delete Income "<<endl;
            cout<<" 5. Edit Income "<<endl;
            cout<<" Enter your option ";
            cin>>op1;
            switch(op1){
                case 1:
                    addIncomeFromUser(incomes);
                    break;

                case 2:
                    Income::viewIncomes(incomes);
                    break;
                case 3:
                    calculateTotalIncome();
                    break;
                case 4:
                    Income::deleteIncome(incomes);
                    break;
                case 5:
                    Income::editIncome(incomes);
                    break;
                default:
                    cout<<"Invalid choice try again"<<endl;
                    break;
            }
            break;
            
            
        case 2:
            int op;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Add expense "<<endl;
            cout<<" 2. View expense "<<endl;
            cout<<" 3. Get Total expense "<<endl;
            cout<<" 4. Delete  expense "<<endl;
            cout<<" 5. Edit expense "<<endl;
            cout<<" Enter your option ";
            cin>>op;
            switch(op){
                case 1:
                    addExpenseFromUser(expenses);
                    break;

                case 2:
                    Expense::viewExpenses(expenses);
                    break;
                case 3:
                    calculateTotalExpense();
                    break;
                case 4:
                    deleteExpense(expenses);
                    break;
                case 5:
                    editExpense(expenses);
                    break;
                default:
                    cout<<"Invalid choice try again"<<endl;
                    break;
            }

            break;

        case 3:
            int op12;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Set Budget "<<endl;
            cout<<" 2. View Budgets "<<endl;
            cout<<" 3. Delete Budget "<<endl;
            cout<<" 4. Update Amount  "<<endl;
            cout<<" Enter your option ";
            cin>>op12;
            switch(op12){
                case 1:
                    addBudgetFromUser(budgets);
                    break;
                case 2:
                    Budgeting::loadFromCSV(lowerCaseMap);
                    
                    Budgeting::viewBudgets(lowerCaseMap);
                    break;
                case 3:
                    deleteBudget();
                    break;
                case 4:
                    updateSpecificAmountInBudget();
                    break;
                default:
                    cout<<"Invalid choice try again"<<endl;
                    break;

            }
            
             
            break;

        case 4:
            int opk;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Set Saving Goal  "<<endl;
            cout<<" 2. View Saving goal "<<endl;
            cout<<" 3. Edit Goal"<<endl;
            cout<<" 4. Delete Goal"<<endl;
            cout<<" 5. Automatic Priority Setter(this generates the goal with nearest deadline to highest priority)"<<endl;
            cout<<" 6 . Contribute to goal"<<endl;

            cout<<" Enter your option ";
            cin>>opk;
            switch (opk){
                case 1:
                    addGoalFromUser();
                    break;
                case 2:
                    SavingGoal::displayGoals(goals);
                    break;
                case 3:
                    SavingGoal::editGoal(goals);
                    break;
                case 4:
                    SavingGoal::deleteGoals(goals);
                    break;
                case 5:
                    SavingGoal::sortGoalsByDeadline(goals);
                    SavingGoal::writeToCSVforUpdating(goals);
                    cout<<"Done"<<endl;
                    break;
                case 6:
                    SavingGoal::contribute(goals);
                    break;
                default:
                    cout<<"Invalid choice try again"<<endl;
                    break;

            }
            
            
            break;


        case 5:
            int opt;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Add Investment "<<endl;
            cout<<" 2. Display Investment "<<endl;
            cout<<" 3. Calculate total Current value "<<endl;
            cout<<" 4. Calculate ROI "<<endl;
            cout<<" 5. Edit Investment "<<endl;
            cout<<" 6. Delete Investment "<<endl;
            cout<<" 7. Total Profit "<<endl;
            
            cout<<" Enter your option ";
            cin>>opt;
            
            switch(opt){
                case 1:
                    addInvestmentFromUser(investments);
                    
                    break;

                case 2:
                    Investment::displayDataOfInvestments(investments);
                    break;
                case 3:
                    calculateTotalCurrentValue();
                    break;
                case 4:
                    Investment::get_roi(investments);
                    break;
                case 5:
                    Investment::edit_investments(investments);
                    break;
                case 6:
                    Investment::delete_investments(investments);
                    break;
                case 7:
                    calculateTotalProfit();
                    
                    break;
                default:
                    cout<<"Invalid choice try again"<<endl;
                    break;
            }
            
            
            break;

        case 6:
            int opy;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Set Recurring Transaction "<<endl;
            cout<<" 2. Display Recurring Transaction  "<<endl;
            cout<<" 3. Delete Recurring Transaction  "<<endl;
            cout<<" 4. Update Datas"<<endl;
            
            cout<<" Enter your option ";
            cin>>opy;
            switch(opy){
                case 1:
                    addTransactionFromUser();
                    break;
                case 2:
                    Recurring::displayTable(transactions);
                    break;
                case 3:
                    Recurring::deleteTransactions(transactions);
                    break;
                case 4:
                    Recurring::edit_transactions(transactions);
                    break;
                default:
                    cout<<"Invalid choice try again"<<endl;
                    break;

            }
            break;
            
        case 7:
            
            int opz;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Set Reminder "<<endl;
            cout<<" 2. Display reminders "<<endl;
            cout<<" 3. Delete reminder "<<endl;
            cout<<" 4. Update Due Date "<<endl;
            cout<<" 5. Update Frequency "<<endl;
            
            
            cout<<" Enter your option ";
            cin>>opz;
            switch(opz){
                case 1:
                    addReminderFromUser();
                    break;
                case 2:
                    Reminder::viewReminders(reminders);
                    break;
                case 3:
                    Reminder::deleteReminders(reminders);
                    break;
                case 4:
                    Reminder::updateDueDate(reminders);
                    break;
                case 5:
                    Reminder::updateFrequency(reminders);
                    break;
                default:
                    cout<<"Invalid choice try again. "<<endl;
                    break;
                

            }
            
            
            break;
            

        case 8:
            int opc;
            cout<<"What do you want to do sir?"<<endl;
            cout<<" 1. Add Asset "<<endl;
            cout<<" 2. Add Liability "<<endl;
            cout<<" 3. View Asset "<<endl;
            cout<<" 4. View Liability "<<endl;
            cout<<" 5. Update Asset "<<endl;
            cout<<" 6. Update Liability "<<endl;
            cout<<" 7. Delete Asset "<<endl;
            cout<<" 8. Delete Liability "<<endl;
            cout<<" 9. Calculate Net Worth "<<endl;
            cout<<" Enter your option ";
            cin>>opc;
            switch(opc){
                case 1:
                    addAssetsFromUser(assets);
                    break;
                case 2:
                    addLiabilitiesFromUser(liabilities);
                    break;
                case 3:
                    Assets::viewAssets(assets);
                    break;
                case 4:
                    Liabilities::viewLiabilities(liabilities);
                    break;
                case 5:
                    Assets::editAsset(assets);
                    break;
                case 6:
                    Liabilities::editLiability(liabilities);
                    break;
                case 7:
                    Assets::deleteAsset(assets);
                    break;
                case 8:
                    Liabilities::deleteLiability(liabilities);
                    break;
                case 9:
                    calculateTotalNetWorth();
                    break;
                default:
                    cout<<"Invalid choice"<<endl;
                    break;
                     
            }
            break; 
            
        case 9:
            generateFinancialReport();
            break;
        
        case 10:

            exit(0);
        default :
            cout<<"Invalid choice try again "<<endl;
            break;




    
    };

}
class UserDatabase {
public:
    UserDatabase(const string& filename) : filename(filename) {
        load();
    }

    bool authenticate(const string& username, const string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second == password) {
            return true;
        }
        return false;
    }

    bool userExists(const string& username) {
        return users.find(username) != users.end();
    }

    void addUser(const string& username, const string& password) {
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }
        file << username << "," << password << "\n";
        file.close();
        users[username] = password;
    }

private:
    unordered_map<string, string> users;
    string filename;

    void load() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        // Skip the header line
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string username, password;
            getline(ss, username, ',');
            getline(ss, password, ',');
            users[username] = password;
        }

        file.close();
    }
};

void createDirectoryAndFiles(const string& username) {
    // Check if directory already exists
    if (access(username.c_str(), F_OK) != 0) {
        // Directory does not exist, create it
        if (mkdir(username.c_str()) != 0) {
            cerr << "Error: Could not create directory for user " << username << " - " << strerror(errno) << endl;
            return;
        }
    } 
    // Create Income CSV file if it does not exist
    string incomeFile = username + "/" + username + "_Income.csv";
    if (access(incomeFile.c_str(), F_OK) != 0) {
        ofstream incomeFileStream(incomeFile);
        if (!incomeFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        incomeFileStream << "Date,Amount,Source\n"; // Example header
        incomeFileStream.close();
    } 
    // Create Expense CSV file if it does not exist
    string expenseFile = username + "/" + username + "_Expense.csv";
    if (access(expenseFile.c_str(), F_OK) != 0) {
        ofstream expenseFileStream(expenseFile);
        if (!expenseFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        expenseFileStream << "Date,Amount,Category\n"; // Example header
        expenseFileStream.close();
    } 

    string investmentFile = username + "/" + username + "_investment.csv";
    if (access(investmentFile.c_str(), F_OK) != 0) {
        ofstream investmentFileStream(investmentFile);
        if (!investmentFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        investmentFileStream << "Purchased Date,Units,Investment name,Purchased Price,Current Price,Total Purchased Price,Total Current Value,Roi(%)\n"; // Example header
        investmentFileStream.close();
    } 

    string budgetFile = username + "/" + username + "_budget.csv";
    if (access(budgetFile.c_str(), F_OK) != 0) {
        ofstream budgetFileStream(budgetFile);
        if (!budgetFileStream.is_open()) {
            cerr << "Error: Could not create file "  << endl;
            return;
        }
        budgetFileStream << "Budget name,Amount,Type,Remaining Amount,Limit,Original Amount\n"; 
        budgetFileStream.close();
    }

    string assetsFile = username + "/" + username + "_assets.csv";
    if (access(assetsFile.c_str(), F_OK) != 0) {
        ofstream assetsFileStream(assetsFile);
        if (!assetsFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        assetsFileStream << "Date,Name,Amount\n"; 
        assetsFileStream.close();
    } 


    string liabilitiesFile = username + "/" + username + "_liabilities.csv";
    if (access(liabilitiesFile.c_str(), F_OK) != 0) {
        ofstream liabilitiesFileStream(liabilitiesFile);
        if (!liabilitiesFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        liabilitiesFileStream << "Date,Name,Amount\n"; 
        liabilitiesFileStream.close();
    }

    string reminderFile = username + "/" + username + "_reminder.csv";
    if (access(reminderFile.c_str(), F_OK) != 0) {
        ofstream reminderFileStream(reminderFile);
        if (!reminderFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        reminderFileStream << "Description,Due Date,Is Recurring,Frequency\n"; 
        reminderFileStream.close();
    }

    string recurringTFile = username + "/" + username + "_transaction.csv";
    if (access(recurringTFile.c_str(), F_OK) != 0) {
        ofstream rTFileStream(recurringTFile);
        if (!rTFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        rTFileStream << "Description,Date,End Date,Amount,Frequency,hasEnd\n"; 
        rTFileStream.close();
    }

    string goalFile = username + "/" + username + "_goal.csv";
    if (access(goalFile.c_str(), F_OK) != 0) {
        ofstream goalFileStream(goalFile);
        if (!goalFileStream.is_open()) {
            cerr << "Error: Could not create file " << endl;
            return;
        }
        goalFileStream << "Description,Deadline,Target Amount,Current Amount,Progress(%),Priority\n"; 
        goalFileStream.close();
    }


}

void login(UserDatabase& db) {
    string username, password;

    cout << "Enter username: ";
    cin.ignore();
    getline(cin,username);
    
    cout << "Enter password: ";
    cin >> password;

    if (db.authenticate(username, password)) {
        cout << "Login successful!" << endl;
        createDirectoryAndFiles(username);
        Investment::setUsername(username);
        Expense::setUsername(username);
        Income::setUsername(username);
        Budgeting::setUsername(username);
        Assets::setUsername(username);
        Liabilities::setUsername(username);
        Reminder::setUsername(username);
        Recurring::setUsername(username);
        SavingGoal::setUsername(username);
        while(true){
            Recurring::processTransaction(transactions);
            checkReminders();
            menubox();

            
           
        }

        
         
    } else {
        cout << "Login failed. Incorrect username or password." << endl;
    }
}

void signUp(UserDatabase& db) {
    string username, password;

    cout << "Enter a new username: ";
    cin.ignore();
    getline(cin,username);

    if (db.userExists(username)) {
        cout << "Username already exists. Please choose a different username." << endl;
        return;
    }

    cout << "Enter a new password: ";
    cin >> password;

    db.addUser(username, password);
    cout << "Sign up successful! You can now log in." << endl;
    createDirectoryAndFiles(username);
    Investment::setUsername(username);
    Expense::setUsername(username);
    Income::setUsername(username);
    Budgeting::setUsername(username);
    Assets::setUsername(username);
    Liabilities::setUsername(username);
    Reminder::setUsername(username);
    Recurring::setUsername(username);
    SavingGoal::setUsername(username);
    while(true){
        Recurring::processTransaction(transactions);
        checkReminders();
        menubox();

       
    }
    
}

int main() {
    UserDatabase db("users.csv");
    
    int choice;
    //cout<<"Welcome to our personal finance management program"<<endl;
    std::cout << "+==============================================+\n";
    std::cout << "|    #####   ######   ##        ##  ######     |\n";
    std::cout << "|    ##  ##  ##       ## ##  ## ##  ##         |\n";
    std::cout << "|    #####   ######   ##   ##   ##  ######     |\n";
    std::cout << "|    ##      ##       ##        ##      ##     |\n";
    std::cout << "|    ##      ##       ##        ##  ######     |\n";
    std::cout << "+==============================================+\n";
    std::cout << "                     ***\n";
    std::cout << "             Welcome to Our Program\n";
    std::cout << "                     ***\n";
    
    cout << "1. Log in" << endl;
    cout << "2. Sign up" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        login(db);
    } else if (choice == 2) {
        signUp(db);
    } else {
        cout << "Invalid choice. Exiting." << endl;
    }

    return 0;
}

