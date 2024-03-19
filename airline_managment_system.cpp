#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <regex>
#include <windows.h>

using namespace std;

class TreeNode
{
public:
    string city;
    vector<TreeNode *> children;
    TreeNode() : city("Unknown") {}
    TreeNode(string name) : city(name) {}
};

class Tree
{
    TreeNode *root;
    string sourceCity;

public:
    /// @brief Constructors
    Tree()
    {
        sourceCity = "Unknown";
        root = nullptr;
    }

    Tree(TreeNode *source)
    {
        root = source;
    }

    Tree(string City)
    {
        this->sourceCity = City;
        root = new TreeNode(sourceCity);
    }

    // Methods

    /// @brief this recives all input of routes
    /// @return tree TreeNode
    TreeNode *addRoutes()
    {
        string city;
        cout << "\nEnter City: ";

        getline(cin, city);

        TreeNode *cityNode = new TreeNode(city);
        int numTransitCities;

        cout << "Enter number of direct destinations from " << city << ": ";
        cin >> numTransitCities;

        cin.ignore();

        for (int i = 1; i <= numTransitCities; i++)
        {
            TreeNode *child = addRoutes();
            cityNode->children.push_back(child);
        }

        return cityNode;
    }

    /// @brief Calculate the number of destination
    /// @param root
    /// @return total number of destination
    int totalDestination(TreeNode *root)
    {
        int result = 1;

        for (int i = 0; i < root->children.size(); i++)
        {
            result += totalDestination(root->children[i]);
        }
        return result;
    }

    /// @brief no parameter function to display all routes
    void displayRoute()
    {
        if (root == NULL)
        {
            cout << "No cities!\nEnter cities" << endl;
        }
        else
        {
            displayAllRoute(root);
        }
    }

    /// @brief TreeNode parameter function to display all routes
    /// @param root
    void displayAllRoute(TreeNode *root)
    {
        if (!root->children.empty())
        {
            cout << "From " << root->city << " to: ";

            for (int i = 0; i < root->children.size(); i++)
            {
                cout << root->children[i]->city << ", ";
            }
            cout << endl;
            for (int i = 0; i < root->children.size(); i++)
            {
                displayAllRoute(root->children[i]);
            }
        }
        else
        {
            // cout << "No route availabe!" << endl;
        }
    }

    void addCity(TreeNode *root, string fromCity, string toCity)
    {
        TreeNode *newCity = new TreeNode(toCity);
        TreeNode *from;
        if (fromCity == sourceCity)
        {
            root->children.push_back(newCity);
            cout << "\nSuccessfully added!" << endl;
        }
        else
        {
            from = findNode(root, fromCity);
            if (from == nullptr)
            {
                cout << "No route exist!" << endl; // if there is no from in the route network
            }
            else
            {
                from->children.push_back(newCity);
            }
        }
    }

    /// @brief finds if there exist city to add another route from it
    /// @param node
    /// @param searchCity
    /// @return tree
    TreeNode *findNode(TreeNode *node, string searchCity)
    {
        if (node->city == searchCity) // search city is all lower case so is the one in the tree
        {
            return node;
        }
        for (int i = 0; i < node->children.size(); i++)
        {
            TreeNode *result = findNode(node->children[i], searchCity);
            if (result != nullptr)
            {
                return result;
            }
        }
        return nullptr;
    }
    /// @brief This function is used to check if route exist before a flight is added
    /// @param route
    /// @param from
    /// @param to
    /// @return
    bool checkRoute(TreeNode *route, string from, string to)
    {
        bool found = false;
        queue<TreeNode *> q;
        q.push(route);
        TreeNode *node;
        while (!q.empty())
        {
            node = q.front();
            q.pop();
            if (node->city == from)
            {
                found = true;
                break;
            }
            for (int i = 0; i < node->children.size(); i++)
            {
                q.push(node->children[i]);
            }
        }
        TreeNode *checkDestination = findNode(node, to);
        if (found)
        {
            if (checkDestination != nullptr)
            {
                return true;
            }
        }
        return false;
    }
};

class Passenger
{
public:
    string name;
    string phoneNumber;
    string passportExpirationDate;
    string specialRequest;
    int id;
    Passenger() : name("Unknown"), phoneNumber("Unknown"), passportExpirationDate("Unknown"), specialRequest("Unknown") {}
    Passenger(string name, string num, string expire, string request) : name(name), phoneNumber(num), passportExpirationDate(expire), specialRequest(request) {}
};

struct Flight
{
    string from;
    string to;
    string flightNumber;
    string airplaneName;
    string departureTime;
    string date;
    int availableSeats;
    vector<Passenger> passenger;
    Flight *next;

    Flight() : from("Unknown"), to("Unknown"), flightNumber("Unknown"), airplaneName("Unknown"), departureTime("Unknown"), date("unknown"), availableSeats(0), next(nullptr) {}
};

class FlightManager
{

private:
    Flight *head = nullptr;
    int numberOfFlights = 0;

public:
    /// @brief Used for making sure the date is vaild
    /// @param date
    /// @return flase if not valid
    bool validateDate(const string &date)
    {
        regex pattern("(202[4-9]|203[0-9])-(0[2-9]|1[0-2])-([0-2][0-9]|3[0-1])");
        return regex_match(date, pattern);
    }
    /// @brief Used for making sure the time is vaild
    /// @param time
    /// @return flase if not valid
    bool validateTime(const string &time)
    {
        regex pattern("(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]|");
        return regex_match(time, pattern);
    }
    /// @brief This function is used to check if route exist before a flight is added
    /// @param route
    /// @param from
    /// @param to
    /// @return flase if route doesn't exist
    bool checkRoute(TreeNode *route, string from, string to)
    {
        Tree t;
        bool found = false;
        queue<TreeNode *> q;
        q.push(route);
        TreeNode *node;
        while (!q.empty())
        {
            node = q.front();
            q.pop();
            if (node->city == from)
            {
                found = true;
                break;
            }
            for (int i = 0; i < node->children.size(); i++)
            {
                q.push(node->children[i]);
            }
        }
        TreeNode *checkDestination = t.findNode(node, to);
        if (found)
        {
            if (checkDestination != nullptr)
            {
                return true;
            }
        }
        return false;
    }
    /// @brief used to add new flights
    /// @param route which it the route the airline has, that is instanciated from the tree class
    void addFlight(TreeNode *route)
    {
        Flight *newFlight = new Flight();
        string from, to, time, date;
        bool successful = false;
        cin.ignore();

        cout << "Enter the departure location: ";
        getline(cin, from);
        cout << "Enter the destination: ";
        getline(cin, to);
        newFlight->from = from;
        newFlight->to = to;

        if (checkRoute(route, from, to))
        {
            // cin.ignore();
            cout << "Enter flight name: ";
            getline(cin, newFlight->flightNumber);
            cout << "Enter the airplane name: ";
            getline(cin, newFlight->airplaneName);

            while (!successful)
            {

                cout << "Enter the departure time: ";
                getline(cin, time);
                if (!validateTime(time))
                {
                    cout << "Invalid name format. Please enter time in the format 'HH:MM' in 24-hour format." << endl;
                    continue;
                }
                newFlight->departureTime = time;
                successful = true;
            }

            successful = false;

            while (!successful)
            {
                cout << "Enter the departure date: ";
                getline(cin, date);
                if (!validateDate(date))
                {
                    cout << "Enter your passport expiration date (YYYY-MM-DD): 2024-02-12" << endl;
                    continue;
                }
                newFlight->date = date;
                successful = true;
            }

            successful = false;

            cout << "Enter the available seat: ";
            cin >> newFlight->availableSeats;

            if (head == nullptr)
            {
                head = newFlight;
            }
            else
            {
                Flight *current = head;
                while (current->next != nullptr)
                {
                    current = current->next;
                }
                current->next = newFlight;
            }

            cout << "\nFlight " << newFlight->flightNumber << " added successfully." << endl;
            numberOfFlights++;
        }
        else
        {
            cout << "Route doesn't exist!" << endl;
        }
    }
    /// @brief Removes a specific flight
    void removeFlight()
    {
        string flightNumber;
        if (head == nullptr)
        {
            cout << "No flights available." << endl;
            return;
        }
        cin.ignore();
        cout << "Enter the flight name: ";
        cin >> flightNumber;
        Flight *current = head;
        Flight *previous = nullptr;

        while (current != nullptr)
        {
            if (current->flightNumber == flightNumber)
            {
                if (previous == nullptr)
                {
                    head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                delete current;
                cout << "\nFlight " << flightNumber << " removed successfully." << endl;
                numberOfFlights--;

                return;
            }

            previous = current;
            current = current->next;
        }

        cout << "Flight " << flightNumber << " not found." << endl;
    }
    /// @brief Update a specific flight
    void updateFlight()
    {
        string flightNumber;
        cin.ignore();
        cout << "Enter the flight name: ";
        getline(cin, flightNumber);
        Flight *current = head;

        while (current != nullptr)
        {
            if (current->flightNumber == flightNumber)
            {
                cout << "\nFlight " << flightNumber << " found. Select an attribute to update:" << endl;

                cout << "1. Airplane name" << endl;
                cout << "2. Departure time" << endl;
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;
                cin.ignore(); // Ignore the newline character left in the input buffer

                switch (choice)
                {
                case 1:
                {
                    cout << "Enter the new airplane name: ";
                    getline(cin, current->airplaneName);
                    break;
                }
                case 2:
                {
                    cout << "Enter the new departure time: ";
                    getline(cin, current->departureTime);
                    break;
                }
                default:
                {
                    cout << "Invalid choice!" << endl;
                    return;
                }
                }

                cout << "\nFlight " << flightNumber << " updated successfully." << endl;
                return;
            }

            current = current->next;
        }

        cout << "\nFlight " << flightNumber << " not found." << endl;
    }

    /// @brief Showes all flights
    void displayFlights()
    {
        if (head == nullptr)
        {
            cout << "No flights available." << endl;
            return;
        }

        cout << "\n____________Flight List____________" << endl;
        Flight *current = head;
        while (current != nullptr)
        {
            cout << "\nFlight Name: " << current->flightNumber << endl;
            cout << "Departure Location: " << current->from << endl;
            cout << "Destination: " << current->to << endl;
            cout << "Airplane Name: " << current->airplaneName << endl;
            cout << "Departure Time: " << current->departureTime << endl;
            cout << "Departure Date: " << current->date << endl;
            cout << "Available Seats: " << current->availableSeats << endl;
            cout << "________________________________________" << endl;

            current = current->next;
        }
    }

    /// @brief This searches the flight number
    /// @param flightNum
    /// @return the flight node if found, else nullptr
    Flight *searchFlight(string flightNum)
    {
        Flight *temp = head;
        while (temp != nullptr)
        {
            if (temp->flightNumber == flightNum)
            {
                return temp;
            }
            temp = temp->next;
        }
        temp = nullptr;
        return temp;
    }

    /// @brief This recives the flight from user send it to another function, searchFlight(string flightNum), and displays the result
    /// @param flightNum
    void showSearchedFlight(string flightNum)
    {
        Flight *temp = searchFlight(flightNum);
        if (temp == nullptr)
        {
            cout << "Flight not found!" << endl;
        }
        else
        {
            cout << "\n______________________________________" << endl;
            cout << "Flight Name: " << temp->flightNumber << endl;
            cout << "Departure Location: " << temp->from << endl;
            cout << "Destination: " << temp->to << endl;
            cout << "Airplane Name: " << temp->airplaneName << endl;
            cout << "Departure Time: " << temp->departureTime << endl;
            cout << "Departure Date: " << temp->date << endl;
            cout << "Available Seats: " << temp->availableSeats << endl;
            cout << "______________________________________" << endl;
        }
    }
    /// @brief This is used to return the head of flight
    /// @return the first node of flight
    Flight **getHead()
    {
        return &head;
    }
    /// @brief This shows all the passengers in each flight
    void displayPassengers()
    {
        bool found = false;
        Flight *temp = head;
        while (temp != nullptr && !temp->passenger.empty())
        {
            cout << "Flight Name: " << temp->flightNumber << endl;
            for (int i = 0; i < temp->passenger.size(); i++)
            {
                cout << "\t_________________________________________________" << endl;
                cout << "\t"
                     << "ID: " << temp->passenger[i].id << endl;
                cout << "\t"
                     << "Full-name: " << temp->passenger[i].name << endl;
                cout << "\t"
                     << "Phone-number: " << temp->passenger[i].phoneNumber << endl;
                cout << "\t"
                     << "Special-request: " << temp->passenger[i].specialRequest << endl;
                cout << "\t"
                     << "Passport Expiration Date: " << temp->passenger[i].passportExpirationDate << endl;
                cout << "\t_________________________________________________" << endl;
            }
            found = true;
            temp = temp->next;
        }
        if (!found)
        {
            cout << "\nNo passengers!" << endl;
        }
    }
    /// @brief this function check if there are flights
    /// @return returns true if there are no flights
    bool noFlights()
    {
        return (numberOfFlights == 0);
    }
};

class PassengerManagment
{
    Flight **head;
    int passengerId = 0;

public:
    PassengerManagment(Flight **flightHead) : head(flightHead) {}

    /// @brief This function is used to book flight by passengers
    /// @param passengerInfo
    void bookFlight(Passenger &passengerInfo)
    {
        string flightNum;
    start:
        cout << "Enter flight name: ";
        getline(cin, flightNum);
        bool success = false;
        Flight *temp = *head;
        while (temp != nullptr)
        {
            if (temp->flightNumber == flightNum)
            {
                if (temp->availableSeats > 0)
                {
                    ++passengerId;
                    passengerInfo.id = passengerId;
                    temp->availableSeats--;
                    temp->passenger.push_back(passengerInfo); // stores the new passenger informaion into flight object

                    cout << "Flight " << flightNum << " booked successfully for " << passengerInfo.name << "!" << endl;
                }
                else
                {
                    cout << "Sorry, no available seats for Flight " << flightNum << "." << endl;
                }
                return;
            }
            temp = temp->next;
        }

        cout << "\nFlight not found!" << endl;
        int choice;
        cout << "\n__________Menu__________" << endl;
        cout << "1. Re-enter flight name" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter choice:";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cin.ignore();
            goto start;
            break;
        case 2:
            break;
        default:
            cout << "Invalid Input!" << endl;
        }

        cout << "Flight " << flightNum << " not found." << endl;
    }

    /// @brief This function is used to cancel booked passengers
    /// @param id is used to find the passenger to cancel his reservation
    void cancelReservation(int id)
    {
        Flight *temp = *head;
        while (temp != nullptr)
        {
            for (int i = 0; i < temp->passenger.size(); i++)
            {
                if (temp->passenger[i].id == id)
                {
                    temp->passenger.erase(temp->passenger.begin() + i);
                    ++temp->availableSeats;
                    cout << "Successfully cancelld reservation!" << endl;
                    return;
                }
            }
            temp = temp->next;
        }
        cout << "\nNo passenger with ID: " << id << "\nPlease try again!" << endl;
    }
    /// @brief Used to display all flights that a passenger can book
    void displayFlights()
    {
        Flight *temp = *head;
        if (temp == nullptr)
        {
            cout << "No flights available" << endl;
        }
        else
        {
            cout << "\nAvailable Flights:" << endl;
            while (temp)
            {
                cout << "\tFlight " << temp->flightNumber << "| From:" << temp->from << "| To:" << temp->to << "| Date:" << temp->date << "| Seats: " << temp->availableSeats << endl;
                temp = temp->next;
            }
            cout << "_______________________________________________________________________________________" << endl;
            cout << endl;
        }
    }
};

/// @brief For cheching phonenumber format validity
/// @param phoneNumber
/// @return
bool validatePhoneNumber(const string &phoneNumber)
{
    regex pattern("\\d{10}");
    return regex_match(phoneNumber, pattern);
}
/// @brief For cheching passport expire date format validity
/// @param date
/// @return
bool validateDate(const string &date)
{
    regex pattern("(202[5-9])-(0[1-9]|1[0-2])-([0-2][0-9]|3[0-1])");
    return regex_match(date, pattern);
}

/// @brief For cheching name format validity
/// @param name
/// @return
bool validateName(const string &name)
{
    regex pattern("[A-Za-z]+ [A-Za-z]+ [A-Za-z]+");
    return regex_match(name, pattern);
}

/// @brief For adding passenger information
/// @return object of passenger type
Passenger addPassenger()
{
    Passenger passengerInfo;
    string nameInput, phoneNumberInput, expirationDateInput;
    bool successful = false;
    cin.ignore();
    while (!successful)
    {
        cout << "\nEnter your name (First Middle Last): ";
        getline(cin, nameInput);
        if (!validateName(nameInput))
        {
            cout << "Invalid name format. Please enter your name in the format 'First Middle Last'." << endl;
            continue;
        }
        passengerInfo.name = nameInput;
        successful = true;
    }

    successful = false;
    while (!successful)
    {
        cout << "\nEnter your phone number (10 digits): ";
        getline(cin, phoneNumberInput);
        if (!validatePhoneNumber(phoneNumberInput))
        {
            cout << "Invalid phone number format. Please enter a 10-digit phone number." << endl;
            continue;
        }
        passengerInfo.phoneNumber = phoneNumberInput;
        successful = true;
    }

    successful = false;
    while (!successful)
    {
        cout << "Enter your passport expiration date (YYYY-MM-DD): ";
        getline(cin, expirationDateInput);
        if (!validateDate(expirationDateInput))
        {
            cout << "Invalid date format. Please enter the expiration date in the format 'YYYY-MM-DD'." << endl;
            continue;
        }
        passengerInfo.passportExpirationDate = expirationDateInput;
        successful = true;
    }

    cout << "Enter any special request (if any): ";
    getline(cin, passengerInfo.specialRequest);

    return passengerInfo;
}

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void Delayer(string str, int sleepTime = 0)
{
    for (int i = 0; i < str.size(); i++)
    {
        cout << str[i];
        Sleep(sleepTime);
    }
    cout << endl;
    SetConsoleTextAttribute(hConsole, 15);
}

void gotoxy(short x, short y)
{

    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void welcome_page()
{

    //---------------------------------------------------------------------------------------------------------------
    gotoxy(33, 6);
    cout << "######################################################################" << endl;
    gotoxy(33, 7);
    cout << "#                                                                    #" << endl;
    gotoxy(33, 8);
    cout << "#                                                                    #" << endl;
    gotoxy(33, 9);
    cout << "#                              AIRLINE                               #" << endl;
    gotoxy(33, 10);
    cout << "#                             MANAGMENT                              #" << endl;
    gotoxy(33, 11);
    cout << "#                              SYSTEM                                #" << endl;
    gotoxy(33, 12);
    cout << "#                                                                    #" << endl;
    gotoxy(33, 13);
    cout << "#                                                                    #" << endl;
    gotoxy(33, 14);
    cout << "######################################################################" << endl;

    string str = "Loading ... ";
    for (int i = 0; i <= 100; i++)
    {
        gotoxy(33, 16);
        cout << str << i << "%";
        Sleep(30);
    }

    gotoxy(33, 17);
    system("pause");
    system("CLS");
    //-----------------------------------------------------------------------------------------------------------------------
}

int menu()
{
    int choice;

    string L1 = "-----------------------------\n";
    string L2 = "|                           |\n";
    string L3 = "|1.ADMIN PAGE               |\n";
    string L4 = "|2.PASSENGER PAGE           |\n";
    string L5 = "|3.ABOUT US                 |\n";
    string L6 = "|4.EXIT                     |\n";
    string L7 = "|CHOICE -                   |\n";
    string L0 = "MAIN  PAGE                    ";
    string L01 = "------------------------------\n";

    //************************************************************
    gotoxy(55, 1);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L0);

    gotoxy(55, 2);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L01);

    gotoxy(55, 4);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);
    gotoxy(55, 5);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(55, 6);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L3);
    gotoxy(55, 7);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(55, 8);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 10);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L1);
    gotoxy(55, 11);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L2);
    gotoxy(55, 12);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L4);
    gotoxy(55, 13);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L2);
    gotoxy(55, 14);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 16);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);
    gotoxy(55, 17);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 18);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L5);
    gotoxy(55, 19);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 20);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 22);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);
    gotoxy(55, 23);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L2);
    gotoxy(55, 24);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L6);
    gotoxy(55, 25);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L2);
    gotoxy(55, 26);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);

    //--------------------------------------
    gotoxy(55, 28);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);
    gotoxy(55, 29);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L7);
    gotoxy(55, 30);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);

    gotoxy(66, 29);
    cin >> choice;

    system("CLS");
    return choice;
}

int menu_admin()
{
    int choice;

    string L1 = "-----------------------------\n";
    string L2 = "|                           |\n";
    string L3 = "|1.Flight                   |\n";
    string L4 = "|2.Route                    |\n";
    string L6 = "|3.BOOKED PASSENGERS        |\n";
    string L10 = "|4.EXIT                     |\n";
    string L7 = "|CHOICE -                   |\n";
    string L0 = "MAIN                          ";
    string L01 = "------------------------------\n";

    //************************************************************
    gotoxy(55, 1);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L0);

    gotoxy(55, 2);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L01);
    //***********************************************************
    gotoxy(55, 4);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);
    gotoxy(55, 5);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(55, 6);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L3);
    gotoxy(55, 7);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(55, 8);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 10);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L1);
    gotoxy(55, 11);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L2);
    gotoxy(55, 12);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L4);
    gotoxy(55, 13);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L2);
    gotoxy(55, 14);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 16);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);
    gotoxy(55, 17);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 18);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L6);
    gotoxy(55, 19);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 20);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);

    //--------------------------------------
    gotoxy(55, 22);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);
    gotoxy(55, 23);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 24);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L10);
    gotoxy(55, 25);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 26);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);

    //************************************************************
    gotoxy(55, 28);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);
    gotoxy(55, 29);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L2);
    gotoxy(55, 30);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L7);
    gotoxy(55, 31);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L2);
    gotoxy(55, 32);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);

    gotoxy(65, 30);
    cin >> choice;

    system("CLS");
    return choice;
}

int menu_f()
{

    int choice;

    string L1 = "-----------------------------\n";
    string L2 = "|                             |\n";
    string L3 = "|1. ADD FLIGHT                |\n";
    string L4 = "|2. REMOVE FLIGHT             |\n";
    string L5 = "|3. UPDATE FLIGHT             |\n";
    string L6 = "|4. SEARCH FLIGHT             |\n";
    string L8 = "|5. DISPLAY FLIGHT            |\n";
    string L9 = "|6. BACK                      |\n";
    string L7 = "|CHOICE -                    |\n";
    string L0 = "ADMIN PAGE                       ";
    string L01 = "-----------------------------------------------------------------------------------------";
    //************************************************************

    gotoxy(5, 1);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L0);

    gotoxy(5, 2);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L01);

    gotoxy(5, 4);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);
    gotoxy(5, 5);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(5, 6);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L3);
    gotoxy(5, 7);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(5, 8);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(35, 4);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L1);
    gotoxy(35, 5);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L2);
    gotoxy(35, 6);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L4);
    gotoxy(35, 7);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L2);
    gotoxy(35, 8);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(65, 4);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L1);
    gotoxy(65, 5);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L2);
    gotoxy(65, 6);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L5);
    gotoxy(65, 7);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L2);
    gotoxy(65, 8);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L1);

    gotoxy(5, 10);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);
    gotoxy(5, 11);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(5, 12);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L6);
    gotoxy(5, 13);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(5, 14);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(35, 10);
    SetConsoleTextAttribute(hConsole, 3);
    Delayer(L1);
    gotoxy(35, 11);
    SetConsoleTextAttribute(hConsole, 3);
    Delayer(L2);
    gotoxy(35, 12);
    SetConsoleTextAttribute(hConsole, 3);
    Delayer(L8);
    gotoxy(35, 13);
    SetConsoleTextAttribute(hConsole, 3);
    Delayer(L2);
    gotoxy(35, 14);
    SetConsoleTextAttribute(hConsole, 3);
    Delayer(L1);

    //--------------------------------------
    //---------------------------------------------------------------
    gotoxy(5, 15);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L1);
    gotoxy(5, 16);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L2);
    gotoxy(5, 17);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L9);
    gotoxy(5, 18);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L2);
    gotoxy(5, 19);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L1);
    //------------------------------------------
    gotoxy(10, 20);
    SetConsoleTextAttribute(hConsole, 13);
    Delayer(L1);
    gotoxy(10, 21);
    SetConsoleTextAttribute(hConsole, 13);
    Delayer(L7);
    gotoxy(10, 22);
    SetConsoleTextAttribute(hConsole, 13);
    Delayer(L1);

    gotoxy(23, 21);
    cin >> choice;

    system("CLS");
    return choice;
}

int menu_r()
{
    int choice;

    string L1 = "-----------------------------\n";
    string L2 = "|                             |\n";
    string L3 = "|1. Enter all routes          |\n";
    string L4 = "|2. Display all routes        |\n";
    string L5 = "|3. Total destinations        |\n";
    string L6 = "|4. Add new route             |\n";
    string L14 = "|5. Check for route           |\n";
    string L9 = "|6. BACK                      |\n";
    string L7 = "|CHOICE -                    |\n";
    string L0 = "MENU                             ";
    string L01 = "-----------------------------------------------------------------------------------------";
    //************************************************************

    gotoxy(5, 1);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L0);

    gotoxy(5, 2);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L01);

    gotoxy(5, 4);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);
    gotoxy(5, 5);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(5, 6);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L3);
    gotoxy(5, 7);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(5, 8);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(35, 4);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L1);
    gotoxy(35, 5);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L2);
    gotoxy(35, 6);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L4);
    gotoxy(35, 7);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L2);
    gotoxy(35, 8);
    SetConsoleTextAttribute(hConsole, 8);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(65, 4);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L1);
    gotoxy(65, 5);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L2);
    gotoxy(65, 6);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L5);
    gotoxy(65, 7);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L2);
    gotoxy(65, 8);
    SetConsoleTextAttribute(hConsole, 11);
    Delayer(L1);

    gotoxy(5, 10);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);
    gotoxy(5, 11);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(5, 12);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L6);
    gotoxy(5, 13);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(5, 14);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(65, 10);
    SetConsoleTextAttribute(hConsole, 4);
    Delayer(L1);
    gotoxy(65, 11);
    SetConsoleTextAttribute(hConsole, 4);
    Delayer(L2);

    gotoxy(65, 12);
    SetConsoleTextAttribute(hConsole, 4);
    Delayer(L14);

    gotoxy(65, 13);
    SetConsoleTextAttribute(hConsole, 4);
    Delayer(L2);
    gotoxy(65, 14);
    SetConsoleTextAttribute(hConsole, 4);
    Delayer(L1);
    //---------------------------------------------------------------
    gotoxy(5, 15);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L1);
    gotoxy(5, 16);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L2);
    gotoxy(5, 17);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L9);
    gotoxy(5, 18);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L2);
    gotoxy(5, 19);
    SetConsoleTextAttribute(hConsole, 6);
    Delayer(L1);
    //------------------------------------------

    gotoxy(10, 20);
    SetConsoleTextAttribute(hConsole, 13);
    Delayer(L1);
    gotoxy(10, 21);
    SetConsoleTextAttribute(hConsole, 13);
    Delayer(L7);
    gotoxy(10, 22);
    SetConsoleTextAttribute(hConsole, 13);
    Delayer(L1);

    gotoxy(22, 21);
    cin >> choice;

    system("CLS");
    return choice;
}
int menu_passenger()
{
    int choice;
    string L1 = "-----------------------------\n";
    string L2 = "|                           |\n";
    string L3 = "|1.Book a Flight            |\n";
    string L4 = "|2.Cancel Reservation       |\n";
    string L6 = "|3.EXIT                     |\n";
    string L7 = "|CHOICE -                   |\n";
    string L0 = "MAIN  PAGE                    ";
    string L01 = "------------------------------\n";

    //************************************************************
    gotoxy(55, 1);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L0);

    gotoxy(55, 2);
    SetConsoleTextAttribute(hConsole, 7);
    Delayer(L01);

    gotoxy(55, 4);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);
    gotoxy(55, 5);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(55, 6);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L3);
    gotoxy(55, 7);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L2);
    gotoxy(55, 8);
    SetConsoleTextAttribute(hConsole, 10);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 10);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L1);
    gotoxy(55, 11);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L2);
    gotoxy(55, 12);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L4);
    gotoxy(55, 13);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L2);
    gotoxy(55, 14);
    SetConsoleTextAttribute(hConsole, 12);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 16);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);
    gotoxy(55, 17);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 18);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L6);
    gotoxy(55, 19);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L2);
    gotoxy(55, 20);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(L1);

    //--------------------------------------
    //************************************************************
    gotoxy(55, 22);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);
    gotoxy(55, 23);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L2);
    gotoxy(55, 24);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L7);
    gotoxy(55, 25);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L2);
    gotoxy(55, 26);
    SetConsoleTextAttribute(hConsole, 9);
    Delayer(L1);

    gotoxy(65, 24);
    cin >> choice;

    system("CLS");
    return choice;
}

void aboutUs()
{
    system("CLS");
    string str1 = " ADDIS ABABA SCIENCE AND TECHNOLOGY UNIVERSITY ";
    string str2 = " COLLAGE OF ENGINEERING";
    string str3 = " DEPARTMENT OF SOFTWARE ENGINEERING";
    string str4 = " FUNDAMENTALS OF PROGRAMMING II";
    string str5 = " SECTION - D";
    string str6 = " Name                             ID_Number    ";
    string str17 = " Miheret Girmachew               ETS 1071/14   ";
    string str7 = " Mihret Desalegn                 ETS 1074/14   ";
    string str8 = " Mikias Goitom                   ETS 1080/14   ";
    string str9 = " Mikiyas Alemayehu Mekonen       ETS 1086/14   ";
    string str10 = " Mikiyas Alemayehu Gebrewold     ETS 1087/14  ";
    string str11 = " Milkias Yeheyis                 ETS 1101/14  ";
    string str12 = "                                       SUBMITTED TO: INST. Abduraman";
    string str13 = "**********************************************************************";
    string str14 = " ABOUT US";

    //*************************************
    gotoxy(33, 4);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str14);

    gotoxy(33, 5);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str13);

    gotoxy(43, 7);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str1);

    gotoxy(59, 9);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str2);

    gotoxy(53, 11);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str3);

    gotoxy(54, 13);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str4);

    gotoxy(62, 15);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str5);

    gotoxy(33, 17);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str6);

    gotoxy(33, 18);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str17);

    gotoxy(33, 19);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str7);

    gotoxy(33, 20);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str8);

    gotoxy(33, 21);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str9);

    gotoxy(33, 22);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str10);

    gotoxy(33, 23);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str11);

    gotoxy(33, 24);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str12);

    gotoxy(33, 26);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str13);

    gotoxy(33, 27);
    SetConsoleTextAttribute(hConsole, 14);

    gotoxy(33, 28);
    SetConsoleTextAttribute(hConsole, 14);
    Delayer(str13);
    //**************************************************

    system("pause");
    system("CLS");
}

int main()
{

    // flight object creation
    FlightManager fly;
    // Route object creation
    TreeNode *root = new TreeNode();
    // Route creation
    Tree route(root);
    // passenger object creation
    Passenger traveler;
    // passenger Managment object creation
    PassengerManagment passengerManage(fly.getHead());
    int numOfDestination = 0, passengerId;
    string from, newDestination, to, searchFlightNum, flightNum;
    TreeNode *toNode = new TreeNode(); // case 6

    // welcome page
    welcome_page();

    do
    {
    start:
        switch (menu())
        {
        case 1: // Admin
            do
            {
            start_a:
                switch (menu_admin())
                {
                case 1:
                    do
                    {
                        switch (menu_f())
                        {
                        case 1:
                            fly.addFlight(root);
                            system("pause");
                            system("cls");
                            break;
                        case 2:
                            fly.removeFlight();
                            system("pause");
                            system("cls");
                            break;
                        case 3:
                            fly.updateFlight();
                            system("pause");
                            system("cls");
                            break;
                        case 4:
                            cin.ignore();
                            cout << "Enter flight name: ";
                            getline(cin, searchFlightNum);
                            fly.showSearchedFlight(searchFlightNum);
                            system("pause");
                            system("cls");
                            break;
                        case 5:
                            fly.displayFlights();
                            system("pause");
                            system("cls");
                            break;
                        case 6:
                            goto start_a;
                        default:
                            cout << "Invalid input!" << endl;
                            system("pause");
                            system("cls");
                            break;
                        }
                    } while (true);

                    break;
                case 2:
                    do
                    {
                        switch (menu_r())
                        {

                        case 1:
                            cout << "\tSource City" << endl;
                            cin.ignore();
                            root = route.addRoutes();
                            if (root == NULL)
                            {
                                cout << "\nAdd route failed!" << endl;
                            }
                            else
                            {
                                cout << "\nSuccessfully added routes!" << endl;
                            }
                            system("pause");
                            system("cls");
                            break;

                        case 2:
                            route.displayAllRoute(root);
                            system("pause");
                            system("cls");
                            break;

                        case 3:
                            numOfDestination = route.totalDestination(root) - 1;
                            cout << "\nOur airline have: " << numOfDestination << " destinations" << endl;
                            system("pause");
                            system("cls");
                            break;

                        case 4:
                            cin.ignore();
                            cout << "Enter from: ";
                            getline(cin, from);
                            cout << "Enter new destination: ";
                            getline(cin, newDestination);
                            route.addCity(root, from, newDestination);
                            system("pause");
                            system("cls");
                            break;

                        case 5:
                            cin.ignore();
                            cout << "Enter from: ";
                            getline(cin, from);
                            cout << "Enter to: ";
                            getline(cin, to);
                            if (route.checkRoute(root, from, to))
                            {
                                cout << "Route exists!" << endl;
                            }
                            else
                            {
                                cout << "Route doesn't exist!" << endl;
                            }
                            system("pause");
                            system("cls");
                            break;
                        case 6:
                            goto start_a;
                        default:
                            cout << "Invalid Input! Enter from(1-6)" << endl;
                            system("pause");
                            system("cls");
                        }
                    } while (true);
                    break;
                case 3:
                    fly.displayPassengers();
                    system("pause");
                    system("cls");
                    break;
                case 4:
                    goto start;
                default:
                    break;
                }
            } while (true);
            break;
        case 2: // passenger
            do
            {
                switch (menu_passenger())
                {
                case 1:
                    if (fly.noFlights())
                    {
                        cout << "\nSorry! There are NO flights" << endl;
                    }
                    else
                    {
                        traveler = addPassenger();
                        passengerManage.displayFlights();
                        passengerManage.bookFlight(traveler);
                    }
                    system("pause");
                    system("cls");
                    break;
                case 2:
                    fly.displayPassengers();
                    cout << "\n_________________________________________________________________" << endl;
                    cout << "Enter passenger ID: ";
                    cin >> passengerId;
                    passengerManage.cancelReservation(passengerId);
                    system("pause");
                    system("cls");
                    break;
                case 3:
                    goto start;
                    break;
                default:
                    cout << "Invalid input!";
                    system("pause");
                    system("cls");
                    break;
                }
            } while (true);

            break;
        case 3:
            aboutUs();
            system("pause");
            system("cls");
            break;
        case 4: // exit
            system("pause");
            cout << "Exiting....." << endl;
            return 0;
        default:
            cout << "Invalid input!" << endl;
            system("pause");
            system("cls");
            break;
        }
    } while (true);
}
