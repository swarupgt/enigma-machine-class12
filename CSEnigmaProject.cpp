//Computer Science Investigatory Project by Swarup Totloor, Aditi Shanmugam, and Utpal Balse.

//The Enigma Machine.

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Rotor
{

	char rotor[27];   //The rotor wiring.
	int rotorIndex;

public:

	void Input()
	{
		cout << "Enter the rotor wiring(26 chars & CAPS only): ";
		cin.getline(rotor, 27);
		cin.getline(rotor, 27);
	}

	void SetIndex(int n)
	{
		rotorIndex = n;
	}

	int RetRindex()
	{
		return rotorIndex;
	}

	char* RetRotor()
	{
		return rotor;
	}
};


char Alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";         //String of alphabets.

char rotors[3][27];

char reflector[] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";


void SetRotors(Rotor rot[])						//Sets the rotor config for use in machine.
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			rotors[i][j] = rot[i].RetRotor()[j];
		}
		
	}
}

void ViewRotors(Rotor r)
{
	cout << r.RetRotor();
}


long mod26(long a)					//Modulo 26 for circular loop of alphabets.
{
	return (a % 26 + 26) % 26;
}

int LI(char l)			// Letter index.
{
	return l - 'A';
}

int indexof(char* array, int find)
{
	return strchr(array, find) - array;
}

string crypt(const char *ct, char key[])
{
	// Set initial combination of rotors.
	int L = LI(key[0]);
	int M = LI(key[1]);
	int R = LI(key[2]);

	string output;

	for (int x = 0; x < strlen(ct); x++)
	{
		if (ct[x] != ' ')
		{
			int ct_letter = LI(ct[x]);

			// Step right rotor on every iteration.
			R = mod26(R + 1);

			// Pass through rotors
			char a = rotors[2][mod26(R + ct_letter)];
			char b = rotors[1][mod26(M + LI(a) - R)];
			char c = rotors[0][mod26(L + LI(b) - M)];

			// Pass through reflector
			char ref = reflector[mod26(LI(c) - L)];

			// Inverse rotor pass
			int d = mod26(indexof(rotors[0], Alpha[mod26(LI(ref) + L)]) - L);
			int e = mod26(indexof(rotors[1], Alpha[mod26(d + M)]) - M);
			char f = Alpha[mod26(indexof(rotors[2], Alpha[mod26(e + R)]) - R)];

			output += f;
		}

		else
			output += " ";
	}

	return output;
}

//Encrypting/Decrypting Function.

void EncryptDecrypt()
{
	ifstream RotorFile("RotorFile.txt", ios::in, ios::binary);
	Rotor temprot, rot[3];
	int r[3];
	char NewKey[3];
	char InputText[200];
	cout << "Enter the rotor indices of the rotors you want to use: ";
	cin >> r[0] >> r[1] >> r[2];

	//Selection of rotors given their indices.

	for (int i = 0; i < 3; i++)
	{
		while (!RotorFile.eof())
		{
			RotorFile.read((char*)&temprot, sizeof(temprot));
			int d = temprot.RetRindex();
			cout << d;
			if (d == r[i])
			{
				rot[i] = temprot;
				break;
			}
		}
	}

	SetRotors(rot);

	
	cout << "Enter the key for each rotor's setting(CAPS only) (ex. - A B C): ";
	for (int i = 0; i < 3; i++)
	{
		cin >> NewKey[i];
	}
	cout << "Enter String to encrypt/decrypt(max. 200 chars & CAPS only): ";
	cin.getline(InputText, 200);
	cin.getline(InputText, 200);
	cout << "Using rotors with indices: ";
	for (int i = 0; i < 3; i++)
	{
		cout << r[i] << "\t";
	}

	//Output.
	string sum = crypt(InputText, NewKey);
	cout <<"\nResultant String is:" << sum;
	RotorFile.close();
}


//Function to add new rotors into program's file.

void AddNewRotor()
{
	Rotor temp;
	temp.Input();
	char tempString[27];
	strcpy_s(tempString, temp.RetRotor());
	ofstream RotorFile;
	RotorFile.open("RotorFile.txt", ios::app, ios::binary);
	ifstream RotorIndexFile("RotorIndexFile.txt", ios::in);
	cout << "New Rotor with wiring: ";
	for (int i = 0; i < 26; i++)
	{
		cout << tempString[i];
	}

	int index;
	RotorIndexFile >> index;
	temp.SetIndex(index);		//Sets index of temporary object.
	cout << "\nRotor Index: " << temp.RetRindex();
	RotorIndexFile.close();
	RotorFile.write((char*)&temp, sizeof(temp));
	ofstream RIFile("RotorIndexFile.txt", ios::out, ios::trunc);
	RIFile << ++index;		//Index then permanently incremented for future use.
	RIFile.close();
	char ch;
	cin >> ch;
}

//Function to View available rotors.

void ViewAllRotors()
{
	cout << "Available rotors for use:" << endl;
	ifstream RotorFile;
	RotorFile.open("RotorFile.txt", ios::in, ios::binary);
	while (!RotorFile.eof())
	{
		Rotor tempa;
		RotorFile.read((char*)&tempa, sizeof(tempa));
		cout << "Rotor: " << tempa.RetRotor() <<"\tRotor Index: "<< tempa.RetRindex()<< endl;
	}
}


//Main function.

int main()
{
	int choice;
	char endchoice;
	cout << "______________ENIGMA(I) MACHINE________________" << endl << endl;
	cout << "Description: " << endl;
	cout << "The Enigma Machine was a device used by the German Nazis in the World War II to send encrypted messages";
	cout << " to several of their bases. This cipher was considered to be one of the hardest ciphers to ever have been cracked. " << endl;
	cout << endl;
	cout << "It is a 3-rotor based machine with a hardwired reflector.";
	cout << "\nProgram by Swarup Totloor, Aditi S, and Utpal Balse.\n";
	do 
	{
		cout << "\nMENU: " << endl;
		cout << "Enter 1 to Encrypt/Decrypt a message. \nEnter 2 to Add a new rotor. \nEnter 3 to view all available rotors.\n";
		cout << "Choice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			{
				EncryptDecrypt();
				break;
			}

		case 2:
			{
				AddNewRotor();
				break;
			}

		case 3:
			{
				ViewAllRotors();
				break;
			}
		default:
			{
				cout << "Oops! Wrong Choice." << endl;
				break;
			}

		}
		cout << "\nDo you want to continue?(y/n): ";
		cin >> endchoice;
	}

	while (endchoice == 'y' || endchoice == 'Y');

	cout << "Exiting Program...";

}

