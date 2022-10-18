#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getHeight(Node * root)
{
    //base case
    if(root == NULL){
        return 0;
    }
    int leftHeight = 1 + getHeight(root->left);
    int rightHeight = 1 + getHeight(root->right);
    if(leftHeight > rightHeight){
        return leftHeight;
    }
    else{
        return rightHeight;
    }
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL){
        return true;
    }
    
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    int difference; 
    if(leftHeight>rightHeight){
        difference = leftHeight - rightHeight;
    }
    else{
        difference = rightHeight - leftHeight;
    }
    if((root->left == NULL || root->right == NULL) && difference < 2){
        return true;
    }
    if(leftHeight != rightHeight){
        return false;
    }
    else{
        bool leftEqual = equalPaths(root->left);
        bool rightEqual = equalPaths(root->right);
        return leftEqual && rightEqual;
    }
}

