#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <regex>
#include <limits>
using namespace std;
 
class Recurring{

    public:
        string description;
        double amount;
        tm date;//start date alo called next due date;
        tm endDate;
        int frequency;// add that day to tm date for next due date
        string hasEndDate;
        static string username;
        int id;


        Recurring(string des,tm date,tm endDate  ,double amt,int f,string hasEnd):description(des),date(date),endDate(endDate),amount(amt),frequency(f),hasEndDate(hasEnd){}

        static void setUsername(const string &user) {
            username = user;
        }

        static void addTransaction(vector<Recurring>transactions,const string& description, const tm& dueDate, const tm& endDate  ,double amt,int f ,string hasEnd) {
            transactions.emplace_back(description,dueDate,endDate,amt,f,hasEnd);
            string dueDate_str = tmToString(dueDate);
            string endDate_str = tmToString(endDate);
            Recurring::saveToCsv(description,dueDate_str,endDate_str,amt,f,hasEnd);

        }

        static void saveToCsv(const string& description, const string& dueDate, const string & endDate,double amt,int f,string hasEnd){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_transaction.csv",ios::app);
            if(file.is_open()){
                file << description << ',' << dueDate << ',' << endDate<<','<<amt<<","<<f<<","<<hasEnd<<'\n';
                file.close();
            }else {
                cerr << "Unable to open file for writing " << endl;
            }
        }

        static void loadDatasFromCSV(vector<Recurring>& transactions) {
            transactions.clear();
            ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_transaction.csv");
        

            string line;
            getline(file, line); // Skip the header line

            while (getline(file, line)) {
                string des,due,option,frequency,end,amt;
                stringstream ss(line);
                getline(ss, des, ',');
                getline(ss, due, ',');
                getline(ss,end, ',');
                getline(ss,amt, ',');
                getline(ss,frequency, ',');
                getline(ss,option, ',');

                tm dueDate11 = stringToDate0(due);
                tm endDate11 = stringToDate0(end);
                int frequency1 = stoi(frequency);
                double amount = stod(amt);
                transactions.emplace_back(des,dueDate11,endDate11,amount,frequency1,option);
            }

            file.close();
        }

        static void displayTable(vector<Recurring>& transactions) {
            Recurring::id_giver(transactions);

            cout << std::left
                    << setw(10) << "ID"
                    << setw(20) << "Description"
                    << setw(15) << "Next Due Date"
                    << setw(15) << "End Date"
                    << setw(10) << "Amount"
                    << setw(15) << "Frequency"
                    << setw(20) << "Has End Date"
                    << endl;
    
            std::cout << std::string(95, '-') << std::endl;

            for (const auto& entry : transactions) {
                string date = tmToString(entry.date);
                string end = tmToString(entry.endDate);
                cout << std::left
                    << setw(10) << entry.id
                    << setw(20) << entry.description
                    << setw(15) << date
                    << setw(15) << end
                    << setw(10) << entry.amount
                    << setw(15) << entry.frequency
                    << setw(20) << entry.hasEndDate
                    << endl;
            }
        }

        static void id_giver(vector<Recurring>&transactions){
            int counter = Recurring::get_counter();
            Recurring::loadDatasFromCSV(transactions);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                transactions[i].id = idd;
                idd++;
                
            }

        }

        static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_transaction.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
        }

        static void writeToCSVforUpdating(vector<Recurring>&transactions){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_transaction.csv", ios::trunc);
            if (file.is_open()) {
        
                file <<  "Description,Date,End Date,Amount,Frequency,hasEnd\n"; 
                for(const auto & t:transactions){
                    string due_str = tmToString(t.date);
                    string end_str = tmToString(t.endDate);
                    file<<t.description<<","<<due_str<<","<<end_str<<","<<t.amount<<","<<t.frequency<<","<<t.hasEndDate<<"\n";
                }
                file.close();
                
            } else {
                cerr << "Unable to open file." << endl;
            }
                 
        }

        static void deleteTransactions(vector<Recurring>&transactions){
            Recurring::id_giver(transactions);
            int counter = Recurring::get_counter();
            int index;
            while (true) {
                cout << "Enter the id of transaction you want to delete : ";
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
                
                    transactions.erase(transactions.begin() + index);
                    Recurring::writeToCSVforUpdating(transactions);

                }

            }

        }


        static void edit_transactions(vector<Recurring>&transactions){
            Recurring::id_giver(transactions);
            int id;
            string blank;
            int counter = Recurring::get_counter();
            while (true) {
                cout << "Enter the id of transaction you want to change : ";
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
            
            cout<<"Which part do you want to change? 1.Description or 2.Date  or 3.End Date or 4.Amount or 5.frequency 6.hasEnd (please only enter 1 to 6)";
            cin>>blank;
            if(blank == "1"){
                string des11;
                cout<<"Enter new description : ";
                cin>>des11;
                transactions[id].description=des11;
            }else if(blank == "2"){
                tm date11;
                string date11_str;
                regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
                while (true) {
                    cout << "Please enter the new date (dd.mm.yy): ";
                    cin >> date11_str;

                    // Check if the input matches the date pattern
                    if (regex_match(date11_str, date_pattern)) {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                    }
                }
                date11 = stringToDate0(date11_str);
                transactions[id].date=date11;
            }else if(blank == "3"){
                tm endDate11;
                string endDate11_str;
                regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
                while (true) {
                    cout << "Please enter the new end Date (dd.mm.yy): ";
                    cin >> endDate11_str;

                    // Check if the input matches the date pattern
                    if (regex_match(endDate11_str, date_pattern)) {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                    }
                }
                endDate11 = stringToDate0(endDate11_str);
                transactions[id].endDate=endDate11;
            }else if(blank == "4"){
                double amt;
                while (true) {
                    cout << "Please enter new amount: ";
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
                transactions[id].amount =amt;
            }else if(blank == "5"){
                int f;
                while (true) {
                    cout << "Please enter new frequency: ";
                    cin >> f;

                    // Check if the input is valid
                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << "Incorrect data. Please enter a valid amount.\n";
                    } else {
                        if(f<0){
                            cout<<"must be greater than 0"<<endl;
                        }else{
                            break;
                        }
                    }
                }
                transactions[id].frequency =f;


            }else if(blank == "6"){
                string hasEnd;
                cout<<"Update has end ";
                cin>>hasEnd;
                transactions[id].hasEndDate = hasEnd;
            }else{
                cout<<"Invalid choice.Try again"<<endl;
            }

            cout<<"Updated Succesfully"<<endl;
            Recurring::writeToCSVforUpdating(transactions);
            
        }


        bool isDue() {
            time_t now = time(nullptr);
            tm * currentDate = localtime(&now);
            
            return (now >= mktime(&date));
        }


        static void processTransaction(vector<Recurring>&transactions){

            Recurring::id_giver(transactions);
            for( auto& t:transactions){
                

                while(t.isDue()){
                    cout<<"Processing Transaction .........."<<t.description<<" .......... Amount : " <<t.amount<<" Due Date : "<<asctime(&t.date);
                    time_t timeSinceEpoch = mktime(&t.date);
                    timeSinceEpoch += t.frequency * 24 * 60 * 60;// Add days in seconds
                    tm *newTm = localtime(&timeSinceEpoch);// Convert back to tm structure
                    t.date = *newTm;

                }
                
                

                if((t.hasEndDate == "true"|| t.hasEndDate == "True") && difftime(mktime(&t.date),mktime((&t.endDate)))>=0){
                    cout<<t.description<<" was the last occurrence of transaction this time  "<<endl;
                     // Remove the transaction from the vector
                    transactions.erase(remove_if(transactions.begin(),transactions.end(),[&t](const Recurring& tx){return tx.id == t.id;}),transactions.end());

                }else{
                    cout<<"No transaction due today for : "<<t.description<<endl;
                }
            }

            Recurring::writeToCSVforUpdating(transactions);

        }
};
string Recurring::username;