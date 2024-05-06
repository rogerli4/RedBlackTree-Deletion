#include <iostream>
#include <cstring>
#include <fstream>
#include "Node.h"

using namespace std;
void add(Node* &treeRoot, Node* current, Node* newNode);
void fixTree(Node* &treeRoot, Node* newNode);
void print(Node* treeRoot, int level);
void case3(Node* &treeRoot, Node* newNode);
void case4(Node* &treeRoot, Node* &newNode);
void case5(Node* &treeRoot, Node* newNode);
void search(Node* treeRoot, int value, bool &contained);
void remove(Node* &treeRoot, Node* current, Node* parent, int direction, int value);
int correctInput();


int main() {
  Node* treeRoot = NULL;
  Node* current;
  Node* newNode;
  int level = 0;
  
  bool looping = true;
  while (looping == true) {
    int option = correctInput();
    // Add numbers to tree
    if (option == 1) {
      cout << "FILE or CONSOLE?" << endl;
      char input[15];
      cin.get(input, 15);
      cin.ignore(15, '\n');
      bool looping2 = true;
      while (looping2 == true) {
	// Add numbers through a file
	if (strcmp(input, "FILE") == 0) {
	  int input[100];
	  int totalNums;
	  cout << "How many numbers are you adding?" << endl;
	  cin >> totalNums;
	  cin.get();
	  // File input
	  ifstream numbers("testNumbers.txt");
	  for (int i = 0; i < totalNums; i++) {
	    numbers >> input[i];
	    Node* newNode = new Node(input[i]);
	    add(treeRoot, treeRoot, newNode);
	    fixTree(treeRoot, newNode);
	  }
	  looping2 = false;
	}
	// Add numbers through the console
	else if (strcmp(input, "CONSOLE") == 0) {
	  int input[100];
	  cout << "How many numbers would you like to add?" << endl;
	  int totalNums;
	  cin >> totalNums;
	  cin.get();
	  // Console input
	  cout << "Please enter your numbers." << endl;
	  for (int i = 0; i < totalNums; i++) {
	    cin >> input[i];
	    cin.get();
	    Node* newNode = new Node(input[i]);
	    add(treeRoot, treeRoot, newNode);
	    fixTree(treeRoot, newNode);
	  }
	  looping2 = false;
	}
	else {
	  cout << "Please input one of the two options" << endl;
	}
      }
    }
    // Print tree
    else if (option == 2) {
      print(treeRoot, level);
    }
    // Search Tree
    else if (option == 3) {
      cout << "What number would you like to search for?" << endl;
      int value;
      cin >> value;
      cin.get();
      bool contained = false;
      search(treeRoot, value, contained);
      if (contained == true) {
	cout << endl << "The inputted value is contained within the tree." << endl << endl;
      }
      else if (contained == false) {
	cout << endl << "The inputted value is NOT contained within the tree." << endl << endl;
      }
    }
    // Delete node
    else if (option == 4) {
      cout << "What number would you like to delete?" << endl;
      int value;
      cin >> value;
      cin.get();
      int direction;
      remove(treeRoot, treeRoot, treeRoot, direction, value);
    }
    // Quit the program
    else if (option == 5) {
      looping = false;
    }
  }
}

int correctInput() {
  bool leaveLoop = false;
  cout << "Enter one of the following options: ADD, PRINT, SEARCH, DELETE, or QUIT (uppercase)" << endl;
  while (leaveLoop == false) {
    char input[15];
    cin.get(input, 15);
    cin.ignore(15, '\n');
    if (strcmp(input, "ADD") == 0) {
      return 1;
    }
    else if (strcmp(input, "PRINT") == 0) {
      return 2;
    }
    else if (strcmp(input, "SEARCH") == 0) {
      return 3;
    }
    else if (strcmp(input, "DELETE") == 0) {
      return 4;
    }
    else if (strcmp(input, "QUIT") == 0) {
      return 5;
    }
    else {
      cout << "Please enter a valid option." << endl;
    }
  }
  return -1;
}

void add(Node* &treeRoot, Node* current, Node* newNode) {
  if (newNode->getParent() == NULL) {
    if (treeRoot == NULL) {
      treeRoot = newNode;
      newNode->setParent(treeRoot);
    }
    // Add to the left
    else if (current->getNum() > newNode->getNum()) {
      if (current->getLeft() == NULL) {
	current->setLeft(newNode);
	newNode->setParent(current);
      }
      else if (current->getLeft() != NULL) {
	add(treeRoot, current->getLeft(), newNode);
      }
    }
    // Add to the right
    else if ((current->getNum() < newNode->getNum()) || (current->getNum() == newNode->getNum())) {
      if (current->getRight() == NULL) {
	current->setRight(newNode);
	newNode->setParent(current);
      }
      else if (current->getRight() != NULL) {
	add(treeRoot, current->getRight(), newNode);
      }
    }
  }
}

void fixTree(Node* &treeRoot, Node* newNode) {
  // Case 1: Insert at root
  if (treeRoot == newNode) {
    treeRoot->setColor('B');
  }
  else if (newNode->getParent()->getParent() != NULL) {
    // Case 3: Parent and uncle are red
    if (newNode->getUncle() != NULL && (newNode->getParent()->getColor() == 'R' && newNode->getUncle()->getColor() == 'R')) {
      case3(treeRoot, newNode);
    }
    // Case 4 & 5: Uncle is black, triangle case and line case
    // Triangle case
    else if ((newNode->getUncle() == NULL || newNode->getUncle()->getColor() == 'B') && (newNode->getParent()->getColor() == 'R')) {
      Node* temp = newNode;
      // Triangle case
      case4(treeRoot, newNode);
      // Line case
      if (temp == newNode) {
	case5(treeRoot, newNode);
      }
    }
  }
}

void print(Node* treeRoot, int level) {
  if (treeRoot == NULL) {
    cout << "The tree is empty!" << endl << endl;
  }
  // Go through the right side of tree, finding where the tree stops, and indenting appropriately
  else {
    if (treeRoot->getRight() != NULL) {
      print(treeRoot->getRight(), level + 1);
    }
    // Indent values
    for (int i = 0; i < level; i++) {
      cout << '\t';
    }
    // Left side
    cout << treeRoot->getColor() << treeRoot->getNum() << endl;
    if (treeRoot->getLeft() != NULL) {
      print(treeRoot->getLeft(), level + 1);
    }
  }
}

void case3(Node* &treeRoot, Node* newNode) {
  // Change colors
  newNode->getParent()->setColor('B');
  newNode->getUncle()->setColor('B');
  newNode->getParent()->getParent()->setColor('R');
  // Recall on grandparent
  fixTree(treeRoot, newNode->getParent()->getParent());
}

void case4(Node* &treeRoot, Node* &newNode) {
  // Parent is left and node is right
  if (newNode->getParent()->getParent()->getLeft() == newNode->getParent() && newNode->getParent()->getRight() == newNode) {
    // Change parents and children
    Node* tempParent = newNode->getParent();
    Node* tempLeft = newNode->getLeft();
    newNode->getParent()->getParent()->setLeft(newNode);
    newNode->setParent(newNode->getParent()->getParent());
    newNode->setLeft(tempParent);
    newNode->getLeft()->setParent(newNode);
    // Set right node if it exists
    if (tempLeft != NULL) {
      tempParent->setRight(tempLeft);
      tempParent->getRight()->setParent(newNode->getLeft());
    }
    else {
      tempParent->setRight(NULL);
    }
    newNode = newNode->getLeft();
    case5(treeRoot, newNode);
  }
  //Parent is right and node is left
  else if (newNode->getParent()->getParent()->getRight() == newNode->getParent() && newNode->getParent()->getLeft() == newNode) {
    // Change parents and children
    Node* tempParent = newNode->getParent();
    Node* tempRight = newNode->getRight();
    newNode->getParent()->getParent()->setRight(newNode);
    newNode->setParent(newNode->getParent()->getParent());
    newNode->setRight(tempParent);
    newNode->getRight()->setParent(newNode);
    // Set left node if it exists
    if (tempRight != NULL) {
      tempParent->setLeft(tempRight);
      tempParent->getLeft()->setParent(newNode->getRight());
    }
    else {
      tempParent->setLeft(NULL);
    }
    newNode = newNode->getRight();
    case5(treeRoot, newNode);
  }
}

void case5(Node* &treeRoot, Node* newNode) {
  // Parent is left and node is left
  if (newNode->getParent()->getParent()->getLeft() == newNode->getParent() && newNode->getParent()->getLeft() == newNode) {
    // Tree rotation
    Node* tempGrandParent = newNode->getParent()->getParent();
    Node* tempRight = newNode->getParent()->getRight();
    // Treeroot case
    if (tempGrandParent == treeRoot) {
      // Change parents and children
      treeRoot = newNode->getParent();
      treeRoot->setParent(NULL);
      treeRoot->setRight(tempGrandParent);
      tempGrandParent->setParent(treeRoot);
      tempGrandParent->setLeft(tempRight);
      // If parent has a right node. set it as grandparent's child
      if (tempRight != NULL) {
	tempRight->setParent(tempGrandParent);
      }
      // Change colors
      tempGrandParent->setColor('R');
      treeRoot->setColor('B');
    }
    else {
      // Grandparent is a right child
      if (tempGrandParent->getParent()->getRight() == newNode->getParent()->getParent()) {
	tempGrandParent->getParent()->setRight(newNode->getParent());
	newNode->getParent()->setParent(tempGrandParent->getParent());
      }
      // Grandparent is a left child
      else if (tempGrandParent->getParent()->getLeft() == newNode->getParent()->getParent()) {
	tempGrandParent->getParent()->setLeft(newNode->getParent());
	newNode->getParent()->setParent(tempGrandParent->getParent());
      }
      // Change parents and children
      newNode->getParent()->setRight(tempGrandParent);
      tempGrandParent->setParent(newNode->getParent());
      tempGrandParent->setLeft(tempRight);
      if (tempRight != NULL) {
	tempRight->setParent(tempGrandParent);
      }
      // Change colors
      tempGrandParent->setColor('R');
      newNode->getParent()->setColor('B');
    }
  }
  // Parent is right and node is right
  else if (newNode->getParent()->getParent()->getRight() == newNode->getParent() && newNode->getParent()->getRight() == newNode) {
    // Tree Rotation
    Node* tempGrandParent = newNode->getParent()->getParent();
    Node* tempLeft = newNode->getParent()->getLeft();
    // Treeroot case
    if (tempGrandParent == treeRoot) {
      // Change parents and children
      treeRoot = newNode->getParent();
      treeRoot->setParent(NULL);
      treeRoot->setLeft(tempGrandParent);
      tempGrandParent->setParent(treeRoot);
      tempGrandParent->setRight(tempLeft);
      // If parent has a left node. set it as grandparent's child
      if (tempLeft != NULL) {
	tempLeft->setParent(tempGrandParent);
      }
      // Change colors
      tempGrandParent->setColor('R');
      treeRoot->setColor('B');
    }
    else {
      // Grandparent is left child
      if (tempGrandParent->getParent()->getLeft() == newNode->getParent()->getParent()) {
	tempGrandParent->getParent()->setLeft(newNode->getParent());
	newNode->getParent()->setParent(tempGrandParent->getParent());
      }
      // Grandparent is right child
      else if (tempGrandParent->getParent()->getRight() == newNode->getParent()->getParent()) {
	tempGrandParent->getParent()->setRight(newNode->getParent());
	newNode->getParent()->setParent(tempGrandParent->getParent());
      }
      // Change parents and children
      newNode->getParent()->setLeft(tempGrandParent);
      tempGrandParent->setParent(newNode->getParent());
      tempGrandParent->setRight(tempLeft);
      if (tempLeft != NULL) {
	tempLeft->setParent(tempGrandParent);
      }
      // Change colors
      tempGrandParent->setColor('R');
      newNode->getParent()->setColor('B');
    }    
  }
}

void search(Node* treeRoot, int value, bool &contained) {
  if (treeRoot == NULL) {
    cout << "The tree is empty!" << endl << endl;
  }
  else {
    if (treeRoot->getNum() == value) {
      contained = true;
    }
    // Search through the right branches
    if (treeRoot->getRight() != NULL) {
      search(treeRoot->getRight(), value, contained);
    }
    // Search through the left branches
    if (treeRoot->getLeft() != NULL) {
      search(treeRoot->getLeft(), value, contained);
    }
  }
}

void remove(Node* &treeRoot, Node* current, Node* parent, int direction, int value) {
  if (treeRoot == NULL) {
    cout << "The tree is empty!" << endl << endl;
    return;
  }
  else {
    if (current->getNum() == value) {
      // Case where node has no children
      if (current->getRight() == NULL && current->getLeft() == NULL) {	
	if (current == treeRoot) {
	  treeRoot = NULL;
	}
	// Set parent left/right to null
	else {
	  if (direction == 1) {
	    parent->setRight(NULL);
	  }
	  else if (direction == 0) {
	    parent->setLeft(NULL);
	  }
	  current = NULL;
	}
      }
      // Case where node has two children
      else if (current->getRight() != NULL && current->getLeft() != NULL) {
	// Find the value to replace node
	Node* replacement = current->getRight();
	Node* replacementParent = current;
	// If node's right child has left children, find leftmost child and fix tree
	while (replacement->getLeft() != NULL) {
	  replacementParent = replacement;
	  replacement = replacement->getLeft();
	}
	if (current != replacementParent) {
	  current->setNum(replacement->getNum());
	  if (replacement->getRight() != NULL) {
	    replacementParent->setLeft(replacement->getRight());
	  }
	  else {
	    replacementParent->setLeft(NULL);
	  }
	}
	else {
	  current->setNum(replacement->getNum());
	  current->setRight(current->getRight()->getRight());
	}
      }
      // Case where node has one child
      else if (current->getRight() != NULL || current->getLeft() != NULL) {
	// If child is to the right
	if (current->getRight() != NULL) {
	  if (current == treeRoot) {
	    treeRoot = current->getRight();
	  }
	  else {
	    // Set parent's left/right
	    if (direction == 1) {
	      parent->setRight(current->getRight());
	    }
	    else if (direction == 0) {
	      parent->setLeft(current->getRight());
	    }
	  }
	}
	// If child is to the left
	else if (current->getLeft() != NULL) {
	  if (current == treeRoot) {
	    treeRoot = current->getLeft();
	  }
	  else {
	    // Set parent's left/right
	    if (direction == 1) {
	      parent->setRight(current->getLeft());
	    }
	    else if (direction == 0) {
	      parent->setLeft(current->getLeft());
	    }
	  }
	}
      }
    }
    else {
      // Recurse
      if (current->getNum() < value) {
	// Recursion to the right
	if (current->getRight() != NULL) {
	  direction = 1;
	  remove(treeRoot, current->getRight(), current, direction, value);
	}
	else {
	  cout << endl << "This number is not contained within the tree!" << endl << endl;
	}
      }
      // Recursion to the left
      else if (current->getNum() > value) {
	if (current->getLeft() != NULL) {
	  direction = 0;
	  remove(treeRoot, current->getLeft(), current, direction, value);
	}
	else {
	  cout << endl << "This number is not contained within the tree" << endl << endl;
	}
      }
    }
  }
}
