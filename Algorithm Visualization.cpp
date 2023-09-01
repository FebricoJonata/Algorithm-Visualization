#include <bits/stdc++.h>
#include <windows.h>
#include <time.h>
#include <vector>
#include <conio.h>

#define HEIGHT 25
#define WIDTH 109

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

char Map[20][100];
int arr[100] = {0};
int check[100] = {0};

using namespace std;

struct Graph{
	int x, y;
	char wall;
	
}MAZE[HEIGHT+1][WIDTH+1];

struct Edge{
	Graph source;
	Graph destination;
};

struct Vertex{
	int x, y;
	int xPrev, yPrev;
	Vertex(int x, int y){
		this->x = x;
		this->y = y;
	}
	
	Vertex(int x, int y, int xPrev, int yPrev){
		
	}
};

int dirX[] = {0, 2, 0, -2};
int dirY[] = {-2, 0, 2, 0};

char Maze[HEIGHT+1][WIDTH+1];

Edge createEdge(Graph source, Graph destination){
	Edge e;
	e.source = source;
	e.destination = destination;
	
	return e;
}

void setPositionDefault(){
	COORD cur;
	cur.X = 0;
	cur.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

void setPosition(int x, int y){
	COORD cur;
	cur.X = y+2;
	cur.Y = x+1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

void printMap(int y, char *color){
	for(int i = 0; i < 100; i++){
        for(int j = 0; j < 20; j++){
            if((5 * j) < arr[i]){
            	Map[19 - j][i] = 219;
			} else{
				Map[19 - j][i] = ' ';
			}
        }
    }
	
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 20; j++){
			setPosition(j, i);
			
			if(strcmp(color, "Red") == 0 && i == y){
				printf("\e[0;31m");
			}
			
			if(strcmp(color, "Green") == 0){
				printf("\e[0;32m");
			}
			
			if(strcmp(color, "Blue") == 0){
				printf("\e[0;36m");
			}
			
			if(check[i] == 1){
				printf("\e[0;34m");
			}
			
			printf("%c", Map[j][i]);
			printf("\e[0;37m");
		}
		
		printf("\n");	
	}
	
	Sleep(0.1);
}

void swap(int *a,int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int low, int high){
	int i = low - 1;
	int pivot = arr[high];
	int curr = low;
	
	printMap(curr, "Red");
	
	for(int curr = low; curr < high; curr++){
		if(arr[curr] < pivot){
			printMap(curr, "Red");
			i++;
			swap(&arr[curr], &arr[i]);
		}
	}
	
	i++;
	swap(&arr[i], &arr[high]);
	printMap(high, "Default");
	check[high] = 1;
	return i;
}

void initMaze(){
	for(int i=0; i < HEIGHT; i++){
		for(int j=0; j<WIDTH; j++){
			MAZE[i][j].x = i;
			MAZE[i][j].y = j;
			MAZE[i][j].wall = 219;
		}
	}
}

void initPrimsMaze(){
	for(int i = 0 ; i < HEIGHT ; i++){
		for(int j = 0 ; j < WIDTH ; j++){
			Maze[i][j] = 219;
		}
	}
}

void primsMaze(){
	for(int i = 0 ; i < HEIGHT ; i++){
		for(int j = 0 ; j < WIDTH ; j++){
			if(i == 0 || i == HEIGHT-1 || j == 0 || j == WIDTH-1){
				Maze[i][j] = 219;
			}
		}
	}
}

void printPrimsMap(){
	for(int i = 0 ; i < HEIGHT ; i++){
		for(int j = 0 ; j < WIDTH ; j++){
			printf("%c", Maze[i][j]);
		}
		printf("\n");
	}
}

void printMaze(){
	for(int i=0; i < HEIGHT; i++){
		for(int j=0; j<WIDTH; j++){
			printf("%c", MAZE[i][j].wall);
		}
		printf("\n");
	}
}

Graph findParent(Graph parent[HEIGHT][WIDTH], Graph position){
	if(parent[position.x][position.y].x == position.x && parent[position.x][position.y].y == position.y) return position;
	
	return findParent(parent, parent[position.x][position.y]);
}

bool isCycle(Graph parent[HEIGHT][WIDTH], Graph destination, Graph source){
	Graph destinationParent = findParent(parent, destination);
	Graph sourceParent = findParent(parent, source);
	
	if(destinationParent.x = sourceParent.x && destinationParent.y == sourceParent.y)return true;
	
	return false;
}

void kruskalMaze(){

	Graph parent[HEIGHT][WIDTH];
	vector<Edge> edges;
	
	for(int i=1; i<HEIGHT-2; i+=2){
		for(int j = 1; j<WIDTH-2; j+=2){
			edges.push_back(createEdge(MAZE[i][j], MAZE[i][j+2]));
		}
	}
	
	for(int i=0; i < HEIGHT; i++){
		for(int j=0; j < WIDTH; j++){
			parent[i][j] = MAZE[i][j];
		}
	}
	
	for(int i=1; i<HEIGHT-2; i+=2){
		for(int j=1; j<WIDTH-2; j+=2){
			edges.push_back(createEdge(MAZE[i][j], MAZE[i+2][j]));
		}
	}
	
	
	while(edges.size()){
		int random = rand() % edges.size();
		
		Edge curr = edges[random];
		edges.erase(edges.begin() + random);
		
		
		Graph parentSource = findParent(parent, curr.source);
		Graph parentDestination = findParent(parent, curr.destination);
		
		if(!isCycle(parent, curr.source, curr.destination)){
			parent[parentSource.x][parentSource.y] = parentDestination;
			
			MAZE[curr.source.x + (curr.destination.x - curr.source.x)/2][curr.source.y + (curr.destination.y - curr.source.y)/2].wall = ' ';
			MAZE[curr.destination.x][curr.destination.y].wall = ' ';
			MAZE[curr.source.x][curr.source.y].wall = ' ';
		}
		
		setPositionDefault();
		printMaze();
		printf("\n");
	}
	
}

void prims(){
	vector<Vertex> vertices;
	
	bool visited[HEIGHT][WIDTH] = {false};
	vertices.push_back(Vertex(1, 1));
	
	bool flag = true;
	int neighbour;
	bool check;
	
	while(!vertices.empty()){
		Vertex curr = vertices.at(0);
		vertices.erase(vertices.begin());
		
		if(!visited[curr.y][curr.x]){

			visited[curr.y][curr.x] = true;
			Maze[curr.y][curr.x] = ' ';
			

			if(flag){
				flag = false;
			}else{
				do{
					neighbour = rand() % 5;
					check = curr.x + dirX[neighbour] >= 0 && curr.x + dirX[neighbour] < WIDTH && curr.y + dirY[neighbour] >= 0 && curr.y + dirY[neighbour] < HEIGHT && Maze[curr.y + dirY[neighbour]][curr.x + dirX[neighbour]] == ' ';
				}while(!check);
				Maze[curr.y + dirY[neighbour] / 2][curr.x + dirX[neighbour] / 2] = ' ';
			}
			
			for(int i = 0 ; i < 4 ; i++){
				if(curr.x + dirX[i] >= 0 && curr.x + dirX[i] < WIDTH && curr.y + dirY[i] >= 0 && curr.y + dirY[i] < HEIGHT){
					vertices.push_back(Vertex(curr.x + dirX[i], curr.y + dirY[i]));
				}
			}

		}
		setPositionDefault();
		printPrimsMap();
	}
}

void quickSort(int low, int high){
	if(high <= low){
		check[high] = check[low] = 1;
		return;
	}
	
	int pi = partition(low, high);
	check[pi] = 1;
	quickSort(low, pi - 1);
	quickSort(pi + 1, high);	
}

void merge(int arr[],int start,int mid ,int end){
    int left = mid-start+1;
    int right = end-mid;

    int arrL[left],arrR[right];

    for (int i = 0; i < left; i++){
        arrL[i] = arr[start+i];
    }
    for (int i = 0; i < right; i++){
        arrR[i] = arr[i+mid+1];
    }
    int leftCur = 0;
    int rightCur = 0;
    int mergeCur = start;
    while(leftCur<left && rightCur<right){
        if (arrL[leftCur]<arrR[rightCur]){
            arr[mergeCur] = arrL[leftCur];
            leftCur++;
        }else{
            arr[mergeCur] = arrR[rightCur];
            rightCur++;
        }
        mergeCur++;
         
    }while (leftCur<left ){
        arr[mergeCur] = arrL[leftCur];
        leftCur++;
        mergeCur++;
    }
    
    while (rightCur<right ){
        arr[mergeCur] = arrR[rightCur];
        rightCur++;
        mergeCur++;
    }
    
}

void mergesort(int arr[],int start,int end){
    if (end<= start) return;
    int mid = (start+end)/2;
    mergesort(arr,start,mid);
    mergesort(arr,mid+1,end);
    merge(arr,start,mid,end);
   	printMap(mid,"Blue");
}

void heapify(int arr[], int n, int i) {
    int largest = i; 
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
    printMap(i, "Red");
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--){
    	heapify(arr, n, i);
    	
	}
    
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
    
}

char frame[23][120]{
	"########################################################################################################",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"#                                                                                                      #",
	"########################################################################################################"
	};

void printFrame(){
	
//	for (int i = 0; i < 24; i++) {
//        for (int j = 0; j < 111; j++) {
//        	if(i == 0 || i == 23 || j == 0 || j == 110){
//        		Map[i][j] = 219;
//			}else{
//				Map[i][j] = ' ';
//			}
//            
//        }
//    }
//    
//    for(int i = 0 ; i < 24 ; i++){
//		for(int j = 0 ; j < 111 ; j++){
//			printf("%c", Map[i][j]);
//		}
//		printf("\n");
//	}

	for(int i=0; i<24; i++){
		for(int j=0; j<111; j++){
			printf("%c", frame[i][j]);
		}
		printf("\n");
	}
}

void initBar(){
	printFrame();
    for (int i = 0; i < 100; i++) {
	    arr[i] = rand() % 96+5;
	    
    }
    printMap(0, "Blue");
    printMap(0, "Green");
    printMap(0, "Default");
}

void menuSort(){
	int option;	
	do{
		initBar();
		printf("\n1. Quick Sort\n");
		printf("2. Merge Sort\n");
		printf("3. Heap Sort\n");
		printf("4. Exit\n");
		printf(">> ");
		scanf("%d",&option);getchar();
			    
		if(option == 1){
			memset(check,0,120);
			quickSort(0,99);
			printMap(-1,"Green");
			printf("Quick sort complete\n");
		}else if(option == 2){
			memset(check,0,120);
			mergesort(arr,0,99);
			printMap(-1,"Green");
			printf("Merge sort complete\n");
		}else if(option == 3){
			heapSort(arr, 99);
			printMap(-1,"Green");
		}else if(option == 4){
			return;
		}
		system("cls");
	}while(option != 4);
}



void menuMaze(){
	int option;
	do{
		system("cls");
		printf("1. Kruskal\n");
		printf("2. Prim's\n");
		printf("3. Exit\n");
		printf(">> ");
		scanf("%d", &option);
		
		if(option == 1){
			initMaze();
			kruskalMaze();
		}else if(option == 2){
			initPrimsMaze();
			primsMaze();
			prims();
		}else if(option == 3){
			return;
		}
	}while(option!=3);
}

void arrow_key_menu() {
   int option = 1;
   int key;
   
   do{
   	
   	  system("cls");
   	  puts("");
   	  puts("");
      printf("%-50sAlgorithm Visualization\n", "\x1b[m");
      printf("\n\n");
      printf("%-53sSorting Vizualization\n", (option == 1)? "\033[31m" : "\x1b[m");
      printf("%-54sMaze Generation\n", (option == 2)? "\033[31m" : "\x1b[m");
      printf("%-59sExit\n", (option == 3)? "\033[31m" : "\x1b[m");
      puts("");
      puts("");
      puts("");
      puts("");
      printf("%s\tArrow Key [down | up] to move\t\t\t\t\t\t\t\tEnter to choose\n", "\x1b[m");
      key = getch();
      

      if (key == 224) {
         key = getch();

         switch (key) {
            case UP_ARROW:
               if (option > 1) {
                  option--; 
               }
               break;
            case DOWN_ARROW:
               if (option < 3) {
                  option++;
               }
               break;
         }
      }
      
      if(option == 1 && key == 13){
      	system("cls");
      	menuSort();	
      	getch();
	  }else if(option == 2 && key == 13){
	  	system("cls");
	  	menuMaze();
	  	puts("");
	  	printf("\t\t\t\t\t\%sPress Enter To Continue....","\x1b[m"); getch();	
	  }else if (key == 13 && option == 3) {
	  	break;
      }
      
   }while(1);
   system("cls");
   printf("%s\nPress Enter To Continue....","\x1b[m"); getch();
}

int main(){
	srand(time(NULL));
	int option;
	arrow_key_menu();
}
