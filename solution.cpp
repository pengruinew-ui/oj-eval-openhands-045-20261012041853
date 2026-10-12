#include <cstdio>
#include <iostream>

using namespace std;

const int MAX_N = 500000;

class ElevatorSystem {
private:
    int requests[MAX_N + 5];
    int request_count;
    int current_floor;
    int direction;  // 1 for up, -1 for down
    
    // Insertion sort to keep array sorted
    void insert_sorted(int floor) {
        int i = request_count - 1;
        while (i >= 0 && requests[i] > floor) {
            requests[i + 1] = requests[i];
            i--;
        }
        requests[i + 1] = floor;
        request_count++;
    }
    
    // Binary search to find floor
    int find_floor(int floor) {
        int left = 0, right = request_count - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (requests[mid] == floor) {
                return mid;
            } else if (requests[mid] < floor) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1;
    }
    
    void remove_request(int floor) {
        int idx = find_floor(floor);
        if (idx != -1) {
            // Shift remaining elements
            for (int j = idx; j < request_count - 1; j++) {
                requests[j] = requests[j + 1];
            }
            request_count--;
        }
    }
    
    int find_nearest_up() {
        // Binary search for first element >= current_floor
        int left = 0, right = request_count - 1;
        int result = -1;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            if (requests[mid] >= current_floor) {
                result = requests[mid];
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        return result;
    }
    
    int find_nearest_down() {
        // Binary search for last element <= current_floor
        int left = 0, right = request_count - 1;
        int result = -1;
        
        while (left <= right) {
            int mid = (left + right) / 2;
            if (requests[mid] <= current_floor) {
                result = requests[mid];
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
public:
    ElevatorSystem() : current_floor(0), direction(1), request_count(0) {}
    
    void add_request(int floor) {
        if (request_count < MAX_N) {
            insert_sorted(floor);
        }
    }
    
    void cancel_request(int floor) {
        remove_request(floor);
    }
    
    void execute_action() {
        if (request_count == 0) {
            return;  // no requests, stay still
        }
        
        int target = -1;
        
        if (direction == 1) {  // going up
            target = find_nearest_up();
            if (target != -1) {
                current_floor = target;
                remove_request(target);
            } else {
                // reverse direction
                direction = -1;
                target = find_nearest_down();
                if (target != -1) {
                    current_floor = target;
                    remove_request(target);
                }
            }
        } else {  // going down
            target = find_nearest_down();
            if (target != -1) {
                current_floor = target;
                remove_request(target);
            } else {
                // reverse direction
                direction = 1;
                target = find_nearest_up();
                if (target != -1) {
                    current_floor = target;
                    remove_request(target);
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