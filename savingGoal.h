#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include<limits>
using namespace std;

class SavingGoal{

    public:
        string description;
        static string username;
        int id,priority;
        double Target,Current,Progress;
        tm deadline;

        SavingGoal(string des,tm dead,double t,double c,double p,int pr):description(des),deadline(dead),Target(t),Current(c),Progress(p),priority(pr){}
        

        static void setUsername(const string &user) {
            username = user;
        }

        static void setGoal(vector<SavingGoal>&goals,string &des,tm dead,double t,double c = 0.0){
            double current = c;
            double progress = (current/t)*100;
            int pr = 0;
            goals.emplace_back(des,dead,t,current,progress,pr);
            SavingGoal::saveToCsv(des,dead,t,current,progress, pr);
        };

        static void sortGoalsByDeadline(vector<SavingGoal>& goals) {
            sort(goals.begin(), goals.end(), [](SavingGoal& a,  SavingGoal& b) {
                // Primary criterion: Deadline
        
                if (mktime(&a.deadline)!= mktime(&b.deadline)) {
                    return mktime(&a.deadline) < mktime(&b.deadline);
                }
                // Secondary criterion: Target Amount (descending)
                return a.Target > b.Target;
            });

            for(int i = 0;i<goals.size();i++){
                goals[i].priority = i+1;
            }
        }
        static void saveToCsv(string &des,tm dead,double t,double c, double p,int pr){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_goal.csv",ios::app);
            if(file.is_open()){
                string dead_str = tmToString(dead);
                file << des << ',' << dead_str  << ',' << t << ',' << c<<','<<p<<','<<pr <<'\n';
                file.close();
            } else {
                cerr << "Unable to open file for writing "<< endl;
            }
        }

        static void displayGoals(vector<SavingGoal>&goals){
             // Display goals in table format
            
            SavingGoal::id_giver(goals);
            cout << left << setw(30) << "Description" <<setw(10)<<"ID"
                << setw(15) << "Deadline"
                << setw(15) << "Target Value"
                << setw(15) << "Current Value"
                << setw(15) << "Progress"
                << setw(10) << "Priority"
                << endl;

            cout << string(90, '-') << endl; // Separator

            for (const auto& goal : goals) {
                string dead_str = tmToString(goal.deadline);
                cout << left <<setw(30) << goal.description<<setw(10)<<goal.id
                    << setw(15) << dead_str
                    <<setw(15) << goal.Target
                    << setw(15) << goal.Current
                    << setw(15) << goal.Progress
                    << setw(10) << goal.priority
                    << endl;
            }
        }

        static void loadGoalsFromCSV(vector<SavingGoal>& goals) {
            goals.clear();
            ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_goal.csv");
        

            string line;
            getline(file, line); // Skip the header line

            while (getline(file, line)) {
                string des,dead,t,c,p,pr;
                stringstream ss(line);
                getline(ss, des, ',');
                getline(ss, dead, ',');
                getline(ss,t, ',');
                getline(ss,c,',');
                getline(ss,p,',');
                getline(ss,pr,',');
                tm dead1 = stringToDate0(dead);
                int pr1 = stoi(pr);
                double t22 = stod(t);
                double c22 = stod(c);
                double p22 = stod(p);
                goals.emplace_back(des,dead1,t22,c22,p22,pr1);
            }

            file.close();
        }


        static void id_giver(vector<SavingGoal>&goals){
            int counter = SavingGoal::get_counter();

            SavingGoal::loadGoalsFromCSV(goals);
            
            int idd = 0;
            for(int i = 0;i<counter;i++){
                goals[i].id = idd;
                idd++;
                
            }

        }

        static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_goal.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
        }

        static void writeToCSVforUpdating(vector<SavingGoal>&goals){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_goal.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Description,Deadline,Target Amount,Current Amount,Progress(%),Priority\n"; 
                for(const auto & goal:goals){
                    string dead_str = tmToString(goal.deadline);
                    file<<goal.description<<","<<dead_str<<","<<goal.Target<<","<<goal.Current<<","<<goal.Progress<<","<<goal.priority<<"\n";
                }
                file.close();
                
            } else {
                cerr << "Unable to open file." << endl;
            }
                 
        }

        static void editGoal(vector<SavingGoal>& goals) {
            SavingGoal::id_giver(goals);
            int id;
            string blank;
            int counter = SavingGoal::get_counter();
            while (true) {
                cout << "Enter the id of Goal you want to change : ";
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
            
        
            cout<<"Which part do you want to change? 1. Description or 2. Deadline or 3. Target Amount  (please only enter 1 to 3)";
            cin>>blank;
            if(blank == "1"){
                string des11;
                cout<<"Enter Description : ";
                cin.ignore();
                getline(cin,des11);
                goals[id].description=des11;
            }else if(blank == "2"){
                string dead;
                regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
                while (true) {
                    cout << "Please enter the new deadline (dd.mm.yy): ";
                    cin >> dead;

                    // Check if the input matches the date pattern
                    if (regex_match(dead, date_pattern)) {
                        break; // Exit the loop if the input is valid
                    } else {
                        cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                    }
                }
                
                tm dead11 = stringToDate0(dead);
                goals[id].deadline=dead11;
                SavingGoal::sortGoalsByDeadline(goals);
            }else if(blank == "3"){
                double t;
                while (true) {
                    cout << "Please enter target amount: ";
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
                goals[id].Progress = (goals[id].Current/t)*100;
                goals[id].Target=t;
            }else{
                cout<<"Invalid choice.Try again"<<endl;
            }

            
            SavingGoal::writeToCSVforUpdating(goals);

        
        }

        static void deleteGoals(vector<SavingGoal>&goals){
            SavingGoal::id_giver(goals);
            int counter = SavingGoal::get_counter();
            int index;
            while (true) {
                cout << "Enter the id of Goal you want to delete : ";
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
                
                    goals.erase(goals.begin() + index);
                    SavingGoal::sortGoalsByDeadline(goals);
                    SavingGoal::writeToCSVforUpdating(goals);
                }

            }

        }
        static void forCase1(vector<SavingGoal>&goals){
            time_t now = time(nullptr); // Get the current times
             
            SavingGoal::loadGoalsFromCSV(goals);
            double amt;
            for(auto& goal:goals){
                if(goal.priority == 1){
                    time_t end = mktime(&goal.deadline);
                    if(now > end){
                        cout<<"this saving goal is expired"<<endl;
                        goals.erase(remove_if(goals.begin(),goals.end(),[](const SavingGoal & g){return g.priority == 1;}),goals.end());
                        SavingGoal::sortGoalsByDeadline(goals);

                    }else{
                        while (true) {
                            cout << "Please enter the amount you want to contribute: ";
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
                        
                        goal.Current += amt;
                        goal.Progress = (goal.Current/goal.Target)*100;
                        if(goal.Current >= goal.Target){
                            cout<<"Congratulations!!! your goal "<<goal.description<<" has achieved!"<<endl;
                            goals.erase(remove_if(goals.begin(),goals.end(),[](const SavingGoal & g){return g.priority == 1;}),goals.end());
                            SavingGoal::sortGoalsByDeadline(goals);
                        }
                    }

                }
            }
            
            SavingGoal::writeToCSVforUpdating(goals);

        }

        static void forCase2(vector<SavingGoal>&goals){
            SavingGoal::id_giver(goals);
            int id;
            double amt;
            int counter = SavingGoal::get_counter();
            time_t now = time(nullptr); // Get the current times
            while (true) {
                cout << "Enter the id of Goal you want to contribute : ";
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
            
            time_t end = mktime(&goals[id].deadline);
            if(now <= end){
                while (true) {
                    cout << "Please enter the amount you want to contribute: ";
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
                        
                
                goals[id].Current += amt;
                goals[id].Progress = (goals[id].Current/goals[id].Target)*100;
                if(goals[id].Current >= goals[id].Target){
                    cout<<"Congratulations!!! your goal "<<goals[id].description<<" has achieved!"<<endl;
                    goals.erase(remove_if(goals.begin(),goals.end(),[](const SavingGoal & g){return g.Current == g.Target;}),goals.end());
                    SavingGoal::sortGoalsByDeadline(goals);
                }
            }else{
                cout<<"this saving goal is expired"<<endl;
                goals.erase(goals.begin() + id);
                SavingGoal::sortGoalsByDeadline(goals);

            }
            
            SavingGoal::writeToCSVforUpdating(goals);

        }


        static void contribute(vector<SavingGoal>&goals){
            
            int option;
            cout<<endl;
            cout<<endl;
            cout<<"If priority is zero using no.1 will do nothing .Please make sure that you have made automatic priority setter"<<endl;
            cout<<"Do you want to 1. contribute by priority or 2. by your own? (enter 1 - 2): ";

            cin>>option;
            switch(option){
                case 1:

                    SavingGoal::forCase1(goals);
                    break;
                case 2:
                    SavingGoal::forCase2(goals);
                    break;
                default :
                    cout<<"Invalid choice.Try again."<<endl;
                    break;


            }
            
        }


};
string SavingGoal::username;