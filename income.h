// expense.h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>
#include <limits>
using namespace std;
tm stringToDate2(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d.%m.%Y");
     

    // Adjust the year (because %y assumes years since 1900)
    tm.tm_year += (tm.tm_year < 100) ? 100 : 0;  // Adjust for years like 24 to mean 2024

    return tm;
}
class Income {

    

public:

    string date;
    double amount;
    string source;
    int id;
    static string username;
    Income(string date, double amount, string source)
        : date(date), amount(amount), source(source) {}

    static void setUsername(const string &user) {
        username = user;
    }
    static void calculateTotalIncome( vector<Income>& incomes, const string& startDate, const string& endDate) {
        Income::loadIncomesFromCSV(incomes);
        tm startTm = stringToDate2(startDate);
        tm endTm = stringToDate2(endDate);
        double total = 0.0; 

        for (const auto& income : incomes) {
            tm incomeDate = stringToDate2(income.date);
            

            if ((mktime(&incomeDate) >= mktime(&startTm)) && (mktime(&incomeDate) <= mktime(&endTm))) {
                total += income.amount;
            }
        }

        cout<<"your total income between "<<startDate<<" "<<"and "<<endDate<<" is "<<total<<endl;
    }

    static double forReport2( vector<Income>& incomes, const string& startDate, const string& endDate) {
        Income::loadIncomesFromCSV(incomes);
        tm startTm = stringToDate2(startDate);
        tm endTm = stringToDate2(endDate);
        double total = 0.0; 

        for (const auto& income : incomes) {
            tm incomeDate = stringToDate2(income.date);
            

            if ((mktime(&incomeDate) >= mktime(&startTm)) && (mktime(&incomeDate) <= mktime(&endTm))) {
                total += income.amount;
            }
        }

        return total;
    }
    static void addIncome(vector<Income>& incomes, const string& date, double amount, const string& source) {
        incomes.emplace_back(date, amount, source);
        saveToCsv(date,amount,source);
    }

    static void viewIncomes(vector<Income>& incomes) {
        // Call the id_giver function
        Income::id_giver(incomes);
    
        // Display table headers
        cout << left << setw(5) << "ID"
            << setw(15) << "Date"
            << setw(15) << "Amount"
            << setw(25) << "Source" << endl;

        // Display a separating line
        cout << string(60, '-') << endl;

        // Display each income in table format
        for (const auto& income : incomes) {
            cout << left << setw(5) << income.id 
                << setw(15) << income.date 
                << setw(15) << fixed << setprecision(2) << income.amount 
                << setw(25) << income.source << endl;
        }
    }


    static void forReport(vector<Income>&incomes,tm &startDate,tm &endDate){
        
        Income::loadIncomesFromCSV(incomes);
        // Display each income in table format
        for (const auto& income : incomes) {
            tm incomeDate = stringToDate2(income.date);
            if ((mktime(&incomeDate) >= mktime(&startDate)) && (mktime(&incomeDate) <= mktime(&endDate))) {
                cout << left 
                    << setw(30) << income.date 
                    << setw(30) << fixed << setprecision(2) << income.amount 
                    << setw(40) << income.source << endl;
            }
        }
    }

    static void editIncome(vector<Income>& incomes) {
        regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
        Income::id_giver(incomes);
        int id;
        string blank;
        int counter = Income::get_counter();
        while (true) {
            cout << "Enter the id of Income you want to change : ";
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
        
        
        
        cout<<"Which part do you want to change? 1. Date or 2. Amount or 3. Source (please only enter 1 to 3)";
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
                    std::cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                }
            }
            incomes[id].date=date11;
        }else if(blank == "2"){
            double amount11;
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
            
            incomes[id].amount=amount11;
        }else if(blank == "3"){
            string source11;
            cout<<"Enter the Source : ";
            cin.ignore();
            getline(cin,source11);
            incomes[id].source=source11;
        }else{
            cout<<"Invalid choice. Try again"<<endl;
        }
        ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Income.csv", ios::trunc);
        if (file.is_open()) {
        
            file << "Date,Amount,Source\n";
            for(int i = 0 ;i<counter;i++){
                    file << incomes[i].date << ',' <<incomes[i].amount << ',' << incomes[i].source<< '\n';
                    
            }
        
            file.close();
               
            cout<<"Edited Successfully"<<endl;
        }else{
            cerr << "Unable to open file." << endl;
        }
    }

    static void deleteIncome(vector<Income>& incomes) {
        Income::id_giver(incomes);
        int counter = Income::get_counter();
        int index;
        while (true) {
            cout << "Enter the id of Income you want to delete : ";
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
        cout<<"are you sure want to delete? y or n : ";
        cin>>opt;
        if(opt == 'y'){
            if(index <0 || index>= counter){
                cout<<"invalid index"<<endl;
            }else{
                
                incomes.erase(incomes.begin() + index);
                counter = Income::get_counter()-1;
                ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Income.csv", ios::trunc);
                if (file.is_open()) {
        
                    file << "Date,Amount,Source\n";
                    for(int i = 0 ;i<counter;i++){
                        file << incomes[i].date << ',' <<incomes[i].amount << ',' <<incomes[i].source<< '\n';
                    
                    }
        
                    file.close();
                }else {
                    cerr << "Unable to open file." << endl;
                }
                Income::id_giver(incomes);
                cout<<"Deleted Successfully"<<endl;
            }   
            }
        
       
    }

    static double getTotalIncomes(vector<Income>& incomes) {
        int counter = Income::get_counter();
        Income::loadIncomesFromCSV(incomes);
        double total_income = 0.0;
        for(int i =0;i<counter;i++){
            total_income+=incomes[i].amount;
        }
        cout<<"Your total income is "<<total_income<<endl;
    }


    static void loadIncomesFromCSV(vector<Income>& incomes) {
        incomes.clear();
        ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_Income.csv");
        

        string line;
        getline(file, line); // Skip the header line

        while (getline(file, line)) {
            string date, amountStr, source;
            stringstream ss(line);
            getline(ss, date, ',');
            getline(ss, amountStr, ',');
            getline(ss, source, ',');

            double amount = stod(amountStr);
            incomes.emplace_back(date, amount, source);
        }

        file.close();
    }

    static void saveIncomesToCSV(vector<Income>& incomes) {
        ofstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_Income.csv", ios::app);
        if (file.is_open()) {
            int counter = Income::get_counter();
            if(counter == 0){
                file << incomes[0].date << ',' << incomes[0].amount << ',' << incomes[0].source << '\n';
            }else{
                for(int i = 0 ;i<counter;i++){
                    file << incomes[i].date << ',' << incomes[i].amount << ',' << incomes[i].source << '\n';
                    
                }
            }
                
            file.close();
        } else {
            std::cerr << "Unable to open file for writing: " << username + "/income.csv" << std::endl;
        }
    }
    static void saveToCsv(string date,double amount,string source){//careful no &
        ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Income.csv",ios::app);
        if(file.is_open()){
            file << date << ',' << amount << ',' << source<< '\n';
            file.close();
        }else {
            std::cerr << "Unable to open file for writing: " << username + "/income.csv" << std::endl;
        }
    }
    static void id_giver(vector<Income>&incomes){
            int counter = Income::get_counter();
            Income::loadIncomesFromCSV(incomes);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                incomes[i].id = idd;
                idd++;
                
            }

    }

    static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_Income.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
    }
};
string Income::username;

