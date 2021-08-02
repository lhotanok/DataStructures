#include <iostream>
#include <variant>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

template<typename id_t, typename age_t, typename name_t, typename specific_t>
class zebra {
public:
    id_t id;
    age_t age;
    name_t name;
    specific_t stripes_count;

    zebra(id_t id, age_t age, const name_t& name, specific_t stripes_count) : age(age), name(name), stripes_count(stripes_count), id(id) {}

    pair<string, specific_t> get_specific_trait() {
        auto specific = make_pair("Number of stripes", stripes_count);
        return specific;
    }

    string get_species_name() {
        return "zebra";
    }
};

template<typename id_t, typename age_t, typename name_t, typename specific_t>
class giraffe {
public:
    id_t id;
    age_t age;
    name_t name;
    specific_t neck_length;

    giraffe(id_t id, age_t age, const name_t& name, specific_t neck_length) : age(age), name(name), neck_length(neck_length), id(id) {}

    pair<string, specific_t> get_specific_trait() {
        auto specific = make_pair("Neck length", neck_length);
        return specific;
    }

    string get_species_name() {
        return "giraffe";
    }
};

template<typename id_t, typename age_t, typename name_t, typename specific_t>
class elephant {
public:
    id_t id;
    age_t age;
    name_t name;
    specific_t trunk_length;

    elephant(id_t id, age_t age, const name_t& name, specific_t trunk_length) : age(age), name(name), trunk_length(trunk_length), id(id) {}

    pair<string, specific_t> get_specific_trait() {
        auto specific = make_pair("Trunk length", trunk_length);
        return specific;
    }

    string get_species_name() {
        return "elephant";
    }
};

template<typename id_t, typename age_t, typename name_t, typename specific_t>
class leopard {
public:
    id_t id;
    age_t age;
    name_t name;
    specific_t running_speed;

    leopard(id_t id, age_t age, const name_t& name, specific_t running_speed) : age(age), name(name), running_speed(running_speed), id(id) {}

    pair<string, specific_t> get_specific_trait() {
        auto specific = make_pair("Running speed", running_speed);
        return specific;
    }

    string get_species_name() {
        return "leopard";
    }
};

template<typename id_t, typename age_t, typename name_t, typename specific_t>
class turtle {
public:
    id_t id;
    age_t age;
    name_t name;
    specific_t shell_size;

    turtle(id_t id, age_t age, const name_t& name, specific_t shell_size) : age(age), name(name), shell_size(shell_size), id(id) {}

    pair<string, specific_t> get_specific_trait() {
        auto specific = make_pair("Shell size", shell_size);
        return specific;
    }

    string get_species_name() {
        return "turtle";
    }
};

template<typename id_t, typename age_t, typename name_t, typename specific_t>
class database {
private:
    using zebra_t = zebra<id_t, age_t, name_t, specific_t>;
    using giraffe_t = giraffe<id_t, age_t, name_t, specific_t>;
    using elephant_t = elephant<id_t, age_t, name_t, specific_t>;
    using leopard_t = leopard<id_t, age_t, name_t, specific_t>;
    using turtle_t = turtle<id_t, age_t, name_t, specific_t>;

    using variant_t = variant<zebra_t, giraffe_t, elephant_t, leopard_t, turtle_t>;

    multimap <id_t, variant_t> animals;

    name_t get_name(id_t id) const {
        auto variant = animals.find(id)->second;
        name_t name;
        visit([&](auto animal) {
            name = animal.name;
            }, variant);
        return name;
    }

    age_t get_age(id_t id) const {
        auto variant = animals.find(id)->second;
        age_t age;
        visit([&](auto animal) {
            age = animal.age;
            }, variant);
        return age;
    }

    template<typename key_t, typename animals_t>
    id_t get_id(const key_t& key, const animals_t& animal_database) const {
        auto variant = animal_database.find(key)->second;
        id_t id;
        visit([&](auto animal) {
            id = animal.id;
            }, variant);
        return id;
    }

    pair<string, specific_t> get_specific_trait(id_t id) const {
        auto variant = animals.find(id)->second;
        pair<string, specific_t> specific;
        visit([&](auto animal) {
            specific = animal.get_specific_trait();
            }, variant);
        return specific;
    }

    string get_species_name(id_t id) const {
        auto variant = animals.find(id)->second;
        string species;
        visit([&](auto animal) {
            species = animal.get_species_name();
            }, variant);
        return species;
    }
public:
    variant_t& operator[](id_t id) {
        return animals.find(id)->second;
    }

    void add_animal(id_t& id, variant_t&& animal) {
        animals.insert(pair<id_t, variant_t>(id, animal));
        id++;
    }

    void erase_animal(id_t id) {
        auto animal = animals.find(id);
        animals.erase(animal);
    }

    vector<id_t> get_all_ids() const {
        vector<id_t> ids;
        for (auto&& anim : animals) {
            id_t id = get_id(anim.first, animals);
            ids.push_back(id);
        }
        return ids;
    }

    template<typename key_t>
    multimap <key_t, variant_t> get_all_sorted_by_age() const {
        multimap <key_t, variant_t> sorted_animals;
        for (auto&& animal : animals) {
            key_t age = get_age(animal.first);
            variant_t variant = animal.second;
            sorted_animals.insert(pair<key_t, variant_t>(age, variant));
        }
        return sorted_animals;
    }

    template<typename key_t>
    multimap <key_t, variant_t> get_all_sorted_by_name() const {
        multimap <key_t, variant_t> sorted_animals;
        for (auto&& animal : animals) {
            key_t name = get_name(animal.first);
            variant_t variant = animal.second;
            sorted_animals.insert(pair<key_t, variant_t>(name, variant));
        }
        return sorted_animals;
    }

    template<typename stream_t>
    void write_header_into_stream(stream_t& stream) const {
        auto header = "Id;Species;Name;Age;Specific trait";
        stream << header << endl;
    }

    string get_animal_record(id_t id) const {
        string species = get_species_name(id);
        name_t name = get_name(id);
        age_t age = get_age(id);
        pair<string, specific_t> specific = get_specific_trait(id);
        stringstream ss;
        ss << id << ";" << species << ";" << name << ";" << age << ";" << specific.first << ": " << specific.second << ";" << endl;
        return ss.str();
    }

    void print_all() const {
        write_header_into_stream(cout);
        for (auto&& anim : animals) {
            id_t id = get_id(anim.first, animals);
            cout << get_animal_record(id);
        }
        cout << endl;
    }

    template<typename key_t>
    void print_all(const multimap <key_t, variant_t>& sorted_animals) const {
        write_header_into_stream(cout);
        for (auto&& anim : sorted_animals) {
            id_t id = get_id(anim.first, sorted_animals);
            cout << get_animal_record(id);
        }
        cout << endl;
    }
};

class file_writer {
public:
    template<typename id_t, typename age_t, typename name_t, typename specific_t>
    void write_database_into_file(const string& filename, const database<id_t, age_t, name_t, specific_t>& database) {
        ofstream ostrm(filename);
        database.write_header_into_stream(ostrm);
        vector<id_t> database_ids = database.get_all_ids();
        for (auto&& id : database_ids) {
            ostrm << database.get_animal_record(id);
        }
    }
};

class file_reader {
public:
    template<typename id_t, typename age_t, typename name_t, typename specific_t>
    database<id_t, age_t, name_t, specific_t> get_database_from_file(const string& filename) {
        database<id_t, age_t, name_t, specific_t> d;
        ifstream istrm(filename);
        string line;
        vector<string> parameters;

        getline(istrm, line); // pass header
        while (getline(istrm, line, ';')) {
            parameters.push_back(line);
        }
        save_animals_into_database(parameters, d);

        return d;
    }

    template<typename id_t, typename age_t, typename name_t, typename specific_t>
    void save_animals_into_database(const vector<string>& parameters, database<id_t, age_t, name_t, specific_t>& d) {
        size_t start_index = 0;
        size_t parameters_count = 4;
        while (start_index + parameters_count < parameters.size()) {
            id_t id = stoi(parameters.at(start_index));
            string species = parameters.at(start_index + 1);
            name_t name = parameters.at(start_index + 2);
            age_t age = stoi(parameters.at(start_index + 3));
            specific_t specific = stoi(get_trait(parameters.at(start_index + 4)));

            save_one_animal(id, species, name, age, specific, d);
            start_index += (parameters_count + 1);
        }
    }

    template<typename id_t, typename age_t, typename name_t, typename specific_t>
    void save_one_animal(id_t id, string species, name_t name, age_t age, specific_t specific, database<id_t, age_t, name_t, specific_t>& d) {
        if (species == "zebra")
        {
            d.add_animal(id, zebra(id, age, name, specific));
        }
        else if (species == "giraffe")
        {
            d.add_animal(id, giraffe(id, age, name, specific));
        }
        else if (species == "elephant")
        {
            d.add_animal(id, elephant(id, age, name, specific));
        }
        else if (species == "leopard")
        {
            d.add_animal(id, leopard(id, age, name, specific));
        }
        else if (species == "turtle")
        {
            d.add_animal(id, turtle(id, age, name, specific));
        }
    }

    string get_trait(string record) {
        size_t index = 0;
        while (index < record.length() && record[index] != ':')
        {
            index++;
        }
        stringstream ss;
        for (size_t i = index + 2; i < record.length() && record[i] != '\n'; i++)
        {
            ss << record[i];
        }
        return ss.str();
    }
};

int main()
{
    // example usage

    database<size_t, size_t, string, size_t> d;
    file_writer writer;
    file_reader reader;
    size_t id = 0;

    d.add_animal(id, zebra<size_t, size_t, string, size_t>(id, 18, "Flora", 16));
    d.add_animal(id, giraffe<size_t, size_t, string, size_t>(id, 5, "Leonie", 120));
    d.add_animal(id, elephant<size_t, size_t, string, size_t>(id, 31, "Henry", 97));
    d.add_animal(id, leopard<size_t, size_t, string, size_t>(id, 4, "Harry", 100));
    d.add_animal(id, turtle<size_t, size_t, string, size_t>(id, 128, "Alice", 67));

    d.print_all();
    writer.write_database_into_file("data.csv", d);

    d.erase_animal(2); // erase according to animal's id
    d.print_all();

    auto age_sorted = d.get_all_sorted_by_age<size_t>();
    d.print_all(age_sorted);

    auto name_sorted = d.get_all_sorted_by_name<string>();
    d.print_all(name_sorted);

    auto data_from_file = reader.get_database_from_file<size_t, size_t, string, size_t>("data.csv");
    data_from_file.print_all();
    data_from_file.print_all(data_from_file.get_all_sorted_by_name<string>());
    data_from_file.print_all(data_from_file.get_all_sorted_by_age<size_t>());
}
