#ifndef LAB4_2_MAIN_MENU_H
#define LAB4_2_MAIN_MENU_H

using namespace std;

char input(void (*func)()){
    char choice;
    char c;
    int i;
    system("cls");
    func();
    cout << "Select paragraph" << endl;
    choice=getchar();
    i = 0;
    if (choice!='\n') while ((c = getchar()) != '\n')i++;
    if (i) choice = '-';
    return choice;
}

void server_menu(){
    cout << "SERVER MENU\n"
         << "---------------------------------------\n"
         << "1. Create named pipe\n"
         << "2. Open connection to the pipe\n"
         << "3. Enter the message\n"
         << "4. Disconnect from the pipe\n"
         << "0. Exit"
         << endl;
}

void client_menu(){
    cout << "CLIENT MENU\n"
         << "---------------------------------------\n"
         << "1. Connect to the pipe\n"
         << "2. Receive message from pipe\n"
         << "3. Disconnect from the pipe\n"
         << "0. Exit"
         << endl;
}

#endif //LAB4_2_MAIN_MENU_H
