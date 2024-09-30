#include <iostream>
#include <vector>
#include <cctype> // for isdigit function to check input is digits not chars
#include <string>
#include <conio.h>
#include <fstream>
#include <sstream>//oop in file
#include <stdlib.h>
using namespace std;
int Max_Booking = 30;
int Max_Rooms = 100;
int Max_Users = 30;
const int Max_RoomsBooking = 30;
const int MAX_RATINGS = 30;
struct Days {
    int day;
    // int month;
   // int year;
};
struct Booking {
    int Booking_ID;
    int User_ID;
    int RoomID;
    Days StartDay;
    Days EndDay;
    int StartPeriod;
    int EndPeriod;
    int roomPrice;
};
struct preferences
{
    string location;
    string bed_type;
    string room_meeting;
    string availability_smooking;
};
struct Room {
    int RoomID;
    string RoomType;
    bool Availability[7][3] = { 1, 1, 1,1,1, 1,1, 1, 1,1, 1,1,1,1, 1,1, 1,1,1,1,1 };
    int Price;
    int RoomBookingCount = 0;
    Booking Room_booking[Max_RoomsBooking];
    preferences room_preferences;
    float averageRating[MAX_RATINGS] = {};
    int totalRatings = 0;
    int ratings[MAX_RATINGS];
    string userReviews[MAX_RATINGS];
};
struct User {
    bool IsAdmin = 0;
    string UserName;
    string Email;
    int UserID;
    string Phone_number;
    string Password;
    int UserBooking_counter = 0;
    Booking* Booking_ptr = new Booking[Max_Booking];
    int RatedRooms[Max_RoomsBooking];

}User_info;
Room* hotel_rooms = new Room[Max_Rooms]; // all hotel rooms
int Welcome();
bool Check_UserName(User*, int&, string&);
bool Check_Email(User*, int&, string&);
bool Check_Phone(User*, int&, string&);
bool Check_ID(User*, int&, int&);
bool ISDigits_IdORPhone(string&);
string astrik_Password();
bool Sign_In(User*, int&, User&, int&, bool&);
bool Log_In(User*, int&, int&, bool&);
void ReadData(User*, int&);
void WriteData(User*, int&);
//user functions
void RoomsHandle();
Booking get_date();
void write_data(Room* hotel_rooms);
void read_data(Room* hotel_rooms);
int user_menu();
string check_availability(Booking Date);
string GetDay(int day);
string GetPeriod(int per);
void BookRoom(User* user, Booking, int);
void ViewBooking(User* current_user, int);
void modify(User users[], int u_id);
int modify_menu();
void calculateavgrating(Room*, int, User*);
void RateRoom(Room*, User*);
void logout();
//Admin
int AdminMenu();
int EditMenu();
void AddRoom(int& size, Room*& arr);
void DisplayRoom(Room*& arr, int size);
void EditRoomInfo(Room*&);
void DeleteRoom(int&, Room*&);
void CheckBooking_For_Admin(Room* hotelRooms);
int main()
{
    srand((unsigned)time(NULL));//Make random numbers for id generated only once;
     RoomsHandle();
     //write_data(hotel_rooms);
    //read_data(hotel_rooms);
    int UsersCount = 0, UserID = 0;
    bool AdminORUser = 0;
    User* Users = new User[Max_Users];
    ReadData(Users, UsersCount);
    char endORcontinue = 'y';
    int SignORLog = Welcome();
    if (SignORLog == 1)
    {
        system("cls");
        if (UsersCount == Max_Users) {
            Max_Users += 10;
            User* NewUsers_ptr = new User[Max_Users];
            for (int i = 0; i < Max_Users; i++) {
                NewUsers_ptr[i] = Users[i];
            }
            delete[]Users;
            Users = NewUsers_ptr;
        }
        Sign_In(Users, UsersCount, User_info, UserID, AdminORUser);
    }
    else if (SignORLog == 2)
    {
        system("cls");
        Log_In(Users, UsersCount, UserID, AdminORUser);
    }
    bool Logout_Choise = 0;
    do {
        if (AdminORUser) {
            int ans;
            ans = AdminMenu();
            system("cls"); //clear console after each iteration
            if (ans == 1) {
                AddRoom(Max_Rooms, hotel_rooms);
            }
            else if (ans == 2) {
                DisplayRoom(hotel_rooms, Max_Rooms);
            }
            else if (ans == 3) {
                EditRoomInfo(hotel_rooms);
            }
            else if (ans == 4) {
                DeleteRoom(Max_Rooms, hotel_rooms);
            }
            else if (ans == 5) {
                CheckBooking_For_Admin(hotel_rooms);
            }
            else if (ans == 6) {
                Logout_Choise = 1;
                break;
            }
        }
        else if (!AdminORUser)
        {
            int choise = user_menu();
            system("cls");
            if (choise == 1)
            {
                char ans;
                do
                {
                    //call function book a room
                    Booking Date = get_date();
                    BookRoom(Users, Date, UserID);
                    cout << "Do you want to book another room? (y/n)\n";
                    cin >> ans;
                    system("cls");
                    while (cin.fail() || (ans != 'y' && ans != 'Y' && ans != 'n' && ans != 'N'))
                    {
                        cout << "Invalid input ,please try again:(y/n)\n";
                        cin >> ans;
                    }
                    system("cls");
                } while (ans == 'y' || ans == 'Y');
            }
            else if (choise == 2)
            {
                system("cls");
                ViewBooking(Users, UserID);
            }
            else if (choise == 3)
            {
                system("cls");
                modify(Users, UserID);
            }
            else if (choise == 4)
            {
                if (!AdminORUser)
                {
                    char answer;
                    cout << "Do yo want to rate a room?(y/n)\n";
                    cin >> answer;
                    if (answer == 'Y' || answer == 'y')
                    {
                        RateRoom(hotel_rooms, Users);
                    }
                }
            }
            else if (choise == 5) {
                system("cls");
                Logout_Choise = 1;
                break;
            }
        }
        if (Logout_Choise == 1) {
            break;
        }
        cout << "Press 'Y' or 'y' if you want to do more operations\n";
        cin >> endORcontinue;
    } while (endORcontinue == 'y' || endORcontinue == 'Y');
    
    write_data(hotel_rooms);
    WriteData(Users, UsersCount);
    if (Logout_Choise == 1) {
        logout();
    }
    delete[]Users;
    Users = NULL;
    delete[]hotel_rooms;
    hotel_rooms = NULL;
    system("pause");
    return 0;
}
int Welcome() {
    cout << "                                    Welcome in Gen_56 hotel                        " << endl;
    cout << "1.Sign in\n2.Log in\n\nEnter your choice:\n";
    int choise;
    cin >> choise;
    while (cin.fail() || choise < 1 || choise>2) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid choise , please entre a valid one\n";
        cin >> choise;
    }
    return choise;
    system("cls");
}
void ReadData(User* Users, int& userCounter) {
    userCounter = 0;
    fstream Data;
    Data.open("Users_Data.txt", ios::in);
    if (!Data.is_open()) {
        cout << "File not found\n";
        return;
    }
    string line;
    while (getline(Data, line)) {

        stringstream lineStream(line);
        lineStream >> Users[userCounter].IsAdmin
            >> Users[userCounter].UserName
            >> Users[userCounter].Email
            >> Users[userCounter].UserID
            >> Users[userCounter].Phone_number
            >> Users[userCounter].Password
            >> Users[userCounter].UserBooking_counter;
        for (int i = 0; i < Users[userCounter].UserBooking_counter; i++) {
            lineStream >> Users[userCounter].Booking_ptr[i].User_ID
                >> Users[userCounter].Booking_ptr[i].RoomID
                >> Users[userCounter].Booking_ptr[i].Booking_ID
                >> Users[userCounter].Booking_ptr[i].StartDay.day
                >> Users[userCounter].Booking_ptr[i].EndDay.day
                >> Users[userCounter].Booking_ptr[i].StartPeriod
                >> Users[userCounter].Booking_ptr[i].EndPeriod;
        }
        if (lineStream) {
            userCounter++;
        }
        else {

            cout << "Error can not read line\n";
        }
    }
    Data.close();  // Always close the file
}
void WriteData(User* Users, int& UserCount) {
    fstream Data;
    Data.open("Users_Data.txt", ios::out);
    if (!Data.is_open()) {
        cout << "File doesn't found\n";
        return;
    }
    else {
        for (int i = 0; i < UserCount; i++) {
            Data << Users[i].IsAdmin << " "
                << Users[i].UserName << " "
                << Users[i].Email << " "
                << Users[i].UserID << " "
                << Users[i].Phone_number << " "
                << Users[i].Password << " " << Users[i].UserBooking_counter << " ";
            for (int j = 0; j < Users[i].UserBooking_counter; j++) {
                Data
                    << Users[i].Booking_ptr[j].User_ID << " "
                    << Users[i].Booking_ptr[j].RoomID << " "
                    << Users[i].Booking_ptr[j].Booking_ID << " "
                    << Users[i].Booking_ptr[j].StartDay.day << " "
                    << Users[i].Booking_ptr[j].EndDay.day << " "
                    << Users[i].Booking_ptr[j].StartPeriod << " "
                    << Users[i].Booking_ptr[j].EndPeriod << " ";
            }
            Data << endl;
        }
    }
    Data.close();
}
bool Check_UserName(User* Users, int& Userscount, string& UserName) {
    for (int i = 0; i < Userscount; i++) {
        if (UserName == Users[i].UserName) {
            return 0;
        }
    }
    return 1;
}
bool Check_Email(User* Users, int& Userscount, string& Email) {
    for (int i = 0; i < Userscount; i++) {
        if (Email == Users[i].Email) {
            return 0;
        }
    }
    return 1;
}
bool Check_Phone(User* Users, int& Userscount, string& Phone) {
    for (int i = 0; i < Userscount; i++) {
        if (Phone == Users[i].Phone_number) {
            return 0;
        }
    }
    return 1;
}
bool ISDigits_IdORPhone(string& phoneORId) {
    for (int i = 0; i < phoneORId.size(); i++) {
        if (!isdigit(phoneORId[i])) {
            return 0;
        }
    }
    return 1;
}
bool Check_ID(User* Users, int& Userscount, int & ID) {
    for (int i = 0; i < Userscount; i++) {
        if (Users[i].UserID == ID) {
            return 0;
        }
    }
    return 1;
}
string astrik_Password() {
    char ch;
    string password;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {

                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            cout << "*";
            password.push_back(ch);
        }
    }
    cout << endl;
    return password;
}
bool Sign_In(User* Users, int& Usercount, User& info, int& Userid, bool& AdmORUsr) {

    //Admin or User;
    int AdminORUser;
    system("cls");
    cout << "1.Admin\n";
    cout << "2.User\n\n";
    cout << "Enter your choise\n";
    cin >> AdminORUser;
    while (cin.fail() || AdminORUser < 1 || AdminORUser>2) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalide choise , please entre a valide one\n";
        cin >> AdminORUser;
    }
    if (AdminORUser == 1) {
        info.IsAdmin = 1;
        AdmORUsr = 1;
    }
    else if (AdminORUser == 2) {
        info.IsAdmin = 0;
        AdmORUsr = 0;
    }
    system("cls");
    //UserName
    cout << "Enter Username : " << endl;
    cin >> info.UserName;
    while (!Check_UserName(Users, Usercount, info.UserName)) {
        cout << "This Username is already exist , please entre a Unique one\n";
        cin >> info.UserName;
    }
    //Email
    cout << "Enter Email address : \n";
    cin >> info.Email;
    while (!Check_Email(Users, Usercount, info.Email)) {
        cout << "This Email already exists \n";
        cout << "please log in\n";
        Log_In(Users, Usercount, Userid, info.IsAdmin);
        return 0;
    }
    //Phone number
    cout << "Enter Phone number : \n";
    cin >> info.Phone_number;
    while (!Check_Phone(Users, Usercount, info.Phone_number)) {
        cout << "This Phone number is already exist \n";
        cout << "please log in\n";
        Log_In(Users, Usercount, Userid, info.IsAdmin);
        return 0;
    }
    //verify that input is digits only
    while (!ISDigits_IdORPhone(info.Phone_number)) {
        cout << "Error,Invalid input.\nPlease enter only numeric digits for the phone number.\n";
        cin >> info.Phone_number;
    }
    /*while (info.Phone_number.size() != 11) {
        cout << "Invalid phone number,please try again\n";
        cin >> info.Phone_number;
    }*/
    //Password
    cout << "Enter password : \n";
    info.Password = astrik_Password();
    //id
    info.UserID = rand() % 1000 + 1000;
    while (!Check_ID(Users, Usercount, info.UserID)) {
        info.UserID = rand() % 1000 + 1000;
    }
    Userid = info.UserID;
    Users[Usercount++] = { info.IsAdmin,info.UserName,info.Email,info.UserID,info.Phone_number,info.Password,0 };

    cout << "You're now signed in!\n";
    cout << "Your id : " << info.UserID << "\n\n";
    return 1;
    system("cls");
}
bool Log_In(User* Users, int& Userscount, int& Userid, bool& AdminORUser) {
    string UserName, Password;
    cout << "Enter User name : \n";
    cin >> UserName;
    cout << "Enter password : \n";
    Password = astrik_Password();
    for (int i = 0; i < Userscount; i++) {
        if (UserName == Users[i].UserName && Password == Users[i].Password) {
            Userid = Users[i].UserID;
            AdminORUser = Users[i].IsAdmin;
            cout << "Login Successful!\n\n";
            return 1;
            system("cls");
        }
    }
    int choise;
    cout << "Invalid login \n Press '1' to create new account \n press '2' to try again\n ";
    cin >> choise;
    while (choise < 1 || choise>2 || cin.fail()) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid input ,please try again (1/2)\n ";
        cin >> choise;
    }
    if (choise == 1) {
        Sign_In(Users, Userscount, User_info, Userid, AdminORUser);
        return 0;
    }
    else {
        Log_In(Users, Userscount, Userid, AdminORUser);
    }
}
int user_menu()
{
    cout << "----------User Menu----------\n\n";
    int ans;
    cout << "Choose the numper of which process you want to do:" << endl;
    cout << "1.Book a room\n2.View your booking\n3.Modify or cancel existing bookings\n4.Rate a Room\n5.Exit\n";
    cin >> ans;
    while (ans < 1 || ans>5)
    {
        cout << "Invalid input ,please try again" << endl;
        cin >> ans;
    }
    return ans;
}
void RoomsHandle() {
    //to give the rooms data to test the function according it
    for (int i = 0; i < 100; i++)
    {
        for (int day = 0; day < 7; day++)
        {
            for (int period = 0; period < 3; period++)
            {
                hotel_rooms[i].Availability[day][period] = true;
            }
        }
        hotel_rooms[i].RoomID = i + 1;

        if ((i >= 0) && (i < 20))
        {
            hotel_rooms[i].RoomType = "standard";
            hotel_rooms[i].Price = 1000;
            hotel_rooms[i].room_preferences.location = "Located in the back";
            hotel_rooms[i].room_preferences.bed_type = "Single";
            hotel_rooms[i].room_preferences.availability_smooking = "Non-smoking room";
            hotel_rooms[i].room_preferences.room_meeting = "Non-meeting room";
        }
        else if ((i >= 20) && (i < 40))
        {
            hotel_rooms[i].RoomType = "suite";
            hotel_rooms[i].Price = 2000;
            hotel_rooms[i].room_preferences.location = "Overlooking a swimming pool";
            hotel_rooms[i].room_preferences.bed_type = "Double";
            hotel_rooms[i].room_preferences.availability_smooking = "Non-smoking room";
            hotel_rooms[i].room_preferences.room_meeting = "Non-meeting room";
        }
        else if ((i >= 40) && (i < 60))
        {
            hotel_rooms[i].RoomType = "deluxe";
            hotel_rooms[i].Price = 3000;
            hotel_rooms[i].room_preferences.location = "With a city view";
            hotel_rooms[i].room_preferences.bed_type = "King Size";
            hotel_rooms[i].room_preferences.availability_smooking = "Smoking room";
            hotel_rooms[i].room_preferences.room_meeting = "Meeting room";
        }
        else if ((i >= 60) && (i < 80))
        {
            hotel_rooms[i].RoomType = "executive";
            hotel_rooms[i].Price = 4000;
            hotel_rooms[i].room_preferences.location = "Near the elevator";
            hotel_rooms[i].room_preferences.bed_type = "single";
            hotel_rooms[i].room_preferences.availability_smooking = "Smoking room";
            hotel_rooms[i].room_preferences.room_meeting = "Meeting room";
        }
        else  if ((i >= 80) && (i < 100))
        {
            hotel_rooms[i].RoomType = "family";
            hotel_rooms[i].Price = 5000;
            hotel_rooms[i].room_preferences.location = "Located in the front";
            hotel_rooms[i].room_preferences.bed_type = "King size";
            hotel_rooms[i].room_preferences.availability_smooking = "Non-smoking room";
            hotel_rooms[i].room_preferences.room_meeting = "Non-meeting room";
        }
    }
}
Booking get_date()
{
    Booking date;
    //to take a date of booking
    cout << "1.Saturday\n2.Sunday\n3.Monday\n4.Tuesday\n5.Wedensday\n6.Thursday\n7.Friday\n";
    cout << "Enter start date, please:(1-7) " << endl;
    cin >> date.StartDay.day;
    system("cls");
    while (date.StartDay.day < 1 || date.StartDay.day > 7 || cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid date, please enter again:(1-7)" << endl;
        cin >> date.StartDay.day;
    }
    system("cls");
    cout << "Enter a specific period of a check_in ,please \n";
    cout << "1. Morning\n2. Afternoon\n3. Evening\n";
    cin >> date.StartPeriod;
    while (date.StartPeriod < 1 || date.StartPeriod > 3 || cin.fail())
    {
        cout << "Invalid input, please try again\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> date.StartPeriod;
    }
    system("cls");
    cout << "1.Saturday\n2.Sunday\n3.Monday\n4.Tuesday\n5.Wedensday\n6.Thursday\n7.Friday\n";
    cout << "Enter end date, please:(1-7)" << endl;
    cin >> date.EndDay.day;
    while (date.EndDay.day < 1 || date.EndDay.day > 7 || cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid date, please enter again:(1-7)" << endl;
        cin >> date.EndDay.day;
    }
    system("cls");
    //check that the end_day is greater than the start_day
    while (date.EndDay.day < date.StartDay.day)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input ,please try again" << endl;
        cin >> date.StartDay.day >> date.EndDay.day;
    }
    system("cls");
    cout << "Enter a specific period of a check_out ,please \n";
    cout << "1. Morning\n2. Afternoon\n3. Evening\n";
    cin >> date.EndPeriod;
    while (date.EndPeriod < 1 || date.EndPeriod >3 || cin.fail())
    {
        cout << "Invalid input, please try again\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> date.EndPeriod;
    }
    /* if (date.StartDay.day == date.EndDay.day)
     {
         while (date.EndPeriod <= date.StartPeriod)
         {
             cout << "Invalid period ,please try again\n";
             cout << "Check-in Period:\n"; cin >> date.StartPeriod;
             cout << "Check-out Period:\n"; cin >> date.EndPeriod;
         }
     }*/
    system("cls");
    return date;
}
string check_availability(Booking Date)
{
    // To take the type of room
    int type;
    string room_type;
    cout << "Enter the type of room you want to book, please" << endl;
    cout << "1. Standard Room\n2. Suite\n3. Deluxe\n4. Executive Room\n5. Family Room\n";
    cin >> type;
    system("cls");
    while (cin.fail() || type < 1 || type>5)
    { // To check validation
        cout << "Invalid input, please try again" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> type;
    }
    system("cls");
    if (type == 1)
        room_type = "standard";
    else if (type == 2)
        room_type = "suite";
    else if (type == 3)
        room_type = "deluxe";
    else if (type == 4)
        room_type = "executive";
    else
        room_type = "family";
    // Check availability 
    for (int i = 0; i < Max_Rooms; i++) {
        if (hotel_rooms[i].RoomType == room_type)
        {
            bool available = true;
            for (int day = Date.StartDay.day - 1; day <= Date.EndDay.day - 1; ++day)
            {
                for (int period = 0; period < 3; period++)
                {
                    if (!hotel_rooms[i].Availability[day][period])
                    {
                        if ((day == Date.StartDay.day - 1) && (period < Date.StartPeriod - 1))
                            continue;
                        else if ((day == Date.EndDay.day - 1) && (period > Date.EndPeriod - 1))
                            continue;
                        available = false;
                        break;
                    }
                }
                if (!available) break;
            }
            if (available)
            {
                // Room is available
                cout << "Room ID: " << hotel_rooms[i].RoomID << endl
                    << "Price: " << hotel_rooms[i].Price << endl
                    << "Type: " << hotel_rooms[i].RoomType << endl
                    << "Room Location: " << hotel_rooms[i].room_preferences.location << endl
                    << "Type Of Bed: " << hotel_rooms[i].room_preferences.bed_type << endl
                    << "Other Preferences: " << hotel_rooms[i].room_preferences.availability_smooking << endl
                    << hotel_rooms[i].room_preferences.room_meeting << endl
                    << "----------------------------------\n";
            }
        }
    }
    return room_type;
}
void write_data(Room* hotel_rooms) {
    ofstream out("Rooms_Data.txt");
    if (!out) {
        cout << "Failed to open file for writing." << endl;
        return;
    }
    // Write room data to file
    for (int i = 0; i < Max_Rooms; ++i) {
        out << hotel_rooms[i].RoomID << " "
            << hotel_rooms[i].RoomType << " "
            << hotel_rooms[i].Price << " ";
        for (int day = 0; day < 7; ++day) {
            for (int period = 0; period < 3; ++period) {
                out << hotel_rooms[i].Availability[day][period] << " ";
            }
        }
        out << hotel_rooms[i].room_preferences.location << ","
            << hotel_rooms[i].room_preferences.bed_type << ","
            << hotel_rooms[i].room_preferences.room_meeting << ","
            << hotel_rooms[i].room_preferences.availability_smooking << ","
            << hotel_rooms[i].RoomBookingCount << " ";
        // Write the booking for rooms
        for (int j = 0; j < hotel_rooms[i].RoomBookingCount; j++) {
            out << hotel_rooms[i].Room_booking[j].User_ID << " "
                << hotel_rooms[i].Room_booking[j].Booking_ID << " "
                << hotel_rooms[i].Room_booking[j].StartDay.day << " "
                << hotel_rooms[i].Room_booking[j].EndDay.day << " "
                << hotel_rooms[i].Room_booking[j].StartPeriod << " "
                << hotel_rooms[i].Room_booking[j].EndPeriod << " ";
        }
        out << endl; // End of room data
    }
    out.close();
}

void read_data(Room* hotel_rooms) {
    ifstream in("Rooms_Data.txt");
    if (!in) {
        cout << "Failed to open file for reading." << endl;
        return;
    }
    string line;
    // Allocate memory for dynamic array
    int room_index = 0;
    while (getline(in, line) && room_index < Max_Rooms) {
        istringstream ss(line); // Use istringstream to parse the line

        // Read room ID, type, and price
        ss >> hotel_rooms[room_index].RoomID
            >> hotel_rooms[room_index].RoomType
            >> hotel_rooms[room_index].Price;

        // Read availability array
        for (int day = 0; day < 7; ++day) {
            for (int period = 0; period < 3; ++period) {
                ss >> hotel_rooms[room_index].Availability[day][period];
            }
        }
        // Read room preferences
        getline(ss, hotel_rooms[room_index].room_preferences.location, ',');
        getline(ss, hotel_rooms[room_index].room_preferences.bed_type, ',');
        getline(ss, hotel_rooms[room_index].room_preferences.room_meeting, ',');
        getline(ss, hotel_rooms[room_index].room_preferences.availability_smooking, ',');
        ss >> hotel_rooms[room_index].RoomBookingCount;

        // Read the booking for rooms
        for (int j = 0; j < hotel_rooms[room_index].RoomBookingCount; j++) {
            ss >> hotel_rooms[room_index].Room_booking[j].User_ID
                >> hotel_rooms[room_index].Room_booking[j].Booking_ID
                >> hotel_rooms[room_index].Room_booking[j].StartDay.day
                >> hotel_rooms[room_index].Room_booking[j].EndDay.day
                >> hotel_rooms[room_index].Room_booking[j].StartPeriod
                >> hotel_rooms[room_index].Room_booking[j].EndPeriod;
        }
        room_index++;
    }
    // Close the file
    in.close();
}

void BookRoom(User* Users, Booking Date, int Userid)
{
    string room_type = check_availability(Date);
    int RoomID;
    cout << "Please,Enter ID acoording to the type of room:" << endl;
    cin >> RoomID;
    system("cls");
    if (room_type == "standard")
    {

        while (RoomID < 1 || RoomID>20)
        {
            cout << "Invalid ID ,please try again:";
            cin >> RoomID;
        }
        system("cls");

    }
    else if (room_type == "suite")
    {

        while (RoomID < 21 || RoomID>40)
        {
            cout << "Invalid ID ,please try again:";
            cin >> RoomID;
        }
        system("cls");

    }
    else if (room_type == "deluxe") {

        while (RoomID < 41 || RoomID>60) {
            cout << "Invalid ID ,please try again:";
            cin >> RoomID;
        }
        system("cls");
    }
    else if (room_type == "executive") {

        while (RoomID < 61 || RoomID>80) {
            cout << "Invalid ID ,please try again:";
            cin >> RoomID;
        }
        system("cls");
    }
    else if (room_type == "family") {

        while (RoomID < 81 || RoomID>100) {
            cout << "Invalid ID ,please try again:";
            cin >> RoomID;
        }
        system("cls");
    }
    bool room_found = false;
    for (int i = 0; i < Max_Rooms; i++)
    {
        if (RoomID == hotel_rooms[i].RoomID)
        {
            //checking if the room is availble for booking on the specified dates         
            bool room_available = true;
            for (int day = Date.StartDay.day - 1; day <= Date.EndDay.day - 1; day++)
            {
                for (int p = 0; p < 3; p++)
                {
                    if (!hotel_rooms[i].Availability[day][p]) {
                        if ((day == Date.StartDay.day - 1) && (p < Date.StartPeriod - 1))
                            continue;
                        else if ((day == Date.EndDay.day - 1) && (p > Date.EndPeriod - 1))
                            continue;
                        room_available = false;
                        break;
                    }
                }
                if (!room_available) {
                    break;
                }
            }if (room_available)
            {
                room_found = true;
                //update availability status
                for (int day = Date.StartDay.day - 1; day <= Date.EndDay.day - 1; day++)
                {
                    for (int p = 0; p < 3; p++)
                    {
                        if ((day == Date.StartDay.day - 1) && (p < Date.StartPeriod - 1))
                            continue;
                        else if ((day == Date.EndDay.day - 1) && (p > Date.EndPeriod - 1))
                            continue;
                        hotel_rooms[i].Availability[day][p] = false;
                    }
                }

                int BookingID = 0;
                for (int i = 0; i < Max_Users; i++) {
                    if (Userid == Users[i].UserID) {
                        if (Users[i].UserBooking_counter == Max_Booking) {
                            Max_Booking += 10;
                            Booking* NewBooking_ptr = new Booking[Max_Booking];
                            for (int j = 0; j < Max_Booking; j++) {
                                NewBooking_ptr[i] = Users[i].Booking_ptr[j];
                            }
                            delete[]Users[i].Booking_ptr;
                            Users[i].Booking_ptr = NewBooking_ptr;


                        }
                        int Booking_index = Users[i].UserBooking_counter;
                        Users[i].Booking_ptr[Booking_index].User_ID = Userid;
                        Users[i].Booking_ptr[Booking_index].RoomID = RoomID;
                        Users[i].Booking_ptr[Booking_index].Booking_ID = Userid+Users[i].UserBooking_counter;
                        BookingID = Users[i].Booking_ptr[Booking_index].Booking_ID;// use this for booking id saved in Room booking array  
                        Users[i].Booking_ptr[Booking_index].StartDay.day = Date.StartDay.day - 1;
                        Users[i].Booking_ptr[Booking_index].EndDay.day = Date.EndDay.day - 1;
                        Users[i].Booking_ptr[Booking_index].StartPeriod = Date.StartPeriod - 1;
                        Users[i].Booking_ptr[Booking_index].EndPeriod = Date.EndPeriod - 1;
                        ++Users[i].UserBooking_counter;
                        break;
                    }
                    else {
                        continue;
                    }
                }
                for (int k = 0; k < Max_Rooms; k++) {
                    if (RoomID == hotel_rooms[k].RoomID) {
                        int RoomBooking_index = hotel_rooms[k].RoomBookingCount;
                        hotel_rooms[k].Room_booking[RoomBooking_index].User_ID = Userid;
                        hotel_rooms[k].Room_booking[RoomBooking_index].Booking_ID = BookingID;
                        hotel_rooms[k].Room_booking[RoomBooking_index].StartDay.day = Date.StartDay.day - 1;
                        hotel_rooms[k].Room_booking[RoomBooking_index].EndDay.day = Date.EndDay.day - 1;
                        hotel_rooms[k].Room_booking[RoomBooking_index].StartPeriod = Date.StartPeriod - 1;
                        hotel_rooms[k].Room_booking[RoomBooking_index].EndPeriod = Date.EndPeriod - 1;
                        ++hotel_rooms[k].RoomBookingCount;
                        break;
                    }
                    else {
                        continue;
                    }
                }
                cout << "Room booked succefully\n\n";
            }
        }
    }if (room_found == false) {
        cout << "this room is not available now" << endl;
    }
}
string GetDay(int day) {
    string weekdays[] = { "Saturday","Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
    return weekdays[day];
}
string GetPeriod(int per)
{
    string periods[] = { "Morning", "Afternoon", "Night" };
    return periods[per];
}
void ViewBooking(User* current_user, int Userid) {
    for (int i = 0; i < Max_Users; i++) {
        if (current_user[i].UserID == Userid) {
            if (current_user[i].UserBooking_counter == 0) {
                cout << "No bookings found!" << endl;
                return;
            }
            else {
                // Check if the user has any bookings

                cout << "----------Your Bookings----------\n\n";
                // Iterate through user's booking array
                for (int j = 0; j < current_user[i].UserBooking_counter; j++) {
                    cout << "#Booking " << j + 1 << " :\n";
                    cout << "Booking ID : " << current_user[i].Booking_ptr[j].Booking_ID << endl;
                    cout << "User ID :" << current_user[i].Booking_ptr[j].User_ID << endl;
                    cout << "Room ID : " << current_user[i].Booking_ptr[j].RoomID << endl;
                    cout << "Start Day :" << GetDay(current_user[i].Booking_ptr[j].StartDay.day) << endl;
                    cout << "End Day :" << GetDay(current_user[i].Booking_ptr[j].EndDay.day) << endl;
                    cout << "Check in period : " << GetPeriod(current_user[i].Booking_ptr[j].StartPeriod) << endl; // Display period
                    cout << "Check out period : " << GetPeriod(current_user[i].Booking_ptr[j].EndPeriod) << endl; // Display period
                    cout << "--------------------------------------\n";
                }
            }
            break;
        }
    }
}
int modify_menu()
{
    cout << "Modify reservation menu\n";
    int ans;
    cout << "Please, choose one of the following operations.\n";
    cout << "1. Modify check-in date.\n";
    cout << "2. Modify check-out date.\n";
    cout << "3. Modify check-in date and check-out date.\n";
    cout << "4. Cancel room reservation.\n";
    cin >> ans;
    while (cin.fail() || ans > 4 || ans < 1)
    {
        cout << "Invalid choice. Please, try again\n";
        cin >> ans;
    }
    return ans;
}
void modify(User users[], int u_id)
{
    char as;
    do
    {
        int op = modify_menu(), b_id, book_index, room_index, u_index;

        for (int i = 0; i < Max_Users; i++)
            if (users[i].UserID == u_id)
            {
                u_index = i;
                break;
            }

        cout << "Please, Enter your booking ID\n";
        // input booking id to modify:
        cin >> b_id;
        while (cin.fail())
        {
            cout << "Invalid booking ID. Please, try again\n";
            cin >> b_id;
        }

        // to make sure booking id exists

        bool is_booked = false;
        while (!is_booked)
        {
            for (int i = 0; i < Max_Booking; i++)
            {
                if (b_id == users[u_index].Booking_ptr[i].Booking_ID)
                {
                    is_booked = 1;
                    book_index = i;
                    break;
                }
            }
            if (!is_booked)
            {
                cout << "Invalid booking ID. Please, try again.\n";
                cin >> b_id;
                while (cin.fail())
                {
                    cout << "Invalid booking ID. Please, try again\n";
                    cin >> b_id;
                }
            }
        }

        for (int i = 0; i < Max_Rooms; i++)
            if (hotel_rooms[i].RoomID == users[u_index].Booking_ptr[book_index].RoomID)
                room_index = i;

        if (op == 1)
        {
            int old_day = users[u_index].Booking_ptr[book_index].StartDay.day;
            int old_per = users[u_index].Booking_ptr[book_index].StartPeriod;
            int Eday = users[u_index].Booking_ptr[book_index].EndDay.day;
            int Eper = users[u_index].Booking_ptr[book_index].EndPeriod;
            int new_day, new_per;
            cout << "Please, choose a check-in day from 1 to 7.\n";
            for (int i = 1; i <= 7; i++)
            {
                cout << i << ". " << GetDay(i - 1) << ".\n";
            }
            cin >> new_day;
            while (cin.fail() || new_day > 7 || new_day < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_day;
            }
            new_day--;
            cout << "Please, choose a check-in period from 1 to 3.\n";
            for (int i = 1; i <= 3; i++)
            {
                cout << i << ". " << GetPeriod(i - 1) << ".\n";
            }
            cin >> new_per;
            while (cin.fail() || new_per > 3 || new_per < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_per;
            }
            new_per--;
            bool is_availble = 1;
            for (int i = new_day; i <= Eday; i++)
                for (int j = 0; j < 3; j++)
                    if (!hotel_rooms[room_index].Availability[i][j])
                    {
                        if (i == old_day && j >= old_per)
                            continue;
                        if (i > old_day && i < Eday)
                            continue;
                        if (i == Eday && j <= Eper)
                            continue;
                        if (i == new_day && j < new_per)
                            continue;
                        is_availble = 0;
                        break;
                    }
            if (!is_availble)
            {
                cout << "Room is not available. Would you like to keep modfiying? Y/N\n";
                char ans;
                cin >> ans;
                while (cin.fail() || (ans != 'Y' && ans != 'N' && ans != 'y' && ans != 'n'))
                {
                    cout << "Invalid input. Please try again.\n";
                    cin >> ans;
                }
                if (ans == 'N' || ans != 'n')
                    return;
                else
                    modify(users, u_id);
            }
            users[u_index].Booking_ptr[book_index].StartDay.day = new_day;
            users[u_index].Booking_ptr[book_index].StartPeriod = new_per;
            hotel_rooms[room_index].Availability[new_day][new_per] = 0;
            if (new_day == old_day)
            {
                if (new_per > old_per)
                    for (int i = old_per; i < new_per; i++)
                        hotel_rooms[room_index].Availability[new_day][i] = 1;
                else if (new_per < old_per)
                    for (int i = new_per; i <= old_per; i++)
                        hotel_rooms[room_index].Availability[new_day][i] = 0;
            }
            else if (new_day > old_day)
            {
                for (int i = old_day; i <= new_day; i++)
                    for (int j = 0; j < 3; j++)
                    {
                        if (i == old_day && j < old_per)
                            continue;
                        if (i == new_day && j >= new_per)
                            continue;
                        hotel_rooms[room_index].Availability[i][j] = 1;
                    }
            }
            else
            {
                for (int i = new_day; i < old_day; i++)
                    for (int j = 0; j < 3; j++)
                    {
                        if (i == new_day && j < new_per)
                            continue;
                        hotel_rooms[room_index].Availability[i][j] = 0;
                    }
            }
            cout << "Changes saved successfully.\n";
        }
        else if (op == 2)
        {
            int old_day = users[u_index].Booking_ptr[book_index].EndDay.day;
            int old_per = users[u_index].Booking_ptr[book_index].EndPeriod;
            int Sday = users[u_index].Booking_ptr[book_index].StartDay.day;
            int Sper = users[u_index].Booking_ptr[book_index].StartPeriod;
            int new_day, new_per;
            cout << "Please, choose a check-out day from 1 to 7.\n";
            for (int i = 1; i <= 7; i++)
            {
                cout << i << ". " << GetDay(i - 1) << ".\n";
            }
            cin >> new_day;
            while (cin.fail() || new_day > 7 || new_day < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_day;
            }
            new_day--;
            cout << "Please, choose a check-out period from 1 to 3.\n";
            for (int i = 1; i <= 3; i++)
            {
                cout << i << ". " << GetPeriod(i - 1) << ".\n";
            }
            cin >> new_per;
            while (cin.fail() || new_per > 3 || new_per < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_per;
            }
            new_per--;
            bool is_availble = 1;
            for (int i = Sday; i <= new_day; i++)
                for (int j = 1; j <= 3; j++)
                    if (!hotel_rooms[room_index].Availability[i][j])
                    {
                        if (i == old_day && j <= old_per)
                            continue;
                        if (i < old_day && i > Sday)
                            continue;
                        if (i == Sday && j >= Sper)
                            continue;
                        if (i == new_day && j > new_per)
                            continue;
                        is_availble = 0;
                        break;
                    }
            if (!is_availble)
            {
                cout << "Room is not available. Would you like to keep modfiying? Y/N\n";
                char ans;
                cin >> ans;
                while (cin.fail() || (ans != 'Y' && ans != 'N' && ans != 'y' && ans != 'n'))
                {
                    cout << "Invalid input. Please try again.\n";
                    cin >> ans;
                }
                if (ans == 'N' || ans == 'n')
                    return;
                else
                    modify(users, u_id);
            }
            users[u_index].Booking_ptr[book_index].EndDay.day = new_day;
            users[u_index].Booking_ptr[book_index].EndPeriod = new_per;
            hotel_rooms[room_index].Availability[new_day][new_per] = 0;
            if (new_day == old_day)
            {
                if (new_per > old_per)
                    for (int i = old_per; i <= new_per; i++)
                        hotel_rooms[room_index].Availability[new_day][i] = 0;
                else if (new_per < old_per)
                    for (int i = new_per + 1; i <= old_per; i++)
                        hotel_rooms[room_index].Availability[new_day][i] = 1;
            }
            else if (new_day > old_day)
            {
                for (int i = old_day; i <= new_day; i++)
                    for (int j = 0; j < 3; j++)
                    {
                        if (i == new_day && j > new_per)
                            continue;
                        hotel_rooms[room_index].Availability[i][j] = 0;
                    }
            }
            else
            {
                for (int i = new_day; i <= old_day; i++)
                    for (int j = 0; j < 3; j++)
                    {
                        if (i == new_day && j <= new_per)
                            continue;
                        if (i == old_day && j > old_per)
                            continue;
                        hotel_rooms[room_index].Availability[i][j] = 1;
                    }
            }
            cout << "Changes saved successfully.\n";
        }
        else if (op == 3)
        {
            int old_Sday = users[u_index].Booking_ptr[book_index].StartDay.day;
            int old_Sper = users[u_index].Booking_ptr[book_index].StartPeriod;
            int old_Eday = users[u_index].Booking_ptr[book_index].EndDay.day;
            int old_Eper = users[u_index].Booking_ptr[book_index].EndPeriod;
            int new_Sday, new_Eday, new_Sper, new_Eper;
            cout << "Please, choose a check-in day from 1 to 7.\n";
            for (int i = 1; i <= 7; i++)
            {
                cout << i << ". " << GetDay(i - 1) << ".\n";
            }
            cin >> new_Sday;
            while (cin.fail() || new_Sday > 7 || new_Sday < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_Sday;
            }
            new_Sday--;
            cout << "Please, choose a check-in period from 1 to 3.\n";
            for (int i = 1; i <= 3; i++)
            {
                cout << i << ". " << GetPeriod(i - 1) << ".\n";
            }
            cin >> new_Sper;
            while (cin.fail() || new_Sper > 3 || new_Sper < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_Sper;
            }
            new_Sper--;
            cout << "Please, choose a check-out day from " << new_Sday << "to 7.\n";
            for (int i = 1; i <= 7; i++)
            {
                cout << i << ". " << GetDay(i - 1) << ".\n";
            }
            cin >> new_Eday;
            while (cin.fail() || new_Eday > 7 || new_Eday < new_Sday)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_Eday;
            }
            new_Eday--;
            cout << "Please, choose a check-out period from 1 to 3.\n";
            for (int i = 1; i <= 3; i++)
            {
                cout << i << ". " << GetPeriod(i - 1) << ".\n";
            }
            cin >> new_Eper;
            while (cin.fail() || new_Eper > 3 || new_Eper < 1)
            {
                cout << "Invalid input. Please, try again.\n";
                cin >> new_Eper;
            }
            new_Eper--;
            bool is_availble = 1;
            for (int i = new_Sday; i <= new_Eday; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (!hotel_rooms[room_index].Availability[i][j])
                    {
                        if (i == new_Sday && j < new_Sper)
                            continue;
                        if (i == new_Eday && j > new_Eper)
                            continue;
                        if (i > old_Sday && i < old_Eday)
                            continue;
                        if (i == old_Sday && j >= old_Sper)
                            continue;
                        if (i == old_Eday && j <= old_Eper)
                            continue;
                        is_availble = 0;
                        break;
                    }
                }
            }
            if (!is_availble)
            {
                cout << "Room is not available. Would you like to keep modfiying? Y/N\n";
                char ans;
                cin >> ans;
                while (cin.fail() || (ans != 'Y' && ans != 'N' && ans != 'y' && ans != 'n'))
                {
                    cout << "Invalid input. Please try again.\n";
                    cin >> ans;
                }
                if (ans == 'N' || ans != 'n')
                    return;
                else
                    modify(users, u_id);
            }
            users[u_index].Booking_ptr[book_index].StartDay.day = new_Sday;
            users[u_index].Booking_ptr[book_index].StartPeriod = new_Sper;
            users[u_index].Booking_ptr[book_index].EndDay.day = new_Eday;
            users[u_index].Booking_ptr[book_index].EndPeriod = new_Eper;
            hotel_rooms[room_index].Availability[new_Sday][new_Sper] = 0;
            hotel_rooms[room_index].Availability[new_Eday][new_Eper] = 0;
            for (int i = old_Sday; i <= old_Eday; i++)
                for (int j = 0; j < 3; j++)
                {
                    if (i == old_Sday && j < old_Sper)
                        continue;
                    if (i == old_Eday && j > old_Eper)
                        continue;
                    hotel_rooms[room_index].Availability[i][j] = 1;
                }
            for (int i = new_Sday; i <= new_Eday; i++)
                for (int j = 0; j < 3; j++)
                {
                    if (i == new_Sday && j < new_Sper)
                        continue;
                    if (i == new_Eday && j > new_Eper)
                        continue;
                    hotel_rooms[room_index].Availability[i][j] = 0;
                }
            cout << "Changes saved successfully.\n";
        }
        else
        {
            int old_Sday = users[u_index].Booking_ptr[book_index].StartDay.day;
            int old_Eday = users[u_index].Booking_ptr[book_index].EndDay.day;
            int old_Sper = users[u_index].Booking_ptr[book_index].StartPeriod;
            int old_Eper = users[u_index].Booking_ptr[book_index].EndPeriod;

            for (int i = old_Sday; i <= old_Eday; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (i == old_Sday && j < old_Sper)
                        continue;
                    if (i == old_Eday && j > old_Eper)
                        continue;
                    hotel_rooms[room_index].Availability[i][j] = 1;
                }
            }
            for (int i = 0; i < Max_Booking; i++)
            {
                if (hotel_rooms[room_index].Room_booking[i].Booking_ID == b_id)
                {
                    hotel_rooms[room_index].Room_booking[i] = {};
                }
            }
            hotel_rooms[room_index].RoomBookingCount--;
            Booking* new_booking = new Booking[Max_Booking];

            for (int i = 0; i < book_index; i++)
                new_booking[i] = users[u_index].Booking_ptr[i];

            for (int i = book_index; i < Max_Booking - 1; i++)
                new_booking[i] = users[u_index].Booking_ptr[i + 1];

            delete[] users[u_index].Booking_ptr;
            

            users[u_index].Booking_ptr = new_booking;
            users[u_index].UserBooking_counter--;
            cout << "Changes saved successfully.\n";
        }
        cout << "Do You Want to do more operations?(y/n):\n";
        cin >> as;
        while ((cin.fail()) || (as != 'y' && as != 'Y' && as != 'n' && as != 'N'))
        {
            cout << "Invalid input'please try again :\n";
            cin >> as;
        }
        system("cls");
    } while (as == 'Y' || as == 'y');
}
void logout()
{
    User_info = {};
    system("cls");
    main();
}

void AddRoom(int& size, Room*& arr) {
    char AdminAns;
    Room* newArr; // Pointer to the new array

    do {
        // Room info for the new room
        Room newRoom;
        cout << "Room ID: "; cin >> newRoom.RoomID;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<int>::max(), '\n');
            cout << "PLease enter a valid ID.\n";
            cin >> newRoom.RoomID;
        }
        cout << "Room price: "; cin >> newRoom.Price;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<int>::max(), '\n');
            cout << "PLease enter a valid price.\n";
            cin >> newRoom.Price;
        }
        cout << "Room type: "; cin >> newRoom.RoomType;
        cin.ignore();
        cout << "Room location: ";
        getline(cin, newRoom.room_preferences.location);
        cout << "Room bed type: ";
        getline(cin, newRoom.room_preferences.bed_type);
        cout << "Availability of smoking: ";
        getline(cin, newRoom.room_preferences.availability_smooking);
        cout << "Meeting room or non-meeting room: ";
        getline(cin, newRoom.room_preferences.room_meeting);

        // Increment the size
        ++size;

        // Create a new dynamic array with increased size
        newArr = new Room[size];

        // Copy existing rooms to the new array
        for (int i = 0; i < size - 1; ++i) {
            newArr[i] = arr[i];
        }

        // Add the new room to the new array
        newArr[size - 1] = newRoom;

        // Deallocate memory for the old array
        delete[] arr;

        // Assign the pointer to the new array
        arr = newArr;

        cout << "\nA new room has been created successfully!\n";

        cout << "\nDo you want to add another room? (y/n): ";
        cin >> AdminAns;

        // Validate user input
        while (cin.fail() || (AdminAns != 'y' && AdminAns != 'Y' && AdminAns != 'n' && AdminAns != 'N')) {
            // Clear input stream
            cin.clear();
            // Ignore remaining characters in input stream
            cin.ignore(numeric_limits<int>::max(), '\n');
            // Prompt user again
            cout << "Invalid input. Please enter 'y' or 'n': ";
            cin >> AdminAns;
        }

        system("cls");
    } while (AdminAns == 'y' || AdminAns == 'Y');

    system("cls");
}

void DisplayRoom(Room*& arr, int size) {
    int ID;
    bool isFound; //id found or not
    char AdminAns;
    do {
        cout << "Enter room ID number : ";
        cin >> ID;
        for (int i = 0; i < size; i++) {
            if (ID == arr[i].RoomID) {
                isFound = true;
                cout << "Room found!\n-----------\n";
                cout << "Room informations : \n";
                cout << "Room ID : " << arr[i].RoomID << "\n";
                cout << "Room type : " << arr[i].RoomType << "\n";
                cout << "Room price : $" << arr[i].Price << "\n";
                cout << "Room location: " << arr[i].room_preferences.location << "\n";
                cout << "Room bed type: " << arr[i].room_preferences.bed_type << "\n";
                cout << "Availability of smoking: " << arr[i].room_preferences.availability_smooking << "\n";
                cout << "Meeting room or non-meeting room: " << arr[i].room_preferences.room_meeting << "\n";
                break;
            }
            else isFound = false;
        }

        if (!isFound) {
            cout << "\nRoom was not found, please try again.\n";
        }

        cout << "\nDo you want to display another room?(y/n)";
        cin >> AdminAns;
        while (cin.fail() || (AdminAns != 'y' && AdminAns != 'Y' && AdminAns != 'n' && AdminAns != 'N')) {
            cin.clear();
            cin.ignore(numeric_limits<int>::max(), '\n');
            cout << "Invalid input. Please enter 'y' or 'n': ";
            cin >> AdminAns;
        }
        system("cls");
    } while (AdminAns == 'y' || AdminAns == 'Y');
    system("cls");
}

int AdminMenu() {
    cout << "\t{******Admin list******}\t" << endl;
    cout << "1.Add rooms.\n";
    cout << "2.Display a room.\n";
    cout << "3.Edit room information.\n";
    cout << "4.Delete a room.\n";
    cout << "5.Check all bookings for a certain room.\n";
    cout << "6.Exit program.\n";
    int MenuAns;
    cout << "Please choose an option : ";
    cin >> MenuAns;
    //menu answer validation
    while (MenuAns < 0 || MenuAns > 6 || cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<int>::max(), '\n');
        cout << "Invalid answer, please try again.\n";
        cin >> MenuAns;
    }
    return MenuAns;
}
int EditMenu() {
    cout << "Please choose the information you want to edit : \n";
    cout << "1.Edit an ID.\n";
    cout << "2.Edit a room type.\n";
    cout << "3.Edit the price.\n";
    cout << "4.Edit the location.\n";
    cout << "5.Edit the bed type.\n";
    cout << "6.Edit the smoking prefrences.\n";
    cout << "7.Edit meeting type.\n";
    cout << "8.Edit availability status with specific date.\n";
    int op;
    cout << "Please choose an option :\n ";
    cin >> op;
    while (op < 0 || op > 8 || cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<int>::max(), '\n');
        cout << "Invalid choise, please try again.\n";
        cin >> op;
    }
    return op;

}
void EditRoomInfo(Room*& arr) {
    int ID;
    char AdminAns;
    do {
        //take input from user
        cout << "Enter the ID of the room you want to modifiy :\n ";
        cin >> ID;

        //check id
        int index = -1; //get index of wanted room

        for (int i = 0; i < Max_Rooms; ++i) {
            if (ID == arr[i].RoomID) {
                index = i;
                break;
            }
        }
        if (index == -1) cout << "Room was not found.\n";
        else {
            cout << "Room found!!\n";
            int ans = EditMenu();
            system("cls");
            if (ans == 1) {
                cout << "Enter the new ID: ";
                cin >> arr[index].RoomID;
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid ID.\n";
                    cin >> arr[index].RoomID;
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 2) {
                cout << "Enter the new room type: ";
                cin >> arr[index].RoomType;
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid room data.\n";
                    cin >> arr[index].RoomType;
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 3) {
                cout << "Enter the new room price: ";
                cin >> arr[index].Price;
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid price.\n";
                    cin >> arr[index].Price;
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 4) {
                cout << "Enter the new room location: ";
                getline(cin, arr[index].room_preferences.location, '\n');
                cin >> arr[index].room_preferences.location;
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid location.\n";
                    getline(cin, arr[index].room_preferences.location);
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 5) {
                cout << "Enter the new room bed type: ";
                cin.ignore(numeric_limits<int>::max(), '\n');
                getline(cin, arr[index].room_preferences.bed_type);
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid data.\n";
                    getline(cin, arr[index].room_preferences.bed_type);
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 6) {
                cout << "Enter smoking availability: ";
                cin.ignore(numeric_limits<int>::max(), '\n');
                getline(cin, arr[index].room_preferences.availability_smooking);
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid data.\n";
                    getline(cin, arr[index].room_preferences.availability_smooking);
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 7) {
                cout << "Enter meeting availability: ";
                cin.ignore(numeric_limits<int>::max(), '\n');
                getline(cin, arr[index].room_preferences.room_meeting);
                while (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<int>::max(), '\n');
                    cout << "PLease enter a valid data.\n";
                    getline(cin, arr[index].room_preferences.room_meeting);
                }
                cout << "Done successfully.\n";
            }
            else if (ans == 8) {
                Booking avdate;//the full date the admin will choose whether to make it available or not
                bool avstatus;//availability status
                cout << "0.Unavailable\n1.Available\nPlease choose(0/1) : "; cin >> avstatus;
                system("cls");
                cout << "1.Saturday\n2.Sunday\n3.Monday\n4.Tuesday\n5.Wedensday\n6.Thursday\n7.Friday\n";
                cout << "Enter start date,(1-7) " << endl;
                cin >> avdate.StartDay.day;
                system("cls");
                while (avdate.StartDay.day < 1 || avdate.StartDay.day > 7 || cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid date, please enter again:(1-7)" << endl;
                    cin >> avdate.StartDay.day;
                }
                system("cls");
                cout << "Enter a specific period ,please \n";
                cout << "1. Morning\n2. Afternoon\n3. Evening\n";
                cin >> avdate.StartPeriod;
                while (avdate.StartPeriod < 1 || avdate.StartPeriod > 3 || cin.fail())
                {
                    cout << "Invalid input, please try again\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> avdate.StartPeriod;
                }
                system("cls");
                cout << "1.Saturday\n2.Sunday\n3.Monday\n4.Tuesday\n5.Wedensday\n6.Thursday\n7.Friday\n";
                cout << "Enter end date,(1-7)" << endl;
                cin >> avdate.EndDay.day;
                while (avdate.EndDay.day < 1 || avdate.EndDay.day > 7 || cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid date, please enter again:(1-7)" << endl;
                    cin >> avdate.EndDay.day;
                }
                system("cls");
                //check that the end_day is greater than the start_day
                while (avdate.EndDay.day < avdate.StartDay.day)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input ,please try again" << endl;
                    cin >> avdate.StartDay.day >> avdate.EndDay.day;
                }
                system("cls");
                cout << "Enter a specific period of a check_out ,please \n";
                cout << "1. Morning\n2. Afternoon\n3. Evening\n";
                cin >> avdate.EndPeriod;
                while (avdate.EndPeriod < 1 || avdate.EndPeriod >3 || cin.fail())
                {
                    cout << "Invalid input, please try again\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> avdate.EndPeriod;
                }
                //set arr[index].Availability[days][period] = false
                for (int day = avdate.StartDay.day - 1; day <= avdate.EndDay.day - 1; day++)
                {
                    for (int p = 0; p < 3; p++)
                    {
                        if ((day == avdate.StartDay.day - 1) && (p < avdate.StartPeriod - 1))
                            continue;
                        else if ((day == avdate.EndDay.day - 1) && (p > avdate.EndPeriod - 1))
                            continue;
                        if (avstatus) arr[index].Availability[day][p] = true;
                        else if (!avstatus) arr[index].Availability[day][p] = false;

                    }
                }
                cout << "Changing status done successfully.\n";
            }
        }
        cin.ignore(numeric_limits<int>::max(), '\n');
        cout << "Do you want to edit more room informations?(y/n) ";
        cin >> AdminAns;
        while (cin.fail() || (AdminAns != 'y' && AdminAns != 'Y' && AdminAns != 'n' && AdminAns != 'N')) {
            cin.clear();
            cin.ignore(numeric_limits<int>::max(), '\n');
            cout << "Invalid input. Please enter 'y' or 'n': ";
            cin >> AdminAns;
        }
        system("cls");
    } while (AdminAns == 'y' || AdminAns == 'Y');
    system("cls");
}
void DeleteRoom(int& size, Room*& arr) {
    int ID;
    char AdminAns;
    do {
        cout << "Enter room ID : ";
        cin >> ID;
        int index = -1;
        //get index
        for (int i = 0; i < size; ++i) {
            if (ID == arr[i].RoomID) {
                index = i;
                break;
            }
        }
        //index check
        if (index != -1) {
            Room* newArr = new Room[size - 1]; // Decrease size by 1
            for (int i = 0, j = 0; i < size; ++i) {
                if (i != index) {
                    newArr[j++] = arr[i]; // Copy all rooms except the one we want to delete
                }
            }
            size--; // Decrement size
            cout << "\nThe room has been deleted successfully.\n";
            delete[] arr; // Delete old array
            arr = newArr; // Update pointer
        }
        else {
            cout << "Room not found.";
        }
        cout << "\n Do you want to delete more rooms?(y/n) ";
        cin >> AdminAns;
        while (cin.fail() || (AdminAns != 'y' && AdminAns != 'Y' && AdminAns != 'n' && AdminAns != 'N')) {
            cin.clear();
            cin.ignore(numeric_limits<int>::max(), '\n');
            cout << "Invalid input. Please enter 'y' or 'n': ";
            cin >> AdminAns;
        }
        system("cls");
    } while (AdminAns == 'y' || AdminAns == 'Y');
    system("cls");
}
void CheckBooking_For_Admin(Room* hotelRooms)
{
    int ID;
    cout << "Please enter room ID\n";
    cin >> ID;
    for (int i = 0; i < Max_Rooms; i++)
    {
        if (hotel_rooms[i].RoomID == ID) {
            if (hotelRooms[i].RoomBookingCount == 0) {
                cout << "NO Bookings found for this room \n";
                break;
            }
            else {
                for (int j = 0; j < hotelRooms[i].RoomBookingCount; j++)
                {
                    cout << "#Booking(" << j + 1 << "):\n";
                    cout << "Room Id: " << ID << "\n";
                    cout << "User Id: " << hotelRooms[i].Room_booking[j].User_ID << "\n";
                    cout << "Booking ID: " << hotelRooms[i].Room_booking[j].Booking_ID << "\n";
                    cout << "Start Day: " << GetDay(hotelRooms[i].Room_booking[j].StartDay.day) << "\n";
                    cout << "End Day: " << GetDay(hotelRooms[i].Room_booking[j].EndDay.day) << "\n";
                    if (GetPeriod(hotelRooms[i].Room_booking[j].StartPeriod) == "Morning")
                        cout << "Check in Period: " << "Morning" << "\n";
                    else if (GetPeriod(hotelRooms[i].Room_booking[j].StartPeriod) == "Afternoon")
                        cout << "Check in Period: " << "Afternoon" << "\n";
                    else
                        cout << "Check in Period: " << "Evening" << "\n";
                    if (GetPeriod(hotelRooms[i].Room_booking[j].EndPeriod) == "Morning")
                        cout << "Check out Period: " << "Morning" << "\n";
                    else if (GetPeriod(hotelRooms[i].Room_booking[j].EndPeriod) == "Afternoon")
                        cout << "Check out Period: " << "Afternoon" << "\n";
                    else
                        cout << "Check out Period: " << "Evening" << "\n";
                    cout << "-----------------------------\n";

                }
            }

        }
    }

}
void RateRoom(Room* room, User* user) {

    cout << "Enter the room id: ";
    int roomID;
    cin >> roomID;

    // Validate room ID
    if (cin.fail() || roomID < 0 || roomID >= Max_Rooms) {
        cout << "Invalid room ID. Please enter a valid room ID.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    bool alreadyRated = false;
    // Check if the user has already rated this room
    for (int i = 0; i < user->UserBooking_counter; ++i) {
        if (user->RatedRooms[i] == roomID) {
            cout << "You have already rated this room.\n";
            cout << "Your previous rating: " << room[roomID].ratings[user->UserID] << " stars\n";
            cout << "Do you want to update your rating? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'n' || choice == 'N') {
                return;
            }
            else {
                cout << "Enter your updated rating for the room (1 to 5 stars): ";
                cin >> room->ratings[roomID];
                if (cin.fail() || room[roomID].ratings[user->UserID] < 1 || room[roomID].ratings[user->UserID] > 5) {
                    cout << "Invalid rating. Please enter a rating between 1 and 5.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                cout << "Rating updated successfully.\n";
                calculateavgrating(room, roomID, user);
                return;
            }
        }
    }
    int newRatingIndex = room->totalRatings;

    if (!alreadyRated) {
        cout << "Enter your rating for the room (1 to 5 stars): ";
        cin >> room[roomID].ratings[user->UserID];

        if (cin.fail() || room[roomID].ratings[user->UserID] < 1 || room[roomID].ratings[user->UserID] > 5) {
            cout << "Invalid rating. Please enter a rating between 1 and 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        room[roomID].totalRatings++;
        calculateavgrating(room, roomID, user);
        cout << "Thank you for rating the room.\n";
    }
}

void calculateavgrating(Room* room, int roomID, User* user) {
    float sum = 0.0;
    for (int j = 0; j < room->totalRatings; j++) {
        sum += room->ratings[j];
    }
    room->averageRating[roomID] = sum / room->totalRatings;


    // Add the room to the list of rated rooms for the user
    user->RatedRooms[user->UserBooking_counter++] = roomID;
}
