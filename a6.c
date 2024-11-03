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
    treeNode* new = (treeNode*)malloc(sizeof(treeNode));
    new->leaf = 1;
    new->label = label;
    new->width = width;
    new->height = height;
    new->cut = '\0';
    new->x = 0;
    new->y = 0;
    new->left = NULL;
    new->right = NULL;
    return new;
}

// Creates new internal node
treeNode* createInternal(char cut, treeNode* left, treeNode* right){
    treeNode* new = (treeNode*)malloc(sizeof(treeNode));
    new->leaf = 0;
    new->label = -1;
    new->cut = cut;
    new->left = left;
    new->right = right;
    // Compute dimensions of enclosing room based on cut types
    if(cut == 'H'){
        new->width = left->width > right->width ? left->width : left->right;
        new->height = left->height + right->height;
    }
    else if(cut == 'V'){
        new->width = left->width + right->width;
        new->height = left->height > right->height ? left->height : right->height;
    }
    new->x = 0;
    new->y = 0;
    return new;
}

// Build binary tree from a post-order input file
treeNode* buildTree(FILE* infile){
    char line[100];
    treeNode* stack[500]; // Stack to help build the tree
    int top = -1;

    while(fgets(line, sizeof(line), infile)){
        if(line[0] == 'H' || line[0] == 'V'){
            // Internal node
            treeNode* right = stack[top--];
            treeNode* left = stack[top--];
            stack[++top] = createInternal(line[0], left, right);
        }
        else{
            // Leaf node
            int label;
            int width;
            int height;
            sscanf(line, "%d(%d,%d)", &label, &width, &height);
            stack[++top] = createLeaf(label, width, height);
        }
    }
    // The root of the tree is the last element in the stack
    return stack[top];
}

// Pre-order traversal to write to the first output file
void preOrder(treeNode* root, FILE* outfile){
    if(root == NULL) return;
    if(root->leaf){
        fprintf(outfile, "%d(%d,%d)\n", root->label, root->width, root->height);
    }
    else{
        fprintf(outfile, "%c\n", root->cut);
    }
    preOrder(root->left, outfile);
    preOrder(root->right, outfile);
}

// Post-order traversal to compute and write dimensions
void postOrderDimensions(treeNode* root, FILE* outfile){
    if(root == NULL) return;
    postOrderDimensions(root->left, outfile);
    postOrderDimensions(root->right, outfile);
    if(root->leaf){
        fprintf(outfile, "%d(%d,%d)\n", root->label, root->width, root->height);
    }
    else{
        fprintf(outfile, "%d(%d,%d)\n", root->cut, root->width, root->height);
    }
}

// Post-order traversal to compute and write coordinates to third output file
void postOrderCoordinates(treeNode* root, int x, int y, FILE* outfile){
    if(root == NULL) return;

    // Assign the current coordinates to the root
    root->x = x;
    root->y = y;

    if(root->leaf){
        fprintf(outfile, "%d((%d,%d)(%d,%d))\n", root->label, root->width, root->height, root->x, root->y);
    }
    else{
        if(root->cut == 'H'){
            postOrderCoordinates(root->left, x, y + root->right->height, outfile);
            postOrderCoordinates(root->right, x, y, outfile);
        }
        else if(root->cut == 'V'){
            postOrderCoordinates(root->left, x, y, outfile);
            postOrderCoordinates(root->right, x + root->left->height, y, outfile);
        }
    }
}

// Free all allocated memory in the tree
void freeTree(treeNode* root){
    if(root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Main function
int main(int argc, char* argv[]){

}