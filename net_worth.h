#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <limits>
using namespace std;
#include <ctime>
tm stringToDate3(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d.%m.%Y");
     

    // Adjust the year (because %y assumes years since 1900)
    tm.tm_year += (tm.tm_year < 100) ? 100 : 0;  // Adjust for years like 24 to mean 2024
    return tm;
}
class Assets{
    public:
        string date;
        double amount;
        string name;
        static string username;
        int id;
        Assets(string date,double amt,string name):date(date),amount(amt),name(name){}

        static void addAssets(vector<Assets>&assets,string date,double amount,string name){
            assets.emplace_back(date,amount,name);
            ///write to csv
            Assets::writeToCSV(date,amount,name);
        }

        static void setUsername(const string &user) {
            username = user;
        }

        static double calculateTotalAsset( vector<Assets>& assets, const string& startDate, const string& endDate) {
            Assets::loadFromCSV(assets);
            tm startTm = stringToDate3(startDate);
            tm endTm = stringToDate3(endDate);
            double total = 0.0; 

            for (const auto& asset : assets) {
                tm assetDate = stringToDate3(asset.date);
            

                if ((mktime(&assetDate) >= mktime(&startTm)) && (mktime(&assetDate) <= mktime(&endTm))) {
                    total += asset.amount;
                }
            }

            return total;
        }

        static void writeToCSV(string date,double amount,string name) {
            ofstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_assets.csv", ios::app);
            if (file.is_open()) {
                file << date << "," <<name << "," << amount  << "\n";
                file.close();
            }else {
                cerr << "Failed to open file "  << endl;
            }
        }

        static void writeToCSVforUpdating(vector<Assets>&assets){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_assets.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Date,Name,Amount\n";
                for(const auto & asset:assets){
                    file<<asset.date<<","<<asset.name<<","<<asset.amount<<"\n";
                }
                file.close();
                cout<<"Updated Successfully"<<endl;
            } else {
                cerr << "Unable to open file." << endl;
            }
                 
        }

        static void loadFromCSV(vector<Assets>&assets) {
            assets.clear();
            ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_assets.csv");
            string line;
            if (file.is_open()) {
                getline(file, line);
                while (getline(file, line)) {
                    stringstream ss(line);
                    string date, name,amt_str;
                    

                    getline(ss, date, ',');
                    getline(ss, name, ',');
                    getline(ss, amt_str, ',');
                    double amt = stod(amt_str);
                    
                    assets.emplace_back(date,amt,name);
                }
                file.close();
              
                
            } else {
                cerr << "Failed to open the file "  << endl;
            }
        }

        static void viewAssets(vector<Assets>&assets){
            Assets::id_giver(assets);
              // Set up the table headers
            cout << left
                << setw(10) << "ID"
                << setw(15) << "Date"
                << setw(25) << "Name"
                << setw(15) << "Amount" << endl;
            cout << string(65, '-') << endl;  // Table separator

            // Display each asset in a formatted way
            for (const auto& asset : assets) {
                cout << setw(10) << asset.id
                    << setw(15) << asset.date
                    << setw(25) << asset.name
                     << setw(15) << fixed << setprecision(2) << asset.amount << endl;
            }
        }

        static void forReport(vector<Assets>&assets,tm startDate,tm endDate){
            Assets::loadFromCSV(assets);
            for (const auto& asset : assets){
                tm aDate = stringToDate3(asset.date);
                if ((mktime(&aDate) >= mktime(&startDate)) && (mktime(&aDate) <= mktime(&endDate))){
                    cout << setw(15) << asset.date
                        << setw(25) << asset.name
                        << setw(15) << fixed << setprecision(2) << asset.amount << endl;
                }
            }

        }

        static void editAsset(vector<Assets>& assets) {
            Assets::id_giver(assets);
            int id;
            string blank;
            int counter = Assets::get_counter();
            while (true) {
                cout << "Enter the id of Asset you want to change : ";
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
            
        
            cout<<"Which part do you want to change? 1. Date or 2. Name or 3. Amount (please only enter 1 to 3)";
            cin>>blank;
            if(blank == "1"){
                string date11;
                regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
                while (true) {
                    cout << "Please enter new date (dd.mm.yy): ";
                    cin >> date11;

                    // Check if the input matches the date pattern
                    if (regex_match(date11, date_pattern)) {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                    }
                }
                
                assets[id].date=date11;
            }else if(blank == "2"){
                string name11;
                cout<<"Enter the name of asset you want to change ";
                cin.ignore();
                getline(cin,name11);
                assets[id].name=name11;
            }else if(blank == "3"){
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
                
                assets[id].amount=amt;
            }else{
                cout<<"Invalid choice.Try again"<<endl;
            }
            Assets::writeToCSVforUpdating(assets);
        }

        static void deleteAsset(vector<Assets>& assets) {
            Assets::id_giver(assets);
            int counter = Assets::get_counter();
            int index;
            while (true) {
                cout << "Enter the id of Asset you want to delete : ";
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
                
                    assets.erase(assets.begin() + index);
                    Assets::writeToCSVforUpdating(assets);
                    cout<<"Deleted Successfully"<<endl;
                }   
            }
        
       
        }

        static void id_giver(vector<Assets>&assets){
            int counter = Assets::get_counter();
            Assets::loadFromCSV(assets);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                assets[i].id = idd;
                idd++;
                
            }

        }

        static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_assets.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
        }   

};
string Assets::username;

class Liabilities{
    public:
        string date;
        double amount;
        string name;
        static string username;
        int id;
        Liabilities(string date,double amt,string name):date(date),amount(amt),name(name){}

        static void addLiabilities(vector<Liabilities>&liabilities,string date,double amount,string name){
            liabilities.emplace_back(date,amount,name);
            ///write to csv
            Liabilities::writeToCSV(date,amount,name);
        }

        static void setUsername(const string &user) {
            username = user;
        }

        static double calculateTotalLiability( vector<Liabilities>& liabilities, const string& startDate, const string& endDate) {
            Liabilities::loadFromCSV(liabilities);
            tm startTm = stringToDate3(startDate);
            tm endTm = stringToDate3(endDate);
            double total = 0.0; 

            for (const auto& lia: liabilities) {
                tm liaDate = stringToDate3(lia.date);
            

                if ((mktime(&liaDate) >= mktime(&startTm)) && (mktime(&liaDate) <= mktime(&endTm))) {
                    total += lia.amount;
                }
            }

            return total;
        }

        static void writeToCSV(string date,double amount,string name) {
            ofstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_liabilities.csv", ios::app);
            if (file.is_open()) {
                file << date << "," <<name << "," << amount  << "\n";
                file.close();
            }else {
            std::cerr << "Failed to open file "  << endl;
            }
        }

        static void writeToCSVforUpdating(vector<Liabilities>&liabilities){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_liabilities.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Date,Name,Amount\n";
                for(const auto & lia:liabilities){
                    file<<lia.date<<","<<lia.name<<","<<lia.amount<<"\n";
                }
                file.close();
                cout<<"Updated Successfully"<<endl;
            } else {
                cerr << "Unable to open file." << endl;
            }
                 
        }
        static void loadFromCSV(vector<Liabilities>&liabilities) {
            liabilities.clear();
            ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_liabilities.csv");
            string line;
            if (file.is_open()) {
                getline(file, line);
                while (getline(file, line)) {
                    stringstream ss(line);
                    string date, name,amt_str;
                    
                    getline(ss, date, ',');
                    getline(ss, name, ',');
                    getline(ss, amt_str, ',');
                    double amt = stod(amt_str);
                    liabilities.emplace_back(date,amt,name);
                }
                file.close();
              
                
            } else {
                cerr << "Failed to open the file "  << endl;
            }
        }

        static void viewLiabilities(vector<Liabilities>&liabilities){

            Liabilities::id_giver(liabilities);
            // Set up the table headers
            cout << left
                << setw(10) << "ID"
                 << setw(15) << "Date"
                << setw(25) << "Name"
                << setw(15) << "Amount" << endl;
            cout << string(65, '-') << endl;  // Table separator

            // Display each liability in a formatted way
            for (const auto& lia : liabilities) {
                cout << setw(10) << lia.id
                    << setw(15) << lia.date
                    << setw(25) << lia.name
                    << setw(15) << fixed << setprecision(2) << lia.amount << endl;
            }
        }

        static void forReport(vector<Liabilities>&liabilities,tm startDate,tm endDate){
            Liabilities::loadFromCSV(liabilities);
            for (const auto& l : liabilities){
                tm lDate = stringToDate3(l.date);
                if ((mktime(&lDate) >= mktime(&startDate)) && (mktime(&lDate) <= mktime(&endDate))){
                    cout << setw(15) << l.date
                        << setw(25) << l.name
                        << setw(15) << fixed << setprecision(2) << l.amount << endl;
                }
            }

        }

        static void editLiability(vector<Liabilities>& liabilities) {
            Liabilities::id_giver(liabilities);
            int id;
            string blank;
            int counter =Liabilities::get_counter();
            while (true) {
                cout << "Enter the id of Liability you want to change : ";
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
            
        
            cout<<"Which part do you want to change? 1. Date or 2. Name or 3. Amount (please only enter 1 to 3)";
            cin>>blank;
            if(blank == "1"){
                string date11;
                regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
                while (true) {
                    cout << "Please enter new date (dd.mm.yy): ";
                    cin >> date11;

                    // Check if the input matches the date pattern
                    if (regex_match(date11, date_pattern)) {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                    }
                }
                liabilities[id].date=date11;
            }else if(blank == "2"){
                string name11;
                cout<<"Enter the name of liability you want to change ";
                cin.ignore();
                getline(cin,name11);
                liabilities[id].name=name11;
            }else if(blank == "3"){
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
                liabilities[id].amount=amt;
            }else{
                cout<<"Invalid choice.Try again"<<endl;
            }
            Liabilities::writeToCSVforUpdating(liabilities);
        }

        static void deleteLiability(vector<Liabilities>& liabilities) {
            Liabilities::id_giver(liabilities);
            int counter = Liabilities::get_counter();
            int index;
            while (true) {
                cout << "Enter the id of Liability you want to delete : ";
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
                
                    liabilities.erase(liabilities.begin() + index);
                    Liabilities::writeToCSVforUpdating(liabilities);
                    cout<<"Deleted Successfully"<<endl;
                }   
            }
        
       
        }

        static void id_giver(vector<Liabilities>&liabilities){
            int counter = Liabilities::get_counter();
            Liabilities::loadFromCSV(liabilities);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                liabilities[i].id = idd;
                idd++;
                
            }

        }

        static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_liabilities.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
        }   

};
string Liabilities::username;