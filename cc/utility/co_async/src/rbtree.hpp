#pragma once

#include <functional>
#include <utility>

template <class Value, class Compare = std::less<Value>>
struct RBTree {
    enum RBColor
    {
        RED,
        BLACK
    };

    class RBNode
    {
        friend struct RBTree;

        RBTree *tree;
        RBNode *left;
        RBNode *right;
        RBNode *parent;
        RBColor color;

      public:
        RBNode() noexcept : tree(nullptr), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        RBNode(RBNode &&) = delete;
        ~RBNode() noexcept
        {
            if (tree) {
                tree->do_erase(this);
            }
        }
    };


  private:
    RBNode *root;
    Compare comp;

  public:

    RBTree() noexcept : root(nullptr) {}
    explicit RBTree(Compare comp) noexcept(noexcept(Compare(comp)))
        : root(nullptr),
          comp(comp) {}
    RBTree(RBTree &&) = delete;
    ~RBTree() noexcept {}

    void insert(Value &value) noexcept
    {
        do_insert(&static_cast<RBNode &>(value));
    }
    void erase(Value &value) noexcept
    {
        do_erase(&static_cast<RBNode &>(value));
    }
    bool empty() noexcept
    {
        return root == nullptr;
    }

    Value &front() const noexcept
    {
        return static_cast<Value &>(*get_front());
    }
    Value &back() const noexcept
    {
        return static_cast<Value &>(*get_back());
    }

    template <typename Visitor>
    void traversal_inorder(Visitor &&visitor)
    {
        do_traversal_inorder(root, std::forward<Visitor>(visitor));
    }



  private:
    bool compare(RBNode *left, RBNode *right) const noexcept
    {
        return comp(static_cast<Value &>(*left), static_cast<Value &>(*right));
    }

    void rotate_left(RBNode *node) noexcept
    {
        RBNode *right_child = node->right;
        node->right         = right_child->left;
        if (right_child->left) { // node->right
            right_child->left->parent = node;
        }
        right_child->parent = node->parent;

        if (!node->parent) {
            root = right_child;
        }
        else if (node == node->parent->left) {
            node->parent->left = right_child;
        }
        else {
            node->parent->right = right_child;
        }

        right_child->left = node;
        node->parent      = right_child;
    }

    void rotate_right(RBNode *node) noexcept
    {
        RBNode *left_child = node->left;
        node->left         = left_child->right;
        if (left_child->right) { // node->left
            left_child->right->parent = node;
        }
        left_child->parent = node->parent;
        if (!node->parent) {
            root = left_child;
        }
        else if (node == node->parent->left) {
            node->parent->left = left_child;
        }
        else {
            node->parent->right = left_child;
        }
        left_child->right = node;
        node->parent      = left_child;
    }

    void fix_violation(RBNode *node) noexcept
    {
        RBNode *parent       = nullptr;
        RBNode *grand_parent = nullptr;

        while (node != root &&
               node->color != BLACK &&
               node->parent->color == RED)
        {
            parent       = node->parent;
            grand_parent = node->parent->parent;

            if (parent == grand_parent->left) {
                RBNode *uncle = grand_parent->right;

                // same balance?
                if (uncle != nullptr && uncle->color == RED) {
                    grand_parent->color = RED;
                    parent->color       = BLACK;
                    uncle->color        = BLACK;
                    node                = grand_parent;
                }
                // left(if) then right
                else {
                    if (node == parent->right) {
                        rotate_left(parent);
                        node   = parent;
                        parent = node->parent;
                    }
                    rotate_right(grand_parent);
                    std::swap(parent->color, grand_parent->color);
                    node = parent;
                }
            }
            else {
                RBNode *uncle = grand_parent->left;
                // same balance?
                if (uncle != nullptr && uncle->color == RED) {
                    grand_parent->color = RED;
                    parent->color       = BLACK;
                    uncle->color        = BLACK;
                    node                = grand_parent;
                }
                // right(if) then left
                else {
                    if (node == parent->left) {
                        rotate_right(parent);
                        node   = parent;
                        parent = node->parent;
                    }
                    rotate_left(grand_parent);
                    std::swap(parent->color, grand_parent->color);
                    node = parent;
                }
            }
        }
        //
        root->color = BLACK;
    }

    void do_insert(RBNode *node) noexcept
    {
        node->left = node->right = nullptr;
        node->tree               = this;
        node->color              = RED;

        RBNode *parent  = nullptr;
        RBNode *current = root;

        while (current != nullptr) {
            parent  = nullptr;
            current = compare(node, current) ? current->left : current->right;
        }

        node->parent = parent;
        // did not iterate
        if (parent == nullptr) {
            root = node;
        }
        else if (compare(node, parent)) {
            parent->left = node;
        }
        else {
            parent->right = node;
        }

        fix_violation(node);
    }

    void do_erase(RBNode *current) noexcept
    {
        current->tree = nullptr;

        RBNode *node  = nullptr;
        RBNode *child = nullptr;
        RBColor color = RED;

        if (current->left && current->right)
        {
            // find the minimum val on right tree  but maximum for left tree
            RBNode *replace = current;
            replace         = current->right;
            while (replace->left != nullptr) {
                replace = replace->left;
            }

            // if do some loops above
            if (current != replace->parent) {
                current->parent->left  = replace->right;
                replace->right         = current->right;
                current->right->parent = replace;
            }
            else {
                replace->parent = current;
            }

            if (current == root) {
                root = replace;
            }
            else if (current->parent->left == current) {
                current->parent->left = replace;
            }
            else {
                current->parent->right = replace;
            }

            replace->left         = current->left;
            current->left->parent = replace;

            node  = replace;
            color = node->color;
            child = node->right;
        }
        else {
            node  = current;
            color = node->color;
            child = node->left ? node->left : node->right;
        }


        if (child) {
            child->parent = node->parent;
        }

        if (node == root) {
            root = child;
        }
        else if (node->parent->left == node) {
            node->parent->left = child;
        }
        else {
            node->parent->right = child;
        }

        if (color == BLACK && root) {
            fix_violation(child ? child : node->parent);
        }
    }

    RBNode *get_front() const noexcept
    {
        RBNode *cur = root;
        while (cur->left != nullptr) {
            cur = cur->left;
        }
        return cur;
    }
    RBNode *get_back() const noexcept
    {
        RBNode *cur = root;
        while (cur->right != nullptr) {
            cur = cur->right;
        }
        return cur;
    }

    template <typename Visitor>
    void do_traversal_inorder(RBNode *node, Visitor &&visitor)
    {
        if (!node) {
            return;
        }
        do_traversal_inorder(node->left, visitor);
        visitor(node);
        do_traversal_inorder(node->right, visitor);
    }
};
