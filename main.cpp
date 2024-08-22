#include <iostream>
#include <mutex>

struct Node {
    int value;
    Node* next;
    std::mutex node_mutex;
};

class FineGrainedQueue {
private:
    Node* head;
    std::mutex queue_mutex;

public:
    FineGrainedQueue() : head(nullptr) {}

    void insertIntoMiddle(int value, int pos) {
        Node* newNode = new Node;
        newNode->value = value;
        newNode->next = nullptr;

        std::lock_guard<std::mutex> queue_lock(queue_mutex);

        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* current = head;
            Node* previous = nullptr;
            int currPos = 1;

            while (current != nullptr && currPos < pos) {
                previous = current;
                current = current->next;
                currPos++;
            }

            if (current == nullptr) {
                previous->next = newNode;
            }
            else {
                std::lock_guard<std::mutex> node_lock(previous->node_mutex);
                std::lock_guard<std::mutex> node_lock2(current->node_mutex);
                previous->next = newNode;
                newNode->next = current;
            }
        }
    }

    Node* getHead() const {
        return head;
    }
};

int main() {
    FineGrainedQueue queue;

    queue.insertIntoMiddle(13, 1);

    queue.insertIntoMiddle(22, 3);

    queue.insertIntoMiddle(30, 5);

    queue.insertIntoMiddle(15, 3);

    queue.insertIntoMiddle(2, 3);


    Node* current = queue.getHead();
    while (current != nullptr) {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;

    return 0;
}