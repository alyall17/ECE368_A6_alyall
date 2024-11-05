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
    struct treeNode* left; // Left child
    struct treeNode* right; // Right child
}treeNode;

// Creates and returns new leaf node (block)
treeNode* createLeaf(int label, int width, int height){
    treeNode* new = (treeNode*)malloc(sizeof(treeNode)); // Allocate node memory

    // Initialize with appropriate data
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

// Creates and returns a new internal node (a cut)
treeNode* createInternal(char cut, treeNode* left, treeNode* right){
    treeNode* new = (treeNode*)malloc(sizeof(treeNode)); // Allocate memory

    // Initialize with appropriate data
    new->leaf = 0;
    new->label = -1;
    new->cut = cut;
    new->left = left;
    new->right = right;

    // Compute dimensions (update height and width) of enclosing room based on cut types
    if(cut == 'H'){ // Horizontal cut -- creates top and bottom blocks
        new->width = left->width > right->width ? left->width : right->width;
        new->height = left->height + right->height;
    }
    else if(cut == 'V'){ // Vertical cut -- creates left and right blocks
        new->width = left->width + right->width;
        new->height = left->height > right->height ? left->height : right->height;
    }
    new->x = 0; // Initialize x-coordinate
    new->y = 0; // Initialize y-coordinate
    return new;
}

// Build binary tree from a post-order input file containing either leaf or internal blocks
treeNode* buildTree(FILE* infile){
    char line[100]; // Buffer for storing file line
    treeNode* stack[500]; // Stack to help build the tree, don't need outside of this function
    int top = -1; // Stack pointer

    // Read each line from the input file and create corresponding node
    while(fgets(line, sizeof(line), infile)){
        // For internal nodes, pop right and left child from stack and push new node to stack
        if(line[0] == 'H' || line[0] == 'V'){
            // Internal node
            treeNode* right = stack[top--];
            treeNode* left = stack[top--];
            stack[++top] = createInternal(line[0], left, right);
        }
        // For leaf nodes, parse label, width, and height and add new node to stack
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

// Pre-order traversal to write tree structure to the first output file
void preOrder(treeNode* root, FILE* outfile){
    if(root == NULL) return; // Base case -- empty node

    // Write block label and dimensions for leaf and cut type for internal
    if(root->leaf){
        fprintf(outfile, "%d(%d,%d)\n", root->label, root->width, root->height);
    }
    else{
        fprintf(outfile, "%c\n", root->cut);
    }

    // Traverse left and right subtrees
    preOrder(root->left, outfile);
    preOrder(root->right, outfile);
}

// Post-order traversal to compute and write dimensions of each block and cut to the second output file
void postOrderDimensions(treeNode* root, FILE* outfile){
    if(root == NULL) return; // Base case -- empty node

    // Traverse left and right subtrees
    postOrderDimensions(root->left, outfile);
    postOrderDimensions(root->right, outfile);

    // Write block dimensions for leaf, and cut type and block dimensions for internal
    if(root->leaf){
        fprintf(outfile, "%d(%d,%d)\n", root->label, root->width, root->height);
    }
    else{
        fprintf(outfile, "%c(%d,%d)\n", root->cut, root->width, root->height);
    }
}

// Post-order traversal to compute and write coordinates to third output file
void postOrderCoordinates(treeNode* root, int x, int y, FILE* outfile){
    if(root == NULL) return;

    // Assign the current coordinates to the root
    root->x = x;
    root->y = y;

    // If leaf, write the block's coordinates
    if(root->leaf){
        fprintf(outfile, "%d((%d,%d)(%d,%d))\n", root->label, root->width, root->height, root->x, root->y);
    }
    // If internal (cut), calculate the coordinates for children
    else{
        // Left child goes above right for horizontal cuts
        if(root->cut == 'H'){
            postOrderCoordinates(root->left, x, y + root->right->height, outfile);
            postOrderCoordinates(root->right, x, y, outfile);
        }
        // Left child goes to the left of right child for vertical cuts
        else if(root->cut == 'V'){
            postOrderCoordinates(root->left, x, y, outfile);
            postOrderCoordinates(root->right, x + root->left->width, y, outfile);
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
    if(argc != 5){
        return 1;
    }

    // Open the input file
    FILE* infile = fopen(argv[1], "r");
    if(!infile){
        return 1;
    }

    // Build the tree from the input file
    treeNode* root = buildTree(infile);
    fclose(infile);

    // Open the output files
    FILE* outfile1 = fopen(argv[2], "w");
    FILE* outfile2 = fopen(argv[3], "w");
    FILE* outfile3 = fopen(argv[4], "w");

    if(!outfile1 || !outfile2 || !outfile3){
        return 1;
    }

    // Write the pre-order traversal to the first output file
    preOrder(root, outfile1);
    fclose(outfile1);

    // Write the dimensions of the rooms in post-order to the second output file
    postOrderDimensions(root, outfile2);
    fclose(outfile2);

    // Write the coordinates of the blocks in post-order to the third output file
    postOrderCoordinates(root, 0, 0, outfile3);
    fclose(outfile3);

    // Free memory
    freeTree(root);

    return 0;
}
