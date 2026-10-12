#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class ElevatorSystem {
private:
    vector<int> requests;
    int current_floor;
    int direction;  // 1 for up, -1 for down
    
    void remove_request(int floor) {
        auto it = find(requests.begin(), requests.end(), floor);
        if (it != requests.end()) {
            requests.erase(it);
        }
    }
    
    int find_nearest_up() {
        int best = -1;
        int best_dist = 1000000000;
        
        for (int floor : requests) {
            if (floor >= current_floor) {
                int dist = floor - current_floor;
                if (dist < best_dist) {
                    best_dist = dist;
                    best = floor;
                }
            }
        }
        return best;
    }
    
    int find_nearest_down() {
        int best = -1;
        int best_dist = 1000000000;
        
        for (int floor : requests) {
            if (floor <= current_floor) {
                int dist = current_floor - floor;
                if (dist < best_dist) {
                    best_dist = dist;
                    best = floor;
                }
            }
        }
        return best;
    }
    
public:
    ElevatorSystem() : current_floor(0), direction(1) {}
    
    void add_request(int floor) {
        requests.push_back(floor);
    }
    
    void cancel_request(int floor) {
        remove_request(floor);
    }
    
    void execute_action() {
        if (requests.empty()) {
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