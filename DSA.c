#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include <stdbool.h>


//MAX NUMBER OF NODES
#define N 10000


char places[N][20] = {""};
int noOfPlaces = 0;

struct Bus {
    char name[20];
    char source[20];
    char destination[20];
    char schedule[20];  // Add schedule information
    int availableSeats;
    float bookingAmount;  // Add booking amount information
};

//maintains output screen
void heading()
{
printf("\t*****************************************************************\n");
printf("\t*          BUS MANAGEMENT AND BOOKING SYSTEM            \t*\n");
printf("\t*                                                       \t*\n");
printf("\t*         Buses create smooth journeys for all.         \t*\n");
printf("\t*                                                  \t\t*\n");
printf("\t*****************************************************************\n");
}


// Function to add a new bus to the system

void addBus() {
    struct Bus newbus;

    printf("Enter Bus name: ");
    scanf("%s", newbus.name);

    printf("Enter source: ");
    scanf("%s", newbus.source);

    printf("Enter destination: ");
    scanf("%s", newbus.destination);

    printf("Enter schedule: ");
    scanf("%s", newbus.schedule);

    printf("Enter available seats: ");
    scanf("%d", &newbus.availableSeats);

    printf("Enter booking amount: ");
    scanf("%f", &newbus.bookingAmount);

    FILE *BusFile = fopen("bus_details.txt", "a");
    if (BusFile == NULL) {
        printf("Error opening bus details file for writing.\n");
        return;
    }

    fprintf(BusFile, "%s %s %s %s %d %.2f\n", newbus.name, newbus.source, newbus.destination, newbus.schedule, newbus.availableSeats, newbus.bookingAmount);
    fclose(BusFile);

    printf("Bus %s added successfully.\n", newbus.name);
}

//FUNCTION TO DELETE THE BUS
void deleteBus() {
    FILE *tempFile, *BusFile;
    struct Bus currentBus;
    char BusNameToDelete[50];

    printf("Enter the name of the Bus to delete: ");
    scanf("%s", BusNameToDelete);

    // Open the original file for reading
    BusFile = fopen("bus_details.txt", "r");
    if (BusFile == NULL) {
        printf("Error opening bus details file for reading.\n");
        return;
    }

    // Open a temporary file for writing
    tempFile = fopen("temp_bus_details.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(BusFile);
        return;
    }

    // Read each line from the original file and write to the temporary file, excluding the buses to be deleted
    while (fscanf(BusFile, "%s %s %s %s %d %f", currentBus.name, currentBus.source, currentBus.destination, currentBus.schedule, &currentBus.availableSeats, &currentBus.bookingAmount) == 6) {
        if (strcmp(currentBus.name, BusNameToDelete) != 0) {
            fprintf(tempFile, "%s %s %s %s %d %f\n", currentBus.name, currentBus.source, currentBus.destination, currentBus.schedule, currentBus.availableSeats, currentBus.bookingAmount);
        }
    }

    // Close both files
    fclose(BusFile);
    fclose(tempFile);

    // Remove the original file
    if (remove("bus_details.txt") != 0) {
        printf("Error deleting the original file.\n");
        return;
    }

    // Rename the temporary file to the original file
    if (rename("temp_bus_details.txt", "bus_details.txt") != 0) {
        printf("Error renaming the temporary file.\n");
        return;
    }

    printf("Bus %s deleted successfully.\n", BusNameToDelete);
}


//FUNCTION TO DISPLAY ALL THE AVAILABLE BUSES
void displayBus() {
    FILE *BusFile = fopen("bus_details.txt", "r");
    if (BusFile == NULL) {
        printf("Error opening bus details file for reading.\n");
        return;
    }

    struct Bus bus;
    printf("\nAvailable buses:\n");

    while (fscanf(BusFile, "%s %s %s %s %d %f", bus.name, bus.source, bus.destination, bus.schedule, &bus.availableSeats, &bus.bookingAmount) == 6) {
        printf("Name: %s, Source: %s, Destination: %s, Schedule: %s, Available Seats: %d, Booking Amount: %.2f\n", bus.name, bus.source, bus.destination, bus.schedule, bus.availableSeats, bus.bookingAmount);
    }

    fclose(BusFile);
}


//FUNCTION TO BOOK A SEAT IN THE Bus

void bookSeat() {
    char busName[20];
    int seatsToBook;
    float totalAmount;

    printf("Enter bus name for booking: ");
    scanf("%s", busName);


    FILE *BusFile = fopen("bus_details.txt", "r+");

    if (BusFile == NULL) {
        printf("Error opening Bus details file for reading and writing.\n");
        return;
    }

    struct Bus bus;

    bool found = false;

    while (fscanf(BusFile, "%s %s %s %s %d %f", bus.name, bus.source, bus.destination, bus.schedule, &bus.availableSeats, &bus.bookingAmount) != EOF) {
        if (strcmp(bus.name, busName) == 0) {
            found = true;
            printf("Enter number of seats to book: ");
            scanf("%d", &seatsToBook);

            if (seatsToBook <= bus.availableSeats) {
                totalAmount = seatsToBook * bus.bookingAmount;
                bus.availableSeats -= seatsToBook;
                fseek(BusFile, -sizeof(struct Bus), SEEK_CUR);
                fprintf(BusFile, "%s %s %s %s %d %.2f", bus.name, bus.source, bus.destination, bus.schedule, bus.availableSeats, bus.bookingAmount);
                fflush(BusFile);

                printf("Booking successful. %d seat(s) booked on bus %s. Total Amount: %.2f\n", seatsToBook, bus.name, totalAmount);
            }
            else {
                printf("Not enough available seats on bus %s.\n", bus.name);
            }

            break;
        }
    }

    fclose(BusFile);

    if (!found) {
        printf("Bus %s not found.\n", busName);
    }
}

//FUNCTION TO CANCEL A SEAT IN THE Bus

void cancelSeat() {
    char busName[20];
    int seatsToCancel;
    float refundAmount;

    printf("Enter bus name for seat cancellation: ");
    scanf("%s", busName);

    FILE *BusFile = fopen("bus_details.txt", "r+");
    if (BusFile == NULL) {
        printf("Error opening Bus details file for reading and writing.\n");
        return;
    }

    struct Bus bus;

    bool found = false;
    while (fscanf(BusFile, "%s %s %s %s %d %f", bus.name, bus.source, bus.destination, bus.schedule, &bus.availableSeats, &bus.bookingAmount) != EOF) {
        if (strcmp(bus.name, busName) == 0) {
            found = true;
            printf("Enter number of seats to cancel: ");
            scanf("%d", &seatsToCancel);

            if (seatsToCancel > 0 && seatsToCancel <= (bus.availableSeats)) {
                refundAmount = seatsToCancel * bus.bookingAmount;
                bus.availableSeats += seatsToCancel;
                fseek(BusFile, -sizeof(struct Bus), SEEK_CUR);
                fprintf(BusFile, "%s %s %s %s %d %.2f", bus.name, bus.source, bus.destination, bus.schedule, bus.availableSeats, bus.bookingAmount);
                fflush(BusFile);

                printf("Cancellation successful. %d seat(s) canceled on bus %s. Refund Amount: %.2f\n", seatsToCancel, bus.name, refundAmount);
            } else {
                printf("Invalid number of seats to cancel on bus %s.\n", bus.name);
            }

            break;
        }
    }

    fclose(BusFile);

    if (!found) {
        printf("Bus %s not found.\n", busName);
    }
}

//CODE TO COMPARE TWO STRINGS
int isGreateString(char first[20], char second[20]){
    int i=0, j=0;
    int n = strlen(first);
    int m = strlen(second);


    while(i<n && j<m){
        if(first[i]==second[j]){
            i++, j++;
        } else if(first[i] > second[j]){
            return 1;
        } else{
            return 0;
        }
    }

    if(j<m) return 0;
    else return 1;
}
   //FUNCTION WHICH PROVIDES KEY FOR THE GIVEN LOCATION
int findKeyForString(char place[20]){
    int lo = 0, hi = noOfPlaces-1;
    int mid = 0;

    while(lo <= hi){
        mid = (lo+hi) >> 1;

        if(strcmp(places[mid], place)==0){
            return mid;
        } else if(isGreateString(place, places[mid])){
            lo = mid+1;
        } else{
            hi = mid-1;
        }
    }

    return -1;
}


//FUNCTION TO SORT PLACES NAMES IN ALPHABATICAL ORDER
void sortPlaces(){
    int n = noOfPlaces;
    char temp[20];
    for(int i=0; i<n; i++){
        for(int j=0; j<n-1; j++){
            if(isGreateString(places[j], places[j+1])){
                strcpy(temp, places[j]);
                strcpy(places[j], places[j+1]);
                strcpy(places[j+1], temp);
            }
        }
    }

}


//ASSIGNING UNIQUE VALUE FOR EVERY PLACE
void addPlaces(){

    strcat(places[0], "bengaluru");
    noOfPlaces++;
    strcat(places[1], "mumbai");
    noOfPlaces++;
    strcat(places[2], "delhi");
    noOfPlaces++;
    strcat(places[3], "indore");
    noOfPlaces++;
    strcat(places[4], "assam");
    noOfPlaces++;
    strcat(places[5], "punjab");
    noOfPlaces++;


    sortPlaces();
}

//FUNCTION TO DISPLAY ALL LOCATIONS WHERE BUSES ARE AVAILABLE
void displayPlaces(){
    int i=0;
    printf("called\n");
    while(strcmp(places[i], "")){
        printf("%s ", places[i]);
        i++;
    }
    printf("\n");
}


//FUNCTIO WHICH CONVERTS ALL CHARECTERS IN A STRING INTO LOWERCASES
void convertToLowerCase(char str[100])
{
    for (int i = 0; i <= strlen(str); i++)
    {
        // The ASCII value of A is 65 and Z is 90
        if (str[i] >= 65 && str[i] <= 90)
            str[i] = str[i] + 32; /* add 32 to string character to convert into lower case. */
    }

    // printf("in lower case %s\n", str);
}


// TEMPROERY NODES USED IN DIJKISTRA
struct TempNode{
    int data;
    int cost;
};

typedef struct TempNode tempNode;


// GLOBAL HEAP
tempNode heap[1000000];
int heapSize = 0;


// FUNCTION TO DISPLAY HEAP
void printHeap(){
    printf("\n");
    for(int i=0; i<heapSize; i++){
        printf("{%d, %d} ", heap[i].data, heap[i].cost);
    }
    printf("\n");
}

// HEAPIFY FUNCTION TO MAINTAIN HEAP PROPERTY.
void heapify(int n, int i)
{

    int largest = i;

    while (i < n)
    {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;


        if (leftChild < n && heap[leftChild].cost > heap[largest].cost)
        {
            largest = leftChild;
        }

        if (rightChild < n && heap[rightChild].cost > heap[largest].cost)
        {
            largest = rightChild;
        }

        if (i != largest)
        {
            tempNode temp = heap[i];
            heap[i] =  heap[largest];
            heap[largest] = temp;
            i = largest;
        }
        else
        {
            break;
        }
    }

}
// FUNCTION TO BUILD A HEAP FROM HEAP.
void buildHeap()
{

    int n = heapSize;

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(n, i);
    }

    for (int i = n - 1; i > 0; i--)
    {
        tempNode temp = heap[i];
        heap[i] = heap[0];
        heap[0] = temp;
        n--;
        heapify(n, 0);
    }
}

// FUNCTION TO SORT ANS HEAP USING HEAP SORT.
void heapSort()
{

    buildHeap();
}

void heapPush(int data, int cost){
    tempNode newNode;
    newNode.data = data;
    newNode.cost = cost;

    heap[heapSize] = newNode;
    heapSize++;
    heapSort();

}

int isHeapEmpty(){
    if(heapSize==0) return 1;
    else return 0;
}

tempNode heapTop(){
    if(!isHeapEmpty()){
        return heap[0];
    } else{
        tempNode newNode = {-1, -1};
        return newNode;
    }
}

void heapPop(){
    if(!isHeapEmpty()){
        heap[0] = heap[heapSize-1];
        heapSize--;
        heapSort();
    } else{
        printf("Heap is Empty\n");
        return ;
    }
}




// STRUCTURE OF A SINGLE NODE
struct ListNode{
    int v;
    int cost;
    struct ListNode* next;
};


typedef struct ListNode node;

//FUNCTION WHICH CREATES AND RETURNS NEW NODE
node* createNewNode(int v, int cost){
    node* newNode = (node*)malloc(sizeof(node));
    newNode->v = v;
    newNode->cost = cost;
    newNode->next = NULL;

    return newNode;
}


// STRUCTURE OF GRAPH
struct Graph{
    node* head[N];
    node* tail[N];
};


typedef struct Graph graph;


// MAIN GRAPH
graph *g = NULL;


// ASSIGNING NULL AS INITIAL VALUES FOR ALL LINKED LISTS
void assignNullInitially(){
    // GRAPH INITIATION
    g = (graph*)malloc(sizeof(graph));

    for(int i=0; i<N; i++){
        g->head[i] = NULL;
        g->tail[i] = NULL;
    }
}


// FUNCTION FOR ADDING AN NEW EDGE
void addEdge(int u, int v, int cost){

    // printf("%d %d %d\n", u, v, cost);

    node* newNode = createNewNode(v, cost);
    node* new_node = createNewNode(u, cost);

    //USING INSERT AT TAIL
    if(!g->head[u]){
        g->head[u] = newNode;
    } else{
        g->tail[u]->next = newNode;
    }
        g->tail[u] = newNode;

    // if(!g->head[v]){
    //     g->head[v] = new_node;
    // } else{
    //     g->tail[v]->next = new_node;
    // }
    // g->tail[v] = new_node;

}


//FUNCTION TO FORM A CONNECTION BETWEEN 2 PLACES USING STRING FORMAT
void formConnectionBetweenPlaces(){
    addEdge(findKeyForString("bengaluru"), findKeyForString("mumbai"), 2);
    addEdge(findKeyForString("bengaluru"), findKeyForString("delhi"), 7);
    addEdge(findKeyForString("mumbai"), findKeyForString("delhi"), 3);
    addEdge(findKeyForString("delhi"), findKeyForString("assam"), 5);
    addEdge(findKeyForString("delhi"), findKeyForString("indore"), 1);
    addEdge(findKeyForString("indore"), findKeyForString("mumbai"), 4);
    addEdge(findKeyForString("indore"), findKeyForString("punjab"), 2);
    addEdge(findKeyForString("indore"), findKeyForString("assam"), 7);
    addEdge(findKeyForString("assam"), findKeyForString("punjab"), 1);

}
//FUNCTION TO DISPLAY THE GRAPH
void displayGraph(){

    node* temp = NULL;

    for(int i=0; i<N; i++){
        int flag = 0;
        temp = g->head[i];
        if(temp){
            printf("%d -> ", i);
        }

        while(temp){
            printf("{%d, %d} ", temp->v, temp->cost);
            temp = temp->next;
            flag = 1;
        }

        if(flag) printf("\n");
    }

}

void dfs(int i, int visited[], int cost){
    node* temp = g->head[i];
    printf("{%d %d} -> ", i, cost);
    visited[i] = 1;


    while(temp){
        int v = temp->v;
        if(!visited[v]){
            dfs(v, visited, temp->cost);
        }
        temp = temp->next;
    }
}

void dfs_traversal(){
    int visited[N] = {0};

    for(int i=0; i<N; i++){
        if(!visited[i] && g->head[i]){
            dfs(i, visited, 0);
            printf("NULL\n");
        }
    }
}
void dijkstra(int s, int d){
    heapPush(s, 0);
    int dest = d;
    int parent[N] = {-1};
    int cost[N] = {10000};
    for(int i=0; i<N; i++){
        cost[i] = 100000;
        parent[i] = i;
    }

    cost[s] = 0;
    parent[s] = s;

    while(!isHeapEmpty()){
        tempNode topEle = heapTop();
        heapPop();

        int u = topEle.data;
        int w = cost[u];
        node* temp = g->head[u];
        while(temp){
            if(w + temp->cost < cost[temp->v]){
                cost[temp->v] = w + temp->cost ;
                parent[temp->v] = u;
                heapPush(temp->v, cost[temp->v]);
            }
            temp = temp->next;
        }
    }



    int tempArr[N] = {-1};
    int idx = 0;
    while(parent[d]!=d){
        tempArr[idx++] = d;
        d = parent[d];
    }

    int tempArrSize = idx;

    int i=0, j=tempArrSize-1;

    while(i<j){
        int temp = tempArr[i];
        tempArr[i] = tempArr[j];
        tempArr[j] = temp;
        i++;
        j--;
    }

    if(cost[dest]==100000){
        printf("\nSorry there is no buses available from %s to %s\n", places[s], places[dest]);
        return;
    }

    printf("%s -> ", places[s]);
    for(int i=0; i<tempArrSize-1; i++){
        printf("%s -> ", places[tempArr[i]]);
    }
    printf("%s\n", places[dest]);
    printf("total cost %d\n", cost[dest]);
}


//SHORTEST PATH W.R.T COST
void shortest_cost_from_source_to_des_w_r_t_cost(){
    char source_name[20], dest_name[20];
    printf("Enter source ");
    scanf("%s", source_name);
    printf("Enter destination ");
    scanf("%s", dest_name);

    //COVERT THE STRING INTO LOWERACSE TO AVOID AMBIGUITY BETWEEN SYSTEM AND USER
    convertToLowerCase(source_name);
    convertToLowerCase(dest_name);


    int u = findKeyForString(source_name);
    int v = findKeyForString(dest_name);

    if(u==-1 || v==-1){
        printf("\nSorry! There is no Bus availability from %s -> %s\n", source_name, dest_name);
        return;
    }
    dijkstra(u, v);
}




//STACK ARRAY USED IN FINDING ALL POSSIBLE ROOTS
int stackArray[N] = {-1};
int stackArraySize = 0;
int flagForPath = 0;

//STACK USED TO KEEP TRACK OF ALL THE NODES WHICH ARE IN A PATH
void printStackArray(){
    printf("\n");

    for(int i=0; i<stackArraySize-1; i++){
        printf("%s -> ", places[stackArray[i]]);
    }
    printf("%s", places[stackArray[stackArraySize-1]]);
    printf("\n");
}

//DFS USED FOR FINDING ALL PATHS
void dfs_for_finding_all_paths(int currNode, int dest, int visited[]){

    stackArray[stackArraySize] = currNode;
    stackArraySize++;

    if(currNode == dest){
        flagForPath = 1;
        printStackArray();
        stackArraySize--;
        return;
    }

    node* temp = g->head[currNode];
    visited[currNode] = 1;

    while(temp){
        int v = temp->v;
        if(!visited[v]){
            dfs_for_finding_all_paths(v, dest, visited);
        }
        temp = temp->next;
    }

    visited[currNode] = 0;
    stackArraySize--;

}

//FUNCTION TO FIND ALL POSSIBLE BUSES/ROOTS
void viewAllPossiblePaths(){

    char source_name[20], dest_name[20];
    printf("Enter source ");
    scanf("%s", source_name);
    printf("Enter destination ");
    scanf("%s", dest_name);

    //COVERT THE STRING INTO LOWERACSE TO AVOID AMBIGUITY BETWEEN SYSTEM AND USER
    convertToLowerCase(source_name);
    convertToLowerCase(dest_name);


    int source = findKeyForString(source_name);
    int dest = findKeyForString(dest_name);

    if(source==-1 || dest==-1){
        printf("\nSorry! There is no Buses availability from %s -> %s\n", source_name, dest_name);
        return;
    }

    int visited[N] = {0};
    dfs_for_finding_all_paths(source, dest, visited);

    if(flagForPath==0){
        printf("\nSorry! There is no Buses availability from %s -> %s\n", source_name, dest_name);
        return;
    }

    char ch;
    printf("\nPress 1 to get root from %s to %s which has minimum cost\n", source_name, dest_name);
    printf("\nelse press any other key than 1\n");
    fflush(stdin);
    scanf("%c", &ch);
    if(ch=='1') ch = '1';

    if(ch=='1') {
        printf("\n");
        dijkstra(source, dest);
    }
    //getch();
    return;
}
//MENU FOR BUSES
void menuForBuses(){
    printf("\n1-View all the roots that are available from source to destination\n2-View the minimum costed path from source to destination\n3.View the availabel buses \n4.Book a bus ticket\n5.Cancel a bus ticket \n0-Log Out\n");
}

// MENU AFTER LOG IN
void menuAferLogIn(){
     int choice;

    do{

        menuForBuses();

        printf("Enter choice\t");
        scanf("%d", &choice);

        switch(choice){
            case 1: // Clearing Screen
           system("cls");
          // Calling heading() function
           heading();
                viewAllPossiblePaths();
            break;

            case 2: // Clearing Screen
           system("cls");
          // Calling heading() function
           heading();
                shortest_cost_from_source_to_des_w_r_t_cost();
            break;
            case 3:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                displayBus();
                break;
            case 4:// Clearing Screen
           system("cls");
          // Calling heading() function
           heading();
           bookSeat();
           break;
           case 5:// Clearing Screen
           system("cls");
          // Calling heading() function
           heading();
           cancelSeat();

            default:// Clearing Screen
           system("cls");
          // Calling heading() function
           heading();
        }
    } while(choice!=0);

    printf("\nYou have logged out successfully!\n");
}


//BELOW CODE REGARDING USER DETAILS

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20


// KMP algorithm for string matching
int kmpSearch(const char *text, const char *pattern) {
    int m = strlen(pattern);
    int n = strlen(text);

    int *lps = (int *)malloc(sizeof(int) * m);
    int j = 0;

    // Build the LPS (Longest Prefix Suffix) array
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[j]) {
            j++;
            lps[i] = j;
            i++;
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    // Perform pattern matching
    i = 0;
    j = 0;
    while (i < n) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }

        if (j == m) {
            free(lps);
            return i - j; // Pattern found at index i-j
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return -1; // Pattern not found
}


// Function to check if a username already exists in the file using KMP
int isUsernameExists(const char *username)
{
    FILE *file = fopen("user_details.txt", "r");
    if (file == NULL) {
        return 0; // File doesn't exist, username doesn't exist
    }

    char buffer[MAX_USERNAME_LENGTH];
    while (fscanf(file, "%s", buffer) != EOF) {
        if (kmpSearch(buffer, username) != -1) {
            fclose(file);
            return 1; // Username exists
        }
    }

    fclose(file);
    return 0; // Username doesn't exist
}


// Function to get a password without displaying it
void getPassword(char *password, int max_length) {
    int i = 0;
    char ch;

    while (1) {
        ch = getch(); // Get a single character without displaying it
        if (ch == 13 || ch == 10) { // Enter key
            password[i] = '\0';
            break;
        } else if (ch == 8 && i > 0) { // Backspace key
            i--;
            printf("\b \b"); // Move the cursor back and erase the character
        } else if (i < max_length - 1) {
            password[i] = ch;
            i++;
            printf("*");
        }
    }
    printf("\n");
}


// Function to register a new user
void registerUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter your username: ");
    scanf("%s", username);



    if (isUsernameExists(username)) {
        printf("Username already exists. Please choose a different username.\n");
        return;
    }

    printf("Enter your password: ");
    //scanf("%s", password);
    getPassword(password, MAX_PASSWORD_LENGTH);
    FILE *file = fopen("user_details.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    printf("Registration successful.\n");
}


// Function to authenticate a user
void loginUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter your username: ");
    scanf("%s", username);


    if (!isUsernameExists(username)) {
        printf("Username does not exist. Please register first.\n");
        return;
    }

    printf("Enter your password: ");
    //scanf("%s", password);
    getPassword(password, MAX_PASSWORD_LENGTH);

    FILE *file = fopen("user_details.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char storedPassword[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH];
    while (fscanf(file, "%s %s", username, storedPassword) != EOF) {
        if (strcmp(storedPassword, password) == 0) {
            fclose(file);
            printf("Login successful. Welcome, %s!\n", username);
            menuAferLogIn();
            return;
        }
    }

    fclose(file);
    printf("Incorrect password. Please try again.\n");
}


//MENU FOR USER LOG IN
void menuForUser(){
    printf("\nBus Booking System\ n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
}


//MENU AFTER LOG IN
void menuBeforeLogIn(){
    int choice;

    do {
        menuForUser();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                registerUser();
                break;
            case 2:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                loginUser();
                break;
            case 3:// Clearing Screen
           system("cls");
           // Calling heading() function
            heading();
                printf("Exiting the program. Goodbye!\n");
                break;
            default:// Clearing Screen
           system("cls");
           // Calling heading() function
            heading();
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 3);
}


#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_MANAGERNAME_LENGTH (MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH)



// Function to check if a manager already exists in the file using strstr
int isManagernameExists(const char *managername) {
    FILE *file = fopen("manager_details.txt", "r");
    if (file == NULL) {
        return 0; // File doesn't exist, managername doesn't exist
    }

    char buffer[MAX_MANAGERNAME_LENGTH];
    while (fscanf(file, "%s", buffer) != EOF) {
        if (strstr(buffer, managername) != NULL) {
            fclose(file);
            return 1; // managername exists
        }
    }

    fclose(file);
    return 0; // managername doesn't exist
}

// Function to register a new manager
void registerManager() {
    char managername[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];  // Fixed variable name

    printf("Enter your name: ");
    scanf("%s", managername);

    if (isManagernameExists(managername)) {
        printf("Managername already exists. Please choose a different managername.\n");
        return;
    }

    printf("Enter your password: ");
    getPassword(password, MAX_PASSWORD_LENGTH);  // Use getPassword function

    FILE *file = fopen("manager_details.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s %s\n", managername, password);
    fclose(file);

    printf("Registration successful.\n");
}

// Function to authenticate a manager
void loginManager() {
    char managername[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter your name: ");
    scanf("%s", managername);

    if (!isManagernameExists(managername)) {
        printf("Manager does not exist. Please register first.\n");
        return;
    }

    printf("Enter your password: ");
    getPassword(password, MAX_PASSWORD_LENGTH);  // Use getPassword function

    FILE *file = fopen("manager_details.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char storedPassword[MAX_PASSWORD_LENGTH];  // Adjusted variable name
    while (fscanf(file, "%s %s", managername, storedPassword) != EOF) {
        if (strcmp(storedPassword, password) == 0) {
            fclose(file);
            printf("Login successful. Welcome, %s!\n", managername);
            return;
        }
    }

    fclose(file);
    printf("Incorrect password. Please try again.\n");
}

void Managermenu() {
    int choice = 0;

    do {
        printf("\nBus Management System\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Add Buses\n");
        printf("4. Alter Buses\n");  // Corrected option number
        printf("5. View Buses\n");  // Corrected option number
        printf("6. EXIT\n");  // Corrected option number
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                registerManager();
                break;
            case 2:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                loginManager();
                break;
            case 3:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                addBus();
                break;
            case 4:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                deleteBus();
                break;
            case 5:// Clearing Screen
           system("cls");
            // Calling heading() function
                heading();
                displayBus();
                break;
            case 6://return;
                printf("Exiting the program. Goodbye!\n");
                //getch();
                //continue;
                break;
            default:
                system("cls");
                heading();
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

    return 0;
}



#include <stdio.h>
#include <stdlib.h> // Include for exit function

int main() {
    int opt = 0;
    assignNullInitially();
    addPlaces();
    formConnectionBetweenPlaces();
    heading();

    while (opt != 3) {
        printf("Login as:\n1. Depot Manager\n2. User\n3. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &opt) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue; // Restart the loop
        }

        switch (opt) {
            case 1://system("cls");
                   //heading();
                Managermenu();
               // system("cls");
                system("cls");
                   heading();
                break;

            case 2://system("cls");
                   //heading();
                menuBeforeLogIn();
                system("cls");
                   heading();
                break;

            case 3://system("cls");
                   //heading();
                printf("Exiting the program.\n");
                system("cls");
                heading();
                exit(0);

            default:
                printf("Invalid choice. Please choose a valid option.\n");
        }
    }

    return 0;
}
