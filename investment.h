#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <regex>
using namespace std;
tm stringToDate1(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d.%m.%Y");
     

   // Adjust the year (because %y assumes years since 1900)
    tm.tm_year += (tm.tm_year < 100) ? 100 : 0;  // Adjust for years like 24 to mean 2024
    return tm;
}


class Investment{
    protected:
        string purchased_date;
        int units;
        string name;
        double purchased_price; 
        double current_price;
        double total_current_value ;
        double total_cost ;
        static string username;
        double roi;
        int id;
    
    
    public:
        Investment(string date,int units,string name,double purchased_price,double current_price, double t_cost,double t_value,double roi):
        purchased_date(date),units(units),name(name),purchased_price(purchased_price),current_price(current_price),total_current_value(t_value), total_cost(t_cost),roi(roi){}

      

        static void setUsername(const std::string &username) {
            Investment::username = username;
        };
        static void addInvestment(vector<Investment>&investments,string &date,int units,string &name,double purchased_price,double current_price){
            double total_current_value = units * current_price;
            double total_cost = units * purchased_price;
            double roi = ((total_current_value - total_cost)/total_cost)*100;
            investments.emplace_back(date,units,name,purchased_price,current_price, total_cost,total_current_value,roi);
            saveToCsv(date,units,name,purchased_price,current_price, total_cost,total_current_value,roi);
        };


        static void displayDataOfInvestments(vector <Investment>& investments) {

            // Call the id_giver function
            Investment::id_giver(investments);
    
            // Display table headers
            cout << left << setw(5) << "ID"
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

            // Display each investment in table format
            for (const auto& investment : investments) {
                cout << left << setw(5) << investment.id 
                    << setw(15) << investment.purchased_date 
                    << setw(10) << investment.units 
                    << setw(15) << investment.name 
                    << setw(18) << fixed << setprecision(2) << investment.purchased_price 
                    << setw(15) << investment.current_price 
                    << setw(18) << investment.total_cost
                    << setw(15) << investment.total_current_value 
                    << setw(22) << fixed << setprecision(2) << investment.roi << "%" << endl;
            }
        }

        static void forReport1(vector<Investment>&investments,tm startDate,tm endDate){
            Investment::load_data_from_csv(investments);
            for(const auto & investment : investments){
                tm invDate = stringToDate1(investment.purchased_date);
                if ((mktime(&invDate) >= mktime(&startDate)) && (mktime(&invDate) <= mktime(&endDate))) {
                    cout << left 
                        << setw(15) << investment.purchased_date 
                        << setw(10) << investment.units 
                        << setw(15) << investment.name 
                        << setw(18) << fixed << setprecision(2) << investment.purchased_price 
                        << setw(15) << investment.current_price 
                        << setw(18) << investment.total_cost
                        << setw(15) << investment.total_current_value 
                        << setw(22) << fixed << setprecision(2) << investment.roi << "%" << endl;

                }
            }
        }

        
        /*static void calculate_total_value(vector<Investment>&investments){
            Investment::load_data_from_csv(investments);
            int counter = Investment::get_counter();
            
            double total_value = 0.0;
            for(int i =0;i<counter;i++){
                total_value += investments[i].total_current_value;
                
            }
            cout<<"Your Current Value of all Invesments is "<<total_value<<endl;
        } */
        static void calculate_total_value( vector<Investment>& investments, const string& startDate, const string& endDate) {
            Investment::load_data_from_csv(investments);
            tm startTm = stringToDate1(startDate);
            tm endTm = stringToDate1(endDate);
            double total_value = 0.0; 

            for (const auto& investment : investments) {
                tm investmentDate = stringToDate1(investment.purchased_date);
            

                if ((mktime(&investmentDate) >= mktime(&startTm)) && (mktime(&investmentDate) <= mktime(&endTm))) {
                    total_value += investment.total_current_value;
                }
            }   

            cout<<"Your Current Value of all Invesments between "<<startDate<<" "<<"and "<<endDate<<" is "<<total_value<<endl;
        }
        static void get_roi(vector<Investment>&investments){
            Investment::load_data_from_csv(investments);
            for (const auto& investment : investments) {
                cout << "Date: " << investment.purchased_date  << ", Return On Investment: "<<investment.roi<<"%"<<endl;
                if(investment.roi <0){
                    cout<<"Warning! your investment of "<<investment.purchased_date<<" is below 0% "<<endl;
                }
            }
            
        }

        static void edit_investments(vector<Investment>&investments){
            Investment::id_giver(investments);
            int id;
            string blank;
            int counter = Investment::get_counter();
            while (true) {
                cout << "Enter the id of Investment you want to change : ";
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
            
            cout<<"Which part do you want to change? 1.Purchased Date or 2.Units or 3.Investment name or 4.Purchased Price or 5.Current Price (please only enter 1 to 5)";
            cin>>blank;
            if(blank == "1"){
                string date11;
                regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
                while (true) {
                    cout << "Please enter the new deadline (dd.mm.yy): ";
                    cin >> date11;

                    // Check if the input matches the date pattern
                    if (regex_match(date11, date_pattern)) {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                    }
                }
                investments[id].purchased_date=date11;
            }else if(blank == "2"){
                int units11;
                while (true) {
                    cout << "Please enter units: ";
                    cin >> units11;

                    // Check if the input is valid
                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << "Incorrect data. Please enter a valid amount.\n";
                    } else {
                        if(units11<=0){
                            cout<<"must be greater than 0"<<endl;
                        }else{
                            break;
                        }
                    }
                }
                investments[id].units=units11;
                investments[id].total_current_value = units11 * investments[id].current_price;
                investments[id].total_cost = units11 * investments[id].purchased_price;
                investments[id].roi =((investments[id].total_current_value - investments[id].total_cost)/investments[id].total_cost)*100;
            }else if(blank == "3"){
                string name11;
                cout<<"Enter the name of investment ";
                cin.ignore();
                getline(cin,name11);
                investments[id].name=name11;
            }else if(blank == "4"){
                int p_price11;
                while (true) {
                    cout << "Please enter new purchased price: ";
                    cin >> p_price11;

                    // Check if the input is valid
                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << "Incorrect data. Please enter a valid amount.\n";
                    } else {
                        if(p_price11<=0){
                            cout<<"must be greater than 0"<<endl;
                        }else{
                            break;
                        }
                    }
                }
                investments[id].purchased_price = p_price11;
                investments[id].total_current_value = investments[id].units * investments[id].current_price;
                investments[id].total_cost = investments[id].units * investments[id].purchased_price;
                investments[id].roi =((investments[id].total_current_value - investments[id].total_cost)/investments[id].total_cost)*100;
            }else if(blank == "5"){
                int c_price11;
                while (true) {
                    cout << "Please enter new current price: ";
                    cin >> c_price11;

                    // Check if the input is valid
                    if (cin.fail()) {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << "Incorrect data. Please enter a valid amount.\n";
                    } else {
                        if(c_price11<=0){
                            cout<<"must be greater than 0"<<endl;
                        }else{
                            break;
                        }
                    }
                }
                investments[id].current_price = c_price11;
                investments[id].total_current_value = investments[id].units * investments[id].current_price;
                investments[id].total_cost = investments[id].units * investments[id].purchased_price;
                investments[id].roi =((investments[id].total_current_value - investments[id].total_cost)/investments[id].total_cost)*100;
            }else{
                cout<<"Invalid choice.Try again"<<endl;
            }
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_investment.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Purchased Date,Units,Investment name,Purchased Price,Current Price,Total Purchased Price,Total Current Value,Roi(%)\n";
                for(int i = 0 ;i<counter;i++){
                        file << investments[i].purchased_date << ',' << investments[i].units << ',' << investments[i].name << ',' << investments[i].purchased_price<<','<<investments[i].current_price<<','<<investments[i].total_cost<<','<<investments[i].total_current_value<<','<<investments[i].roi <<'\n';
                    
                }
        
                file.close();
               
                cout<<"Edited Successfully"<<endl;
            } else {
                cerr << "Unable to open file." << endl;
            }

            
        }


        static void delete_investments(vector <Investment>&investments){
            Investment::id_giver(investments);
            int counter = Investment::get_counter();
            int index;
            while (true) {
                cout << "Enter the id of Investment you want to delete : ";
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
                }else{
                    investments.erase(investments.begin() + index);
                    counter = Investment::get_counter()-1;
                    ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_investment.csv", ios::trunc);
                    if (file.is_open()) {
        
                        file << "Purchased Date,Units,Investment name,Purchased Price,Current Price,Total Purchased Price,Total Current Value,Roi(%)\n";
                        for(int i = 0 ;i<counter;i++){
                                file << investments[i].purchased_date << ',' << investments[i].units << ',' << investments[i].name << ',' << investments[i].purchased_price<<','<<investments[i].current_price<<','<<investments[i].total_cost<<','<<investments[i].total_current_value<<','<<investments[i].roi <<'\n';
                    
                        }
        
                        file.close();
                    } else {
                        cerr << "Unable to open file." << endl;
                    }
                    Investment::id_giver(investments);
                    cout<<"Deleted Successfully"<<endl;
                }   
            }
        }

        /*static void get_total_profit(vector<Investment>investments){
            int counter = Investment::get_counter();
            Investment::load_data_from_csv(investments);
            
            double total_total_cost = 0.0;
            double total_total_current_price = 0.0;
            for(int i =0;i<counter;i++){
                total_total_cost += investments[i].total_cost;
                total_total_current_price += investments[i].total_current_value;
                
            }
            cout<<"Your total profit of all Invesments is "<<total_total_current_price-total_total_cost<<endl;
        }*/
        static void calculateTotalProfit( vector<Investment>& investments, const string& startDate, const string& endDate) {
            Investment::load_data_from_csv(investments);
            tm startTm = stringToDate1(startDate);
            tm endTm = stringToDate1(endDate);
            double total_profit ;
            
            double total_total_cost = 0.0;
            double total_total_current_price = 0.0;

            for (const auto& investment : investments) {
                tm investmentDate = stringToDate1(investment.purchased_date);
            

                if ((mktime(&investmentDate) >= mktime(&startTm)) && (mktime(&investmentDate) <= mktime(&endTm))) {
                    total_total_cost += investment.total_cost;
                    total_total_current_price += investment.total_current_value;
                }
            }
            total_profit=total_total_current_price-total_total_cost;

            cout<<"your total profit between "<<startDate<<" "<<"and "<<endDate<<" is "<<total_profit<<endl;
        }

        static double forReport2( vector<Investment>& investments, const string& startDate, const string& endDate) {
            Investment::load_data_from_csv(investments);
            tm startTm = stringToDate1(startDate);
            tm endTm = stringToDate1(endDate);
            double total_profit ;
            
            double total_total_cost = 0.0;
            double total_total_current_price = 0.0;

            for (const auto& investment : investments) {
                tm investmentDate = stringToDate1(investment.purchased_date);
            

                if ((mktime(&investmentDate) >= mktime(&startTm)) && (mktime(&investmentDate) <= mktime(&endTm))) {
                    total_total_cost += investment.total_cost;
                    total_total_current_price += investment.total_current_value;
                }
            }
            return total_profit=total_total_current_price-total_total_cost;

            
        }


            
        
    public :
        static void load_data_from_csv(vector<Investment>&investments){
            investments.clear();
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_investment.csv");
            string line;
            
            getline(file,line);
            while(getline(file,line)){
                stringstream ss(line);
                string date, name, units_str, p_price_str, c_price_str, t_purchased_str, t_value_str,roi;
        
                getline(ss, date, ',');
                getline(ss, units_str, ',');
                getline(ss, name, ',');
                getline(ss, p_price_str, ',');
                getline(ss, c_price_str, ',');
                getline(ss, t_purchased_str, ',');
                getline(ss, t_value_str, ',');
                getline(ss, roi, ',');
                
                
        
                // Convert string values to appropriate types
                int units = stoi(units_str);
                double purchased_price = stod(p_price_str);
                double current_price = stod(c_price_str);
                double total_purchased_price = stod(t_purchased_str);
                double total_current_value = stod(t_value_str);
                double r_o_i = stod(roi);
                investments.emplace_back(date, units, name, purchased_price, current_price, total_purchased_price, total_current_value,r_o_i);
                
            }
        };
        //string &date,int units,string &name,double purchased_price,double current_price, double total_cost,double total_current_value,double roi
        static void saveInvestmentToCSV(vector<Investment>&investments){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_investment.csv",ios::app);
            if(file.is_open()){
                int counter = Investment::get_counter();
                if(counter == 0){
                    file << investments[0].purchased_date << ',' << investments[0].units << ',' << investments[0].name << ',' << investments[0].purchased_price<<','<<investments[0].current_price<<','<<investments[0].total_cost<<','<<investments[0].total_current_value<<','<<investments[0].roi <<'\n';
                }else{
                    for(int i = 0 ;i<counter;i++){
                        file << investments[i].purchased_date << ',' << investments[i].units << ',' << investments[i].name << ',' << investments[i].purchased_price<<','<<investments[i].current_price<<','<<investments[i].total_cost<<','<<investments[i].total_current_value<<','<<investments[i].roi <<'\n';
                    
                    }
                }
                
                file.close();

                /*file << date << ',' << units << ',' << name << ',' << purchased_price<<','<<current_price<<','<<total_cost<<','<<total_current_value<<','<<roi <<'\n';
                file.close();*/
            } else {
            std::cerr << "Unable to open file for writing: " << username + "/investment.csv" << std::endl;
            }
        };
        static void saveToCsv(string &date,int units,string &name,double purchased_price,double current_price, double total_cost,double total_current_value,double roi){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_investment.csv",ios::app);
            if(file.is_open()){
                file << date << ',' << units << ',' << name << ',' << purchased_price<<','<<current_price<<','<<total_cost<<','<<total_current_value<<','<<roi <<'\n';
                file.close();
        } else {
            std::cerr << "Unable to open file for writing: " << username + "/investment.csv" << std::endl;
            }
        }

        static void id_giver(vector<Investment>&investments){
            int counter = Investment::get_counter();
            Investment::load_data_from_csv(investments);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                investments[i].id = idd;
                idd++;
                
            }

        }


        static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_investment.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
        }
};
string Investment::username;
