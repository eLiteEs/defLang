// defLang
// @author Blas Fernández
// @date 29-12-2024

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;

class Modification {
    private:
    public:
        string originalValue = "";
        string newValue = "";

        Modification(string o, string n) {
            originalValue = o;
            newValue = n;
        }
};

vector<Modification> modifications;

int findFirstIndexOutsideQuotes(const std::string& input, const std::string& target) {
    bool inQuotes = false;
    size_t inputLength = input.length();
    size_t targetLength = target.length();

    for (size_t i = 0; i <= inputLength - targetLength; ++i) {
        if (input[i] == '"' || input[i] == '\'') {
            inQuotes = !inQuotes;
        }
                                                                       
        if (!inQuotes && input.substr(i, targetLength) == target) {
            return i;
        }
    }
                                                                       
    return -1;
}
vector<string> splitString(const string& input, char delimeter) {
    vector<string> result;
    istringstream stream(input);
    string token;

    while(std::getline(stream, token, delimeter)) {
        if(!token.empty()) {
            result.push_back(token);
        }
    }

    return result;
}
string removeSpacesOutsideQuotes(string source) {
    bool insideQuotes = false;
    string result = "";

    for(size_t i = 0; i < source.length(); i++) {
        if(source.at(i) == '\"') {
            insideQuotes = !insideQuotes;
            result.push_back(source.at(i));
        } else if(source.at(i) == ' ' && !insideQuotes) {
        } else {
            result.push_back(source.at(i));
        }
    }

    return result;
}
bool isOutsideQuotes(string source, char find) {
    bool insideQuotes = false;

    for(size_t i = 0; i < source.length(); i++) {
        if(source.at(i) == ' ' ) {
            insideQuotes = !insideQuotes;
        } else if(source.at(i) == find && !insideQuotes) {
            return true;
        }
    }

    return false;
}
void removeAll(string& source, char find) {
    string result = "";

    for(size_t i = 0; i < source.length(); i++) {
        if(source.at(i) != find) {
            result.push_back(source.at(i));
        }
    }

    source = result;
}

string translateSingleText(string& source) {
    for(Modification m : modifications) {
        if(source == m.originalValue) {
            source = m.newValue;
            return source;
        }
    }

    return source;
}

void makeLiteral(string& source) {
    string result = "";

    if(isOutsideQuotes(source, '+')) {
        for(string s : splitString(source, '+')) {
            result += translateSingleText(s);
        }

        source = result;
        removeAll(source, '\"');
    } else {
        translateSingleText(source);
        removeAll(source, '\"');
        return;
    }
}

void run(string command) {
    command = removeSpacesOutsideQuotes(command);

    if(command.substr(0, 5) == "print") {
        string args = command.substr(5);

        makeLiteral(args);

        cout << args << endl;
        return;
    }

    if(isOutsideQuotes(command, '=')) {
        string name = splitString(command, '=')[0];
        string content = splitString(command, '=')[1];

        Modification m(name, content);

        modifications.push_back(m);
        return;
    }
}

void read(string filename) {
    fstream f(filename);
    
    string line;

    while(getline(f, line)) {
        run(line);
    }

    f.close();
}

int main(int argc, char** argv) {
    string filename(argv[1]);

    read(filename);

    return 0;
}
