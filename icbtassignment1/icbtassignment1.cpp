

#include <iostream>
#include <string>
using namespace std;
//Room Struct
struct Room
{
	string roomID;
	string roomType;
	bool isAC;
	bool isAvailable;
	double pricePerNight;
};
//Booking Struct
struct Booking
{
	string customerName;
	string roomID;
	int numberNights;
	double totalAmount;
};
//User Struct
struct User
{
	string username;
	string password;
	string role;
};

//Function declarations
void displayMainMenu(string);
int roomManagementMenu(Room[], int, Booking[], int&);
int orderManagementMenu(Booking[], int&, Room[], int);
bool bookRoom(Room[], int, Booking[], int&);
void viewAvailableRooms(Room[], int);
void viewReservedRooms(Booking[], int, Room[], int);
void searchRoombyID(Room[], int);
void viewBookings(Booking[], int);
double calculateBill(const Room&, int);
void generateBill(Booking[], int, Room[], int, string);
bool login(User[], int, string&, string&);

int main()
{
	int const MAX_ROOMS = 12;
	int const MAX_BOOKINGS = 20;
	int const MAX_USERS = 2;

	Room rooms[MAX_ROOMS] = {
	{"101", "Single Room", false, true, 5000.0},
    {"102", "Double Room", false, true, 8000.0},
    {"103", "Twin Room",  false, true, 10000.0},
    {"104", "Family Room", false, true, 15000.0},
    {"105", "Standard Room", false, true, 6000.0},
    {"106", "Deluxe Room", false, true, 11000.0},
    {"107", "AC Single Room", true, true, 6000.0},
    {"108", "AC Double Room", true, true, 9000.0},
    {"109", "AC Twin Room", true, true, 11000.0},
    {"110", "AC Family Room", true, true, 16000.0},
    {"111", "AC Standard Room", true, true, 7000.0},
    {"112", "AC Deluxe Room", true, true, 12000.0}
};
	Booking bookings[MAX_BOOKINGS] = {};

	User users[MAX_USERS] = {
		{"Receptionist","pw2464","Receptionist"},
		{"Admin","pw1234","Admin"}
	};
	int roomCount = MAX_ROOMS;
	int bookingCount = 0;
	string currentUser, currentRole;

	if (login(users, MAX_USERS, currentUser, currentRole))
	{
		int choice;
		do {
			displayMainMenu(currentRole);
			cout << "Enter Your Choice : ";
			cin >> choice;

			switch (choice)
			{
			case 1:
				choice = roomManagementMenu(rooms, roomCount, bookings, bookingCount);
				break;
			case 2:
				choice = orderManagementMenu(bookings, bookingCount, rooms, roomCount);
				break;
			case 3:
				cout << "Exiting the program..." << endl;
				break;
			default:
				cout << "Invalid Choice. Please try again. " << endl;
				break;
			}
		} while (choice != 3);
	}
	else
	{
		cout << "Login Failed. Exiting the Program..." << endl;
	}

	return 0;
}
void displayMainMenu(string role)
{
	cout << "\nMoon Hotel Automation System" << endl;
	cout << "...Welcome to Our Platform..." << endl;
	cout << "Logged in as : " << role << endl;
	cout << "1 . Room Management " << endl;
	cout << "2 . Order Management " << endl;
	cout << "3 . Exit " << endl;
}
bool login(User users[], int userCount, string& currentUser, string& currentRole)
{
	string username, password;
	cout << "Enter Username : ";
	cin >> username;
	cout << "Enter Password : ";
	cin >> password;

	for (int i = 0;i < userCount;i++)
	{
		if (users[i].username == username && users[i].password == password)
		{
			currentUser = users[i].username;
			currentRole = users[i].role;
			return true;
		}
	}
	return false;
}
int roomManagementMenu(Room rooms[], int roomCount, Booking bookings[], int& bookingCount)
{
	int choice;
	do
	{
		cout << "\nRoom Management Menu " << endl;
		cout << "1. View Avaliable Rooms " << endl;
		cout << "2. View Reserved Rooms " << endl;
		cout << "3. Search Room by ID " << endl;
		cout << "4. Book a Room " << endl;
		cout << "5. Returm to Main Menu " << endl;

		cout << " Enter Your Choice : ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			viewAvailableRooms(rooms, roomCount);
			break;
		case 2:
			viewReservedRooms(bookings, bookingCount, rooms, roomCount);
			break;
		case 3:
			searchRoombyID(rooms, roomCount);
			break;
		case 4:
			if (bookRoom(rooms, roomCount, bookings, bookingCount))
			{
				cout << " Room booked successfully ! " << endl;
			}
			else
			{
				cout << " Failed to book the Room " << endl;
			}
			break;
		case 5:
			cout << " Returning to the Main Menu..." << endl;
			return choice;
		default:
			cout << " Invalid Choice. Please try again " << endl;
			break;
		}
	} while (choice != 5);
	return choice;
}
int orderManagementMenu(Booking bookings[], int& bookingCount, Room rooms[], int roomCount)
{
	int choice;
	do
	{
		cout << "\nOrder Management Menu" << endl;
		cout << "1. View Bookings " << endl;
		cout << "2. Generate Bill " << endl;
		cout << "3. Return to Main Menu " << endl;

		cout << " Enter Your Choice : ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			viewBookings(bookings, bookingCount);
			break;
		case 2:
		{
			string roomID;
			cout << " Enter the Room ID : ";
			cin >> roomID;
			generateBill(bookings, bookingCount, rooms, roomCount, roomID);
			break;
		}
		case 3:
			cout << " Returning to the Main Menu..." << endl;
			return choice;
		default:
			cout << " Invalid Choice. Please try again " << endl;
			break;
		}
	} while (choice != 3);
	return choice;
}
bool bookRoom(Room rooms[], int roomCount, Booking bookings[], int& bookingCount)
{
	string roomID;
	int numberNights;
	string customerName;

	cout << " Enter the Room ID : ";
	cin >> roomID;
	cout << " Enter the number of Nights : ";
	cin >> numberNights;
	cout << " Enter the Customer Name : ";
	cin >> customerName;

	//Find the room & check availability
	bool roomFound = false;
	for (int i = 0;i < roomCount;i++)
	{
		if (rooms[i].roomID == roomID)
		{
			roomFound = true;
			if (rooms[i].isAvailable)
			{
				rooms[i].isAvailable = false;
				double totalAmount = calculateBill(rooms[i], numberNights);
				bookings[bookingCount] = { customerName,roomID,numberNights,totalAmount };

				bookingCount++;
				return true;
			}
			else
			{
				cout << " Sorry. The Room is not available. " << endl;
				return false;
			}
		}
	}
	if (!roomFound)
	{
		cout << " Invalid Room ID. Please try again. " << endl;
	}
	return false;
}
void viewAvailableRooms(Room rooms[], int roomCount)
{
	cout << " \nAvailable Rooms : " << endl;
	for (int i = 0;i < roomCount;i++)
	{
		if (rooms[i].isAvailable)
		{
			cout << " Room ID : " << rooms[i].roomID << " , Room Type : " << rooms[i].roomType << " , AC : " << (rooms[i].isAC ? "Yes" : "No") << " , Price per Night : LKR " << rooms[i].pricePerNight << endl;


		}
	}
}
void viewReservedRooms(Booking bookings[], int bookingCount, Room rooms[], int roomCount)
{
	cout << " \nReserved Rooms : " << endl;
	for (int i = 0;i < bookingCount;i++)
	{
		for (int j = 0;j < roomCount;j++)
		{

			if (rooms[j].roomID == bookings[i].roomID)
			{

				cout << " Room ID : " << rooms[j].roomID << " , Room Type : " << rooms[j].roomType << " , AC : " << (rooms[j].isAC ? "Yes" : "NO") << " , Customer :" << bookings[i].customerName << " , Nights : " << bookings[i].numberNights << " , Total : LKR " << bookings[i].totalAmount << endl;
				break;
			}
		}
	}
}
void searchRoombyID(Room rooms[], int roomCount)
{
	string roomID;
	cout << " Enter the Room ID : ";
	cin >> roomID;

	bool roomFound = false;
	for (int i = 0;i < roomCount;i++)
	{
		if (rooms[i].roomID == roomID)
		{
			cout << " Room ID : " << rooms[i].roomID << " ,Type : " << rooms[i].roomType << " , AC : " << (rooms[i].isAC ? "Yes" : "No") << " , Available : " << (rooms[i].isAvailable ? "Yes" : "No") << " , Price Per Night : LKR" << rooms[i].pricePerNight << endl;
			roomFound = true;
			break;

		}
	}
	if (!roomFound)
	{
		cout << " Invalid Room ID. Please try Again. " << endl;
	}
}
void viewBookings(Booking bookings[], int bookingCount)
{
	cout << " \nBookings : " << endl;
	for (int i = 0;i < bookingCount;i++)
	{
		cout << " Customer : " << bookings[i].customerName << " , Room ID : " << bookings[i].roomID << " ,Nights : " << bookings[i].numberNights << " , Total : LKR " << bookings[i].totalAmount << endl;
	}
}
double calculateBill(const Room& room, int numberNights)
{
	return room.pricePerNight * numberNights;
}
void generateBill(Booking bookings[], int bookingCount, Room rooms[], int roomCount, string roomID)
{
	bool bookingFound = false;
	for (int i = 0;i < bookingCount;i++)
	{
		if (bookings[i].roomID == roomID)
		{
			for (int j = 0;j < roomCount;j++)
			{
				if (rooms[j].roomID == bookings[i].roomID)
				{
					cout << " Customer : " << bookings[i].customerName << ", Room ID : " << bookings[i].roomID << ", Room Type : " << rooms[j].roomType << ", AC : " << (rooms[j].isAC ? "Yes" : "No") << ", Nights : " << bookings[i].numberNights << ", Total : LKR " << bookings[i].totalAmount << endl;
					bookingFound = true;
					break;
				}
			}
			break;
		}
	}
	if (!bookingFound)
	{
		cout << " No booking found for the given Room ID. " << endl;
	}
}








