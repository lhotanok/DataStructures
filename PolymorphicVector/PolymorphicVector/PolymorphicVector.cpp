#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class element {
public:
    virtual ~element() = default;
    virtual void print() const {}
    virtual const char* get_value_type() const { return nullptr; }
};

class int_element : public element {
private:
    int value;
public:
    int_element(int val) :value(val) {}
    const char* get_value_type() const override { return typeid(value).name(); };
    void print() const override { cout << value; };
};

class double_element : public element {
private:
    double value;
public:
    double_element(double val) :value(val) {}
    const char* get_value_type() const override { return typeid(value).name(); };
    void print() const override { cout << value; };
};

class string_element : public element {
private:
    string value;
public:
    string_element(string val) :value(val) {}
    const char* get_value_type() const override { return typeid(value).name(); };
    void print() const override { cout << value; };
};

class polymorphic_vector {
private:
    vector<unique_ptr<element>> vector;
public:
    element& operator[](size_t i) {
        return *vector[i].get();
    }

    void push_back(int value) {
        vector.push_back(make_unique<int_element>(value));
    }
    void push_back(double value) {
        vector.push_back(make_unique<double_element>(value));
    }
    void push_back(const string& value) {
        vector.push_back(make_unique<string_element>(value));
    }
    void print(size_t i) const {
        element* element_ptr = vector.at(i).get();
        element_ptr->print();
    }
    void print_all() const {
        for (auto&& curr_element : vector) {
            element* element_ptr = curr_element.get();
            element_ptr->print();
            cout << ' ';
        }
        cout << endl;
    }
};
int main()
{
    polymorphic_vector v;

    v.push_back(1); v.push_back(2.3); v.push_back("four");

    v[1].print(); // 2.3
    cout << endl;

    v.print_all(); // 1 2.3 "four"

    cout << v[0].get_value_type() << endl;
    cout << v[1].get_value_type() << endl;
    cout << v[2].get_value_type() << endl;
}
