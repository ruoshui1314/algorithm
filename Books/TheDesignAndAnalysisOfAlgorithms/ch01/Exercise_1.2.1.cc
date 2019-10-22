/*
Old World puzzle A peasant finds himself on a riverbank with a wolf,
a goat, and a head of cabbage. He needs to transport all three to the
other side of the river in his boat. However, the boat has room for only
the peasant himself and one other item (either the wolf, the goat, or the
cabbage). In his absence, the wolf would eat the goat, and the goat would
eat the cabbage. Solve this problem for the peasant or prove it has no
solution. (Note: The peasant is a vegetarian but does not like cabbage
and hence can eat neither the goat nor the cabbage to help him solve the
problem. And it goes without saying that the wolf is a protected species.)
*/

#include <iostream>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

struct Item {
    string name;
    int value;
    bool destination;

    Item(const char* n, int v, bool d):
        name(n),
        value(v),
        destination(d) {}

    Item(const Item& item):
        name(item.name),
        value(item.value),
        destination(item.destination) {}

    Item& operator=(const Item& item) {
        if (&item == this)
            return *this;
        name = item.name;
        value = item.value;
        destination = item.destination;
        return *this;
    }
};

struct Action {
    Item peasant;
    string good;
    bitset<4> bits;

    Action(): peasant("peasant", 1, false) {}

    void update_bits(const vector<Item>& items) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].destination)
                bits.set(i);
        }
    }
};

bool arrive_destination(const vector<Item>& items) {
    for (auto& item : items) {
        if (!item.destination)
            return false;
    }
    return true;
}

bool is_new_state(const vector<Action>& actions, Action& action) {
    if (action.bits == 0)
        return false;
    for (auto& a : actions) {
        if (a.bits == action.bits)
            return false;
    }
    return true;
}

void try_cross_river(vector<Item>& items, vector<Action>& actions) {
    // find solution and print the actions
    if (arrive_destination(items)) {
        cout << "------ solution begin -----" << endl;
        for (auto& action : actions) {
            cout << action.peasant.name << (action.good.empty() ? "" : " transports " + action.good)
                << " " << (action.peasant.destination ? "to the destination" : "back") << endl;
        }
        cout << "------ solution end -----" << endl;
        return;
    }

    Item& peasant = items[0];
    vector<Item> goods;
    for (size_t i = 1; i < items.size(); ++i) {
        if (peasant.destination != items[i].destination)
            goods.push_back(items[i]);
    }
    // we should return when the size of left goods without peasant is bigger than 2
    if (goods.size() > 2)
        return;
    // no peasant and some goods could be eat by other.
    if (goods.size() == 2 && goods[0].value << 1 == goods[1].value)
        return;
    for (auto& item : items) {
        Action action;
        bool current = items[0].destination;
        items[0].destination = !current;
        action.peasant = items[0];
        // peasant could back to origin place without any good.
        if (item.value == 1 && current) {
            action.update_bits(items);
            if (is_new_state(actions, action)) {
                actions.push_back(action);
                try_cross_river(items, actions);
                actions.pop_back();
            }
        }
        // take goods to cross river and maybe it is back to origin.
        else if (item.destination == current) {
            action.good = item.name;
            item.destination = !item.destination;
            action.update_bits(items);
            if (is_new_state(actions, action)) {
                actions.push_back(action);
                try_cross_river(items, actions);
                actions.pop_back();
            }
            item.destination = !item.destination;
        }
        items[0].destination = current;
    }
}

int main() {
    vector<Item> items;
    items.emplace_back("peasant", 1, false);
    items.emplace_back("wolf", 1 << 1, false);
    items.emplace_back("goat", 1 << 2, false);
    items.emplace_back("cabbage", 1 << 3, false);
    vector<Action> actions;
    try_cross_river(items, actions);
    return 0;
}