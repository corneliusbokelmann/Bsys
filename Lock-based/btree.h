#ifndef BTREE_H
#define BTREE_H

typedef struct Node {
  int value;
  struct Node *left;
  struct Node *right;
} Node;

typedef struct {
  Node *root;
  pthread_mutex_t mutex;
} BTree;

void btree_init(BTree *tree) {
  tree->root = NULL;
  pthread_mutex_init(&tree->mutex, NULL);
}

void btree_insert(BTree *tree, int value) {
  pthread_mutex_lock(&tree->mutex);
  if (tree->root == NULL) {
    tree->root = (Node*) malloc(sizeof(Node));
    tree->root->value = value;
    tree->root->left = NULL;
    tree->root->right = NULL;
  } else {
    Node *current = tree->root;
    while (1) {
      if (value < current->value) {
        if (current->left == NULL) {
          current->left = (Node*) malloc(sizeof(Node));
          current->left->value = value;
          current->left->left = NULL;
          current->left->right = NULL;
          break;
        } else {
          current = current->left;
        }
      } else {
        if (current->right == NULL) {
          current->right = (Node*) malloc(sizeof(Node));
          current->right->value = value;
          current->right->left = NULL;
          current->right->right = NULL;
          break;
        } else {
          current = current->right;
        }
      }
    }
  }
  pthread_mutex_unlock(&tree->mutex);
}

int btree_search(BTree *tree, int value) {
  pthread_mutex_lock(&tree->mutex);
  Node *current = tree->root;
  while (current != NULL) {
    if (current->value == value) {
      pthread_mutex_unlock(&tree->mutex);
      return 1;
    }
    if (value < current->value) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  pthread_mutex_unlock(&tree->mutex);
  return 0;
}

void btree_destroy(BTree *tree) {
  pthread_mutex_lock(&tree->mutex);
  if (tree->root == NULL) {
    return;
  }
  Node *current = tree->root;
  Node *prev = NULL;
  while (current != NULL) {
    if (current->left == NULL) {
      prev = current;
      current = current->right;
      free(prev);
    } else {
      Node *predecessor = current->left;
      while (predecessor->right != NULL && predecessor->right != current) {
        predecessor = predecessor->right;
      }
      if (predecessor->right == NULL) {
        predecessor->right = current;
        current = current->left;
      } else {
        predecessor->right = NULL;
        prev = current;
        current = current->right;
        free(prev);
      }
    }
  }
  pthread_mutex_unlock(&tree->mutex);
}

#endif
