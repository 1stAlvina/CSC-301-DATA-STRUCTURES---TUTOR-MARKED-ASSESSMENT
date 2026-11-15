/*
 * CSC 301 - Data Structures
 * Question 1: Implementing a Custom Linked List
 *
 * A singly linked list of integers that supports:
 *   - inserting at the beginning, in the middle, at the end, and at any position
 *   - deleting by position or by value
 *   - displaying all nodes
 *   - error handling for invalid operations (e.g. deleting from an empty list)
 *
 * The program runs three separate test cases with different input data
 * and prints the result of every operation.
 *
 * Compile:  g++ -std=c++17 -Wall -Wextra -o q1_linked_list q1_linked_list.cpp
 * Run:      ./q1_linked_list
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/*
 * Node
 * One element of the list. It stores a value and a pointer to the next node.
 * The last node's "next" pointer is nullptr, which marks the end of the list.
 */
struct Node {
    int data;
    Node* next;

    explicit Node(int value) : data(value), next(nullptr) {}
};

/*
 * LinkedList
 * Keeps a pointer to the first node (head) and a count of nodes (size).
 * Every node is created with "new" on the heap and released with "delete".
 */
class LinkedList {
private:
    Node* head;   // first node in the list, or nullptr if the list is empty
    int size;     // number of nodes currently in the list

public:
    // Constructor: starts with an empty list.
    LinkedList() : head(nullptr), size(0) {}

    // Destructor: frees every node so no memory is leaked.
    ~LinkedList() {
        clear();
    }

    // The list owns raw pointers, so copying is disabled to avoid two lists
    // deleting the same nodes (double free).
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    // Returns true if the list has no nodes.
    bool isEmpty() const {
        return head == nullptr;
    }

    // Returns the number of nodes in the list.
    int getSize() const {
        return size;
    }

    /*
     * insertAtBeginning
     * Creates a new node and makes it the new head.
     * The new node points to the old head. Runs in O(1) time.
     */
    void insertAtBeginning(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        size++;
    }

    /*
     * insertAtEnd
     * Walks to the last node and links the new node after it.
     * If the list is empty, the new node becomes the head. Runs in O(n) time.
     */
    void insertAtEnd(int value) {
        Node* newNode = new Node(value);

        if (isEmpty()) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    /*
     * insertAtPosition
     * Inserts a value so that it ends up at the given 1-based position.
     *   position 1        -> beginning of the list
     *   position size + 1 -> end of the list
     * Throws out_of_range if the position is outside 1 .. size + 1.
     */
    void insertAtPosition(int position, int value) {
        if (position < 1 || position > size + 1) {
            throw out_of_range("Insert failed: position " + to_string(position) +
                               " is invalid (valid range is 1 to " +
                               to_string(size + 1) + ").");
        }

        if (position == 1) {
            insertAtBeginning(value);
            return;
        }

        // Stop at the node just BEFORE the target position.
        Node* previous = head;
        for (int i = 1; i < position - 1; i++) {
            previous = previous->next;
        }

        // Link the new node in between "previous" and the node after it.
        Node* newNode = new Node(value);
        newNode->next = previous->next;
        previous->next = newNode;
        size++;
    }

    /*
     * insertInMiddle
     * Inserts a value at the middle of the list.
     * For a list of n nodes the new node goes to position (n / 2) + 1,
     * so it sits after the first half of the existing nodes.
     * On an empty list this simply creates the first node.
     */
    void insertInMiddle(int value) {
        insertAtPosition(size / 2 + 1, value);
    }

    /*
     * deleteAtPosition
     * Removes the node at the given 1-based position and returns its value.
     * Throws underflow_error if the list is empty and out_of_range if the
     * position does not exist.
     */
    int deleteAtPosition(int position) {
        if (isEmpty()) {
            throw underflow_error("Delete failed: the list is empty.");
        }
        if (position < 1 || position > size) {
            throw out_of_range("Delete failed: position " + to_string(position) +
                               " is invalid (valid range is 1 to " +
                               to_string(size) + ").");
        }

        Node* toDelete;

        if (position == 1) {
            // Removing the head: the second node becomes the new head.
            toDelete = head;
            head = head->next;
        } else {
            // Find the node before the one being removed, then bypass it.
            Node* previous = head;
            for (int i = 1; i < position - 1; i++) {
                previous = previous->next;
            }
            toDelete = previous->next;
            previous->next = toDelete->next;
        }

        int removedValue = toDelete->data;
        delete toDelete;   // return the node's memory to the heap
        size--;
        return removedValue;
    }

    /*
     * deleteByValue
     * Removes the FIRST node that holds the given value.
     * Throws underflow_error if the list is empty and invalid_argument
     * if the value is not in the list.
     */
    void deleteByValue(int value) {
        if (isEmpty()) {
            throw underflow_error("Delete failed: the list is empty.");
        }

        // Special case: the value is in the head node.
        if (head->data == value) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
            size--;
            return;
        }

        // Look one node ahead so we always hold the node before the match.
        Node* previous = head;
        while (previous->next != nullptr && previous->next->data != value) {
            previous = previous->next;
        }

        if (previous->next == nullptr) {
            throw invalid_argument("Delete failed: value " + to_string(value) +
                                   " was not found in the list.");
        }

        Node* toDelete = previous->next;
        previous->next = toDelete->next;
        delete toDelete;
        size--;
    }

    /*
     * display
     * Prints every node from head to tail in the form:
     *   HEAD -> 10 -> 20 -> 30 -> NULL   (size = 3)
     */
    void display() const {
        cout << "   List: HEAD -> ";
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "NULL   (size = " << size << ")" << endl;
    }

    /*
     * clear
     * Deletes every node one by one and resets the list to empty.
     */
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;  // save the link before deleting
            delete current;
            current = nextNode;
        }
        head = nullptr;
        size = 0;
    }
};

/* ------------------------------------------------------------------------
 * Test helpers
 * Each helper performs one operation, prints what was attempted, catches
 * any exception the list throws, and then displays the list.
 * ---------------------------------------------------------------------- */

void doInsertBeginning(LinkedList& list, int value) {
    cout << "-> Insert " << value << " at the BEGINNING" << endl;
    list.insertAtBeginning(value);
    list.display();
}

void doInsertEnd(LinkedList& list, int value) {
    cout << "-> Insert " << value << " at the END" << endl;
    list.insertAtEnd(value);
    list.display();
}

void doInsertMiddle(LinkedList& list, int value) {
    cout << "-> Insert " << value << " in the MIDDLE" << endl;
    list.insertInMiddle(value);
    list.display();
}

void doInsertAt(LinkedList& list, int position, int value) {
    cout << "-> Insert " << value << " at POSITION " << position << endl;
    try {
        list.insertAtPosition(position, value);
    } catch (const exception& e) {
        cout << "   ERROR: " << e.what() << endl;
    }
    list.display();
}

void doDeleteAt(LinkedList& list, int position) {
    cout << "-> Delete node at POSITION " << position << endl;
    try {
        int removed = list.deleteAtPosition(position);
        cout << "   Removed value " << removed << endl;
    } catch (const exception& e) {
        cout << "   ERROR: " << e.what() << endl;
    }
    list.display();
}

void doDeleteValue(LinkedList& list, int value) {
    cout << "-> Delete node with VALUE " << value << endl;
    try {
        list.deleteByValue(value);
        cout << "   Removed value " << value << endl;
    } catch (const exception& e) {
        cout << "   ERROR: " << e.what() << endl;
    }
    list.display();
}

void printHeader(const string& title) {
    cout << endl;
    cout << "============================================================" << endl;
    cout << title << endl;
    cout << "============================================================" << endl;
}

int main() {
    /* ---------------- Test Set 1: typical values ---------------- */
    printHeader("TEST SET 1: Typical data (10, 20, 30, 40, 50)");
    {
        LinkedList list;
        list.display();
        doInsertEnd(list, 30);         // end of an empty list -> first node
        doInsertBeginning(list, 10);   // new head
        doInsertMiddle(list, 20);      // goes between 10 and 30
        doInsertEnd(list, 50);         // new tail
        doInsertAt(list, 4, 40);       // goes between 30 and 50
        doDeleteAt(list, 3);           // removes 30 (middle)
        doDeleteValue(list, 10);       // removes the head
        doDeleteValue(list, 50);       // removes the tail
        doDeleteValue(list, 99);       // error: value not found
    }

    /* ---------------- Test Set 2: empty list and bad positions ---------------- */
    printHeader("TEST SET 2: Error handling on an empty list and invalid positions");
    {
        LinkedList list;
        list.display();
        doDeleteAt(list, 1);           // error: list is empty
        doDeleteValue(list, 5);        // error: list is empty
        doInsertMiddle(list, 5);       // middle of empty list -> first node
        doInsertAt(list, 0, 1);        // error: position 0 is invalid
        doInsertAt(list, 5, 1);        // error: beyond size + 1
        doInsertAt(list, 2, 7);        // valid: position size + 1 = end
        doDeleteAt(list, 3);           // error: position does not exist
        doDeleteAt(list, 1);           // removes 5
        doDeleteAt(list, 1);           // removes 7, list becomes empty
        doDeleteAt(list, 1);           // error: list is empty again
    }

    /* ---------------- Test Set 3: negatives, zero and duplicates ---------------- */
    printHeader("TEST SET 3: Negative numbers, zero and duplicate values");
    {
        LinkedList list;
        doInsertBeginning(list, 0);
        doInsertBeginning(list, -15);
        doInsertEnd(list, 8);
        doInsertEnd(list, 8);          // duplicate value
        doInsertMiddle(list, -3);
        doInsertAt(list, 4, 100);
        doInsertMiddle(list, 42);
        doDeleteValue(list, 8);        // removes only the FIRST 8
        doDeleteAt(list, list.getSize()); // removes the last node
        doDeleteValue(list, -15);      // removes the head
        doDeleteAt(list, 2);
    }

    cout << endl << "All tests finished. Every node was freed by the destructor." << endl;
    return 0;
}
