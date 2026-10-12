#include <cstdio>
#include <iostream>

using namespace std;

const int MAX_N = 500000;

class MinHeap {
private:
    int arr[MAX_N + 5];
    int cnt;
    
    void heapify_up(int idx) {
        while (idx > 1 && arr[idx] < arr[idx / 2]) {
            int temp = arr[idx];
            arr[idx] = arr[idx / 2];
            arr[idx / 2] = temp;
            idx /= 2;
        }
    }
    
    void heapify_down(int idx) {
        while (idx * 2 <= cnt) {
            int smallest = idx;
            if (idx * 2 <= cnt && arr[idx * 2] < arr[smallest]) {
                smallest = idx * 2;
            }
            if (idx * 2 + 1 <= cnt && arr[idx * 2 + 1] < arr[smallest]) {
                smallest = idx * 2 + 1;
            }
            if (smallest == idx) break;
            
            int temp = arr[idx];
            arr[idx] = arr[smallest];
            arr[smallest] = temp;
            idx = smallest;
        }
    }
    
public:
    MinHeap() : cnt(0) {}
    
    void push(int x) {
        arr[++cnt] = x;
        heapify_up(cnt);
    }
    
    int top() {
        if (cnt == 0) return -1;
        return arr[1];
    }
    
    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        heapify_down(1);
    }
    
    bool empty() {
        return cnt == 0;
    }
    
    int size() {
        return cnt;
    }
};

class MaxHeap {
private:
    int arr[MAX_N + 5];
    int cnt;
    
    void heapify_up(int idx) {
        while (idx > 1 && arr[idx] > arr[idx / 2]) {
            int temp = arr[idx];
            arr[idx] = arr[idx / 2];
            arr[idx / 2] = temp;
            idx /= 2;
        }
    }
    
    void heapify_down(int idx) {
        while (idx * 2 <= cnt) {
            int largest = idx;
            if (idx * 2 <= cnt && arr[idx * 2] > arr[largest]) {
                largest = idx * 2;
            }
            if (idx * 2 + 1 <= cnt && arr[idx * 2 + 1] > arr[largest]) {
                largest = idx * 2 + 1;
            }
            if (largest == idx) break;
            
            int temp = arr[idx];
            arr[idx] = arr[largest];
            arr[largest] = temp;
            idx = largest;
        }
    }
    
public:
    MaxHeap() : cnt(0) {}
    
    void push(int x) {
        arr[++cnt] = x;
        heapify_up(cnt);
    }
    
    int top() {
        if (cnt == 0) return -1;
        return arr[1];
    }
    
    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        heapify_down(1);
    }
    
    bool empty() {
        return cnt == 0;
    }
    
    int size() {
        return cnt;
    }
};

class ElevatorSystem {
private:
    MinHeap up_heap;
    MaxHeap down_heap;
    int current_floor;
    int direction;  // 1 for up, -1 for down
    bool cancelled[MAX_N + 5];  // Simple array to track cancelled requests
    
    int find_up_target() {
        while (!up_heap.empty()) {
            int target = up_heap.top();
            if (target == -1) {
                up_heap.pop();
                continue;
            }
            if (target < MAX_N + 5 && cancelled[target]) {
                cancelled[target] = false;
                up_heap.pop();
                continue;
            }
            if (target >= current_floor) {
                return target;
            }
            // Move this request to down heap since it's below current position
            up_heap.pop();
            down_heap.push(target);
        }
        return -1;  // no suitable target found
    }
    
    int find_down_target() {
        while (!down_heap.empty()) {
            int target = down_heap.top();
            if (target == -1) {
                down_heap.pop();
                continue;
            }
            if (target < MAX_N + 5 && cancelled[target]) {
                cancelled[target] = false;
                down_heap.pop();
                continue;
            }
            if (target <= current_floor) {
                return target;
            }
            // Move this request to up heap since it's above current position
            down_heap.pop();
            up_heap.push(target);
        }
        return -1;  // no suitable target found
    }
    
public:
    ElevatorSystem() : current_floor(0), direction(1) {
        for (int i = 0; i < MAX_N + 5; i++) {
            cancelled[i] = false;
        }
    }
    
    void add_request(int floor) {
        if (floor >= current_floor) {
            up_heap.push(floor);
        } else {
            down_heap.push(floor);
        }
    }
    
    void cancel_request(int floor) {
        if (floor < MAX_N + 5) {
            cancelled[floor] = true;
        }
    }
    
    void execute_action() {
        int target = -1;
        
        if (direction == 1) {  // going up
            target = find_up_target();
            if (target != -1) {
                current_floor = target;
                up_heap.pop();
            } else {
                // reverse direction
                direction = -1;
                target = find_down_target();
                if (target != -1) {
                    current_floor = target;
                    down_heap.pop();
                }
            }
        } else {  // going down
            target = find_down_target();
            if (target != -1) {
                current_floor = target;
                down_heap.pop();
            } else {
                // reverse direction
                direction = 1;
                target = find_up_target();
                if (target != -1) {
                    current_floor = target;
                    up_heap.pop();
                }
            }
        }
    }
    
    int get_current_floor() const {
        return current_floor;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    
    ElevatorSystem elevator;
    
    for (int i = 0; i < n; i++) {
        char operation[20];
        scanf("%s", operation);
        
        if (operation[0] == 'l') {  // locate
            printf("%d\n", elevator.get_current_floor());
        } else if (operation[0] == 'a') {  // action or add
            if (operation[1] == 'c') {  // action
                elevator.execute_action();
            } else {  // add
                int floor;
                scanf("%d", &floor);
                elevator.add_request(floor);
            }
        } else if (operation[0] == 'c') {  // cancel
            int floor;
            scanf("%d", &floor);
            elevator.cancel_request(floor);
        }
    }
    
    return 0;
}