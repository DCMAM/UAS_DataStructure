#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool flag = false;
int total = 0;
int currTemperature = 0;
int minimal = 0;

struct node { 
	int key;
	int height;
	char name[101];
	struct node *left;
	struct node *right;
};

struct node *newNode(int x, char name[]) { 
	struct node *curNewNode = (struct node*)malloc(sizeof(struct node));
	
	curNewNode->key = x;
	strcpy(curNewNode->name, name);
	curNewNode->height = 1;
	curNewNode->left = NULL;
	curNewNode->right = NULL;
	
	total++;
	return curNewNode;
}

int max(int a, int b) { 
	if (a < b) return b;
	else return a;
}

int getHeight(struct node *curr) { 
	if (curr == NULL) return 0;
	return curr->height;
}

int getBF(struct node *curr) {
	if (curr == NULL) return 0;
	return getHeight(curr->left) - getHeight(curr->right);
}

struct node *rightRotate(struct node *y) {
	struct node *x = y->left;
	struct node *B = x->right;
	
	x->right = y;
	y->left = B;	

	y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	
	return x;
}

struct node *leftRotate(struct node *x) {
	struct node *y = x->right;
	struct node *B = y->left;
	
	y->left = x;
	x->right = B;
	
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
	
	return y;
}


struct node *insert(struct node *curr, int value, char name[]) {
	if (curr == NULL) return newNode(value, name);
	else if (value == curr->key){
		flag = true;
		return curr;
	}
	else if (value < curr->key) {
		curr->left = insert(curr->left, value, name);
	} else if(value > curr->key) {
		curr->right = insert(curr->right, value, name);
	}
	
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) + 1;
	int bfactor = getBF(curr);
	
	if (bfactor > 1 && value < curr->left->key) { 
		return rightRotate(curr);
	}
	if (bfactor < -1 && value > curr->right->key) {
		return leftRotate(curr);
	}
	if (bfactor > 1 && value > curr->left->key) { 
		curr->left = leftRotate(curr->left);
		return rightRotate(curr);
	}
	if (bfactor < -1 && value < curr->right->key) { 
		curr->right = rightRotate(curr->right);
		return leftRotate(curr);
	}
	return curr;
}

struct node *predecessor(struct node *root) {
	
	struct node *cur = root->left;
	while(cur->right != NULL) {
		cur = cur->right;
	}
	return cur;
}

struct node *deleteValue(struct node *curr, int value) {
	if (curr == NULL){
		flag = true;
		return NULL;
	}
	if (value < curr->key) {
		curr->left = deleteValue(curr->left, value);
	}
	else if (value > curr->key) {
		curr->right = deleteValue(curr->right, value);
	} 
	else {

		if ((curr->left == NULL) || (curr->right == NULL)) {
		  
		  	struct node *temp = NULL;
		  	if (curr->left != NULL) temp = curr->left;
		 	else temp = curr->right;
		  
			if (temp == NULL) {
			    temp = curr;
				curr = NULL;
			} 
			else { 
		  		*curr = *temp;
		 	}
			free(temp);
		} 
		else {
		  
		 	struct node *temp = predecessor(curr);
		 	curr->key = temp->key;
		 	strcpy(curr->name, temp->name);
		  

		 	curr->left = deleteValue(curr->left, temp->key);
		 	
		}
	}
	
	if (curr == NULL) return curr;
	
	curr->height = max(getHeight(curr->left), getHeight(curr->right)) +1 ;
	int bfactor = getBF(curr);
	

	if (bfactor > 1 && getBF(curr->left) >= 0) {
		return rightRotate(curr);
	}
	if (bfactor > 1 && getBF(curr->left) < 0) {
		curr->left = leftRotate(curr->left);
		return rightRotate(curr);
	}
	if (bfactor < -1 && getBF(curr->right) <= 0) {
		return leftRotate(curr);
	}
	if (bfactor < -1 && getBF(curr->right) > 0) {
		curr->right = rightRotate(curr->right);
		return leftRotate(curr);
	}
	return curr;
}

int minimalTemp = 0;
char minimalPlanet[31];

struct node *findValue(struct node *curr, int value) {
	if (curr == NULL) {
//		printf("Number is not found..\n");
		flag = true;
		return curr;
	}
	else if (value > curr->key) { 
		if(minimal >  value - curr->key){
			minimal = value - curr->key;
			minimalTemp = curr->key;
			strcpy(minimalPlanet, curr->name);
		}
		else if(minimal == value - curr->key){
			minimal = value - curr->key;
			minimalTemp = curr->key;
			strcpy(minimalPlanet, curr->name);
		}
		
		curr->right = findValue(curr->right, value);
	} 
	if (value < curr->key) { 
		if(minimal > curr->key - value){
			minimal = curr->key - value;
			minimalTemp = curr->key;
			strcpy(minimalPlanet, curr->name);
		}
		else if(minimal == curr->key - value){
			minimal = curr->key - value;
			minimalTemp = curr->key;
			strcpy(minimalPlanet, curr->name);
		}
		
		curr->left = findValue(curr->left, value);
	}
	else {
		flag = false;
		printf("%d %s\n", curr->key, curr->name);
	}
	
	if (curr == NULL) return curr;
	
	return curr;
}

int rank = 0;

void printInorder(struct node *root, int number1, int number2, int step){
	if(root == NULL) return;
	printInorder(root->left, number1, number2, step++);
	if(step >= number1 || step <= number2){
		printf("%d: %d %s\n", rank+1, root->key, root->name);
		rank++;
	}
	printInorder(root->right, number1, number2, step++);
}

int larger = 0;

void printInorderMenu1(struct node *root){
	if(root == NULL){
		return;
	} 
	printInorderMenu1(root->left);
	if(root->key > currTemperature){
		larger++;
	}
	printInorderMenu1(root->right);
}

struct node *freeAll(struct node *root) { 
	if (root == NULL) return NULL;
	root->left = freeAll(root->left);
	root->right = freeAll(root->right);
	free(root);
	return NULL;
}

int main() {
	struct node *root = NULL;
	
	int menu = 0;
	int t = 0;
	scanf("%d", &t);
	for(int j=0; j<t; j++){
		flag = false;
		scanf("%d", &menu);
		
		if(menu == 1){
			int number = 0;
			char name[31];
			larger = 0;
			
			scanf("%d", &number);
			
			do{
				scanf("%s", &name);
			}while(strlen(name) > 30);
			int i = 0;
			while(name[i] != '\0'){
				if(name[i] < 97){
					name[i] = name[i] + 97 - 65;
				}
				i++;
			}
			
			root = insert(root, number, name);
			if(flag == false) currTemperature = number;
			
			printInorderMenu1(root);
			printf("%d\n", larger);
		}
		else if(menu == 2){		
			int number = 0;	
			scanf("%d", &number);
			root = deleteValue(root, number);
			total--;
			printf("%d\n", total);
		}
		else if(menu == 3){
			int number = 0;
			minimal = 2000000000;
			
			scanf("%d", &number);
			root = findValue(root, number);
			if(flag == true) printf("%d %s\n", minimalTemp, minimalPlanet);
		}
		else if(menu == 4){
			int number1 = 0;
			int number2 = 0;
			rank = 0;
			
			scanf("%d", &number1);
			do{
				scanf("%d", &number2);	
			}while(number2 <= number1);
			printInorder(root, number1, number2, 1);
		}
	}
	root = freeAll(root);
	return 0;
}
