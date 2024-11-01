#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tree node structure with extra data components for 2D packing
typedef struct treeNode{
    int leaf; // Whether a node is a leaf (1) or not (0)
    int label; // Block's label (leaf nodes)
    int width; // Width dimension
    int height; // Height dimension
    char cut; // 'H' for horizontal cut, 'V' for vertical cut (internal nodes)
    int x; // X-coordinate of bottom left corner
    int y; // Y-coordinate of bottom left corner
    treeNode* left; // Left child
    treeNode* right; // Right child
}treeNode;

// Creates new leaf node
treeNode* createLeaf(int label, int width, int height){

}

// Creates new internal node
treeNode* createInternal(char cut, treeNode* left, treeNode* right){

}

// Build binary tree from a post-order input file
treeNode* buildTree(FILE* infile){
    char line[100];
    treeNode* stack[500]; // Stack to help build the tree
}

// Pre-order traversal to write to the first output file
void preOrder(treeNode* root, FILE* outfile){

}

// Post-order traversal to compute and write dimensions
void postOrderDimensions(treeNode* root, FILE* outfile){

}

// Post-order traversal to compute and write coordinates to third output file
void postOrderCoordinates(treeNode* root, int x, int y, FILE* outfile){

}

// Free all allocated memory in the tree
void freeTree(treeNode* root){

}

// Main function
int main(int argc, char* argv[]){

}