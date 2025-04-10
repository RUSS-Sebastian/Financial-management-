#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
using namespace std;
class Budgeting{
    

    public:
        static string username;
        double amount;
        double remainingBalance;
        string budgetType; // "Monthly" or "Yearly"
        double limit;
        double original_amt;
        time_t setDate;

        Budgeting() : amount(0.0), remainingBalance(0.0), budgetType("Monthly"),limit(0.0),original_amt(0.0) {}

        static void setUsername(const string &user) {
            username = user;
        }

        void checkType(){
            if(budgetType != "None"){
                time(&setDate);
            }
        }
        // Function to reset the budget if the period has passed
        void checkAndResetBudget() {
            time_t now = time(0);
            tm *ltm = localtime(&now);
            tm *setLtm = localtime(&setDate);

            if (budgetType == "Monthly" && (ltm->tm_mon != setLtm->tm_mon)) {
                remainingBalance = original_amt; // Reset the budget
                setDate = now; // Update the set date to the new period
            } else if (budgetType == "Yearly" && (ltm->tm_year != setLtm->tm_year)) {
                remainingBalance = original_amt; // Reset the budget
                setDate = now; // Update the set date to the new period
            }
        }

       
        

        bool checkBudget(){
            time_t now = time(0);
            tm *ltm = localtime(&now);
            tm *setLtm = localtime(&setDate);

            if (budgetType == "Monthly" && (ltm->tm_mon != setLtm->tm_mon)) {
                
                return true;
            } else if (budgetType == "Yearly" && (ltm->tm_year != setLtm->tm_year)) {
                
                return true;
            }else if (budgetType == "None"){
                return false;
            }else if (budgetType == "Monthly" && (ltm->tm_mon == setLtm->tm_mon)) {
                
                return false;
            } else if (budgetType == "Yearly" && (ltm->tm_year == setLtm->tm_year)) {
                
                return false;
            }
        }


        void setBudget(double amt, const string& type,double lmt) {
            amount = amt;
            remainingBalance = amt; // Initially, remaining balance is equal to the total amount
            budgetType = type;
            limit = lmt;
            original_amt = amt;
        }

        static void writeToCSV(const string& name, Budgeting& budget) {
            ofstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_budget.csv", ios::app);
            if (file.is_open()) {
                file << name << "," << budget.amount << "," << budget.budgetType <<","<< budget.remainingBalance<<","<<budget.limit<<","<<budget.original_amt  << "\n";
                file.close();
            }else {
            std::cerr << "Failed to open file "  << endl;
            }
        }

        static void writeToCSVforUpdating(map<string,Budgeting>& budgets){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_budget.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Budget name,Amount,Type,Remaining Amount,Limit,Original Amount\n";
                for(const auto & pair:budgets){
                    file<<pair.first<<","<<pair.second.amount<<","<<pair.second.budgetType<<","<<pair.second.remainingBalance<<","<<pair.second.limit<<","<<pair.second.original_amt<<"\n";
                }
                file.close();
                cout<<"Updated Successfully"<<endl;
            } else {
                cerr << "Unable to open file." << endl;
            }
                 
        }

         // Static method to load budgets from a CSV file
        static void loadFromCSV(map<string, Budgeting>& budgets) {
            budgets.clear();
            ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_budget.csv");
            string line;
            if (file.is_open()) {
                getline(file, line);
                while (getline(file, line)) {
                    stringstream ss(line);
                    string name, type;
                    double amt, remaining,limit,org_amt;

                    getline(ss, name, ',');
                    ss >> amt;
                    ss.ignore(1); // Skip comma
                    getline(ss, type, ',');
                    ss >> remaining;
                    ss.ignore(1); // Skip comma
                    ss>>limit;
                    ss.ignore(1);
                    ss>>org_amt;

                    Budgeting budget;
                    budget.amount = amt;
                    budget.remainingBalance = remaining;
                    budget.budgetType = type;
                    budget.limit = limit;
                    budget.original_amt = org_amt;

                    budgets[name] = budget;
                }
                file.close();
              
                
            } else {
                cerr << "Failed to open the file "  << endl;
            }
        }

        static void viewBudgets(map<string,Budgeting>&budgets){
            // Set up the table headers
            cout << left
                << setw(25) << "Name"
                << setw(15) << "Amount"
                << setw(15) << "Type"
                << setw(20) << "Remaining Amount" << endl;
            cout << string(75, '-') << endl;  // Table separator

            // Display each budget in a formatted way
            for (const auto& pair : budgets) {
                cout << setw(25) << pair.first
                    << setw(15) << fixed << setprecision(2) << pair.second.amount
                    << setw(15) << pair.second.budgetType
                    << setw(20) << fixed << setprecision(2) << pair.second.remainingBalance << endl;
            }
        }

        static void deleteBudgets(map<string,Budgeting>&budgets,string str){
            budgets.erase(str);
            Budgeting::writeToCSVforUpdating(budgets);
            
        }

        
};      



string Budgeting::username;