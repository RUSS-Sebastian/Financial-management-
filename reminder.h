#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <map>
#include <iomanip>
#include <vector>
#include <fstream>
#include <regex>
#include <limits>
using namespace std;
tm stringToDate0(const string& dateStr) {
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d.%m.%Y");
     

    // Adjust the year (because %y assumes years since 1900)
    tm.tm_year += (tm.tm_year < 100) ? 100 : 0;  // Adjust for years like 24 to mean 2024
    return tm;
}
// Function to convert tm structure to string in dd.mm.yy format
string tmToString(const tm& date) {
    ostringstream oss;
    oss <<put_time(&date, "%d.%m.%y");
    return oss.str();
}
/*string tmToString(const tm& date) {
    char buffer[11];
    strftime(buffer,sizeof(buffer),"%d.%m.%y",&date);
    return string(buffer);
};*/

/*std::ostringstream: Used to construct the string with 
formatted output.std::setw(2): Ensures that each field (day, month, and year) 
is at least two digits wide.std::setfill('0'): Fills any empty space with 0 (e.g., "5" becomes "05")
.date.tm_mday: Day of the month.date.tm_mon + 1: Month (adjusted from 0-based to 1-based index).
date.tm_year % 100: Last two digits of the year.
The function returns a string formatted as dd.mm.yy.*/
class Reminder {
    public:
        string description;
        tm dueDate; // Time structure for due date
        string isRecurring;
        int frequency; // Frequency in days for recurring reminders
        static string username;
        int id;
        Reminder(const string& desc, const tm& date, string recurring , int freq ): description(desc), dueDate(date), isRecurring(recurring), frequency(freq) {}
        

        static void setUsername(const string &user) {
            username = user;
        }

        

        static void addReminder(vector<Reminder>&reminders,const string& description, const tm& dueDate, string isRecurring, int frequency) {
            reminders.emplace_back(description,dueDate,isRecurring,frequency);
            string dueDate_str = tmToString(dueDate);
            Reminder::saveToCsv(description,dueDate_str,isRecurring,frequency);

        }

        static void saveToCsv(const string& description, const string& dueDate_str, string isRecurring , int frequency ){//careful no &
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_reminder.csv",ios::app);
            if(file.is_open()){
                file << description << ',' << dueDate_str << ',' << isRecurring<<','<<frequency<< '\n';
                file.close();
            }else {
                cerr << "Unable to open file for writing " << endl;
            }
        }


        static void loadRemindersFromCSV(vector<Reminder>& reminders) {
            reminders.clear();
            ifstream file("D:/Personal FInance Management/.vscode/" + username + "/" + username + "_reminder.csv");
        

            string line;
            getline(file, line); // Skip the header line

            while (getline(file, line)) {
                string des,due,option,frequency;
                stringstream ss(line);
                getline(ss, des, ',');
                getline(ss, due, ',');
                getline(ss,option, ',');
                getline(ss,frequency, ',');
                tm dueDate11 = stringToDate0(due);
                int frequency1 = stoi(frequency);
                reminders.emplace_back(des,dueDate11,option,frequency1);
            }

            file.close();
        }


        static void viewReminders(vector<Reminder>&reminders){
            Reminder::id_giver(reminders);
            cout << left
                << setw(10) << "ID"
                << setw(30) << "Description"
                << setw(15) << "Due Date" << endl;
            cout << string(55, '-') << endl;  // Table separator

            // Display each reminder in a formatted way
            for (const auto& reminder : reminders) {
                string due = tmToString(reminder.dueDate);
                cout << setw(10) << reminder.id
                    << setw(30) << reminder.description
                    << setw(15) << due << endl;
            }
        }

        static void id_giver(vector<Reminder>&reminders){
            int counter = Reminder::get_counter();
            Reminder::loadRemindersFromCSV(reminders);
            int idd = 0;
            for(int i = 0;i<counter;i++){
                reminders[i].id = idd;
                idd++;
                
            }

        }

        static int get_counter(){
            int counter=0;
            ifstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_reminder.csv");
            string line;
            
            getline(file,line);
            
            while(getline(file,line)){
                counter++;
            }
            
            return counter;
        }

        static void deleteReminders(vector<Reminder>&reminders){
            Reminder::id_giver(reminders);
            int counter = Reminder::get_counter();
            int index;
            while (true) {
                cout << "Enter the id of Reminder you want to delete : ";
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
                
                    reminders.erase(reminders.begin() + index);
                    Reminder::writeToCSVforUpdating(reminders);

                }

            }

        }

        static void updateDueDate(vector<Reminder>&reminders){
            Reminder::id_giver(reminders);
            string date11;
            int counter = Reminder::get_counter();
            int id;
            while (true) {
                cout << "Enter the id of Reminder you want to change : ";
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
            
            regex date_pattern(R"(\d{2}\.\d{2}\.\d{2})");
            while (true) {
                cout << "Update the due Date (dd.mm.yy): ";
                cin >> date11;

                // Check if the input matches the date pattern
                if (regex_match(date11, date_pattern)) {
                    break; // Exit the loop if the input is valid
                } else {
                    cout << "Incorrect format. Please enter the date in the format dd.mm.yy.\n";
                }
            }
            

            reminders[id].dueDate=stringToDate0(date11);
            Reminder::writeToCSVforUpdating(reminders);
            
        }

        static void updateFrequency(vector<Reminder>&reminders){
            Reminder::id_giver(reminders);
            int f;
            int id;
            int counter = Reminder::get_counter();
            while (true) {
                cout << "Enter the id of Reminder you want to change : ";
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
            
            while (true) {
                cout << "Update the frequency : ";
                cin >> f;

                // Check if the input is valid
                if (cin.fail()) {
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Incorrect data. Please enter a valid amount.\n";
                } else {
                    if(f<=0){
                        cout<<"must be greater than 0"<<endl;
                    }else{
                        break;
                    }
                }
            }
            reminders[id].frequency=f;
            Reminder::writeToCSVforUpdating(reminders);
            
        }

        static void writeToCSVforUpdating(vector<Reminder>&reminders){
            ofstream file("D:/Personal FInance Management/.vscode/"+username+"/"+username+"_reminder.csv", ios::trunc);
            if (file.is_open()) {
        
                file << "Description,Due Date,Is Recurring,Frequency\n"; 
                for(const auto & reminder:reminders){
                    string due_str = tmToString(reminder.dueDate);
                    file<<reminder.description<<","<<due_str<<","<<reminder.isRecurring<<","<<reminder.frequency<<"\n";
                }
                file.close();
                
            } else {
                cerr << "Unable to open file." << endl;
            }
                 
        }

        void updateDueDate1 ( ) {
            
            /*dueDate.tm_mday+=frequency; // Reschedule the reminder for its next occurrence
            time_t time = mktime(&dueDate);
            tm* convertedDate = localtime(&time);
            dueDate = *convertedDate;*/
            // Convert tm structure to time_t
            time_t timeSinceEpoch = mktime(&dueDate);
            timeSinceEpoch += frequency * 24 * 60 * 60;// Add days in seconds
            tm *newTm = localtime(&timeSinceEpoch);// Convert back to tm structure
            dueDate = *newTm;
        };
};
string Reminder::username;