#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib> // use for exit(0) func

std::string get_file_string();
void chose_person_to_search(std::string& name_one, std::string& name_two, std::string file_copy);
//bool choice_person(std::string name_one, std::string name_two);
void check_first(std::string name,std::string file_c, int& message_counter, std::string& str_to_get_len);
void check_second(std::string name,std::string file_c, int& message_counter, std::string& str_to_get_len);
void message_period(int& time_period, std::string file_copy);
void write_statistic(float t_c_s, int t_p, int m_c_s, int len_str_s, std::string name_s, float t_c_f, int m_s_f, std::string name_f, int len_str_f);

int main() {

    std::string file = get_file_string();
    std::string name_f;
    std::string name_s;
    std::string str_to_get_len_s;
    std::string str_to_get_len_f;
    //bool choice;
    float time_chatting_s;
    float time_chatting_f;
    int time_period = 0;
    int message_counter_s = 0;
    int message_counter_f = 0;

    chose_person_to_search(name_f, name_s, file);
    //choice = choice_person(name_f, name_s);
    check_second(name_s,file, message_counter_s, str_to_get_len_s);
    check_first(name_f,file, message_counter_f, str_to_get_len_f);
    message_period(time_period, file);

    time_chatting_s = str_to_get_len_s.length() / 4 / 60 / time_period;
    time_chatting_f = str_to_get_len_f.length() / 4 / 60 / time_period;

    std::string name_stat;
    //choice ? name_stat = name_s : name_stat = name_f;
    write_statistic(time_chatting_s, time_period, message_counter_s, str_to_get_len_s.length(), name_s, time_chatting_f, message_counter_f, name_f, str_to_get_len_f.length());


    //std::cout << "time_chatting: " << time_chatting << " time_period: " << time_period << " message_couter: " << message_counter << " str: " << str_to_get_len;
}

void write_statistic(float t_c_s, int t_p, int m_c_s, int len_str_s, std::string name_s, float t_c_f, int m_c_f, std::string name_f,int len_str_f) {
    std::ofstream outfile("stats.txt");
    if (outfile.is_open()){
        outfile << "Person: " << name_s << std::endl;
        outfile << std::endl;
        outfile << "Messages sent: " << m_c_s << std::endl;
        outfile << std::endl;
        outfile << "Time chatting ( " << t_p << " ) days : " << t_c_s << " min/day" << std::endl;
        outfile << std::endl;
        outfile << "Characters number: " << len_str_s;
        outfile << std::endl;
        outfile << std::endl;
        outfile << std::endl;
        outfile << std::endl;
        outfile << "Person: " << name_f << std::endl;
        outfile << std::endl;
        outfile << "Messages sent: " << m_c_f << std::endl;
        outfile << std::endl;
        outfile << "Time chatting ( " << t_p << " ) days : " << t_c_f << " min/day" << std::endl;
        outfile << std::endl;
        outfile << "Characters number: " << len_str_f;
    }
}

void message_period(int& time_period, std::string file_copy){
    int cursor = 0;
    time_t period_min = 0;
    time_t period_max = 0;
    auto d_unix = file_copy.find("date_unixtime", cursor);
        cursor = d_unix + 17;
        auto d_unix_c = file_copy.find("\"", cursor);
        period_min = std::stoi(file_copy.substr(cursor, d_unix_c-cursor));
        cursor = d_unix_c + 1;
    do {
        auto d_unix_max = file_copy.find("date_unixtime", cursor);
        cursor = d_unix_max + 17;
        auto d_unix_c_max = file_copy.find("\"", cursor);
        period_max = std::stoi(file_copy.substr(cursor, d_unix_c_max-cursor));
        cursor = d_unix_c_max + 1;
    } while ((cursor = file_copy.find("date_unixtime", cursor)) != std::string::npos);
    time_t unixtime = period_max - period_min;
    const int secondsPerDay = 24*60*60;
    time_period = unixtime / secondsPerDay;
}

void check_second(std::string name,std::string file_c, int& message_counter, std::string& str_to_get_len){
    int cursor = 0;
    std::string name_check;
    do
    {
        auto ind_name = file_c.find("from", cursor);
        cursor = ind_name + 8;
        auto ind_name_sec = file_c.find("\"", cursor);
        name_check = file_c.substr(cursor, ind_name_sec-cursor);
        cursor = ind_name_sec+1;
    } while (name_check != name);

    auto id = file_c.find("from_id", cursor);
    cursor = id + 11;
    auto id_sec = file_c.find("\"", cursor);
    std::string id_str = file_c.substr(cursor, id_sec-cursor);
    cursor = 0;

    while ((cursor = file_c.find("from_id", cursor)) != std::string::npos){ // Continue searching until no match is found
        auto index = file_c.find("from_id", cursor); // target first, position second
        cursor = index + 11;
        auto index_second = file_c.find("\"", cursor);
        std::string from_index = file_c.substr(cursor, index_second-cursor); // first - position, second - length
        cursor = index_second+1;
        if (from_index == id_str) {
            auto text = file_c.find("text", cursor);
            cursor = text + 8;
            auto text_c = file_c.find("\"", cursor);
            str_to_get_len += file_c.substr(cursor, text_c-cursor);
            cursor = text_c +1;
            message_counter ++;
        }
        else {
            auto text = file_c.find("text", cursor);
            cursor += 8;
            auto text_c = file_c.find("\"", cursor);
            cursor = text_c +1;
        }
    }
}

void check_first(std::string name,std::string file_c, int& message_counter, std::string& str_to_get_len){
    int cursor = 0;
    std::string name_check;
    do
    {
        auto ind_name = file_c.find("from", cursor);
        cursor = ind_name + 8;
        auto ind_name_sec = file_c.find("\"", cursor);
        name_check = file_c.substr(cursor, ind_name_sec-cursor);
        cursor = ind_name_sec+1;
    } while (name_check != name);

    auto id = file_c.find("from_id", cursor);
    cursor = id + 11;
    auto id_sec = file_c.find("\"", cursor);
    std::string id_str = file_c.substr(cursor, id_sec-cursor);
    cursor = 0;

    while ((cursor = file_c.find("from_id", cursor)) != std::string::npos){ // Continue searching until no match is found
        auto index = file_c.find("from_id", cursor); // target first, position second
        cursor = index + 11;
        auto index_second = file_c.find("\"", cursor);
        std::string from_index = file_c.substr(cursor, index_second-cursor); // first - position, second - length
        cursor = index_second+1;
        if (from_index == id_str) {
            auto text = file_c.find("text", cursor);
            cursor = text + 8;
            auto text_c = file_c.find("\"", cursor);
            str_to_get_len += file_c.substr(cursor, text_c-cursor);
            cursor = text_c +1;
            message_counter ++;
        }
        else {
            auto text = file_c.find("text", cursor);
            cursor += 8;
            auto text_c = file_c.find("\"", cursor);
            cursor = text_c +1;
        }
    }
}

/*
bool choice_person(std::string name_one, std::string name_two) {
    std::cout << "Enter person what u wanna check: " << "1." << name_one << " 2." << name_two << std::endl;
    std::string answer_str;
    std::cin >> answer_str;
    if(answer_str.find("-") == std::string::npos){
        if (answer_str == name_one) {return false;}
        else if ( answer_str == name_two) {return true;}
        else {std::cout << "Error, incorrect input"; exit(0);}
    }
    else{
        do{
            auto dash_finder = answer_str.find("-");
            answer_str.erase(dash_finder, 1);
            answer_str.insert(dash_finder, " ");
        }while(answer_str.find("-") != std::string::npos);
        if (answer_str == name_one) {return false;}
        else if ( answer_str == name_two) {return true;}
        else {std::cout << "Error, incorrect input"; exit(0);}
    }
}
*/
void chose_person_to_search(std::string& name_one, std::string& name_two, std::string file_copy) {
    int cursor = 0;
    std::string target = "from";
    std::string target_second = "\"";
    std::string name;
    while ((cursor = file_copy.find(target, cursor)) != std::string::npos){ // Continue searching until no match is found
        auto index = file_copy.find(target, cursor); // target first, position second
        cursor = index + 8;
        auto index_second = file_copy.find(target_second, cursor);
        name = file_copy.substr(cursor, index_second-cursor); // first - position, second - length
        if (name == ": "){continue;}
        if (name_one == ""){
            name_one = name;
        }
        else if (name_two == ""){
            name_two = name;
            if(name_one == name_two){
                name_two = "";
            }
        }
        cursor = index_second+1;
    }
}

std::string get_file_string() {
    std::ifstream inputFile("result.json");
    std::string line;
    std::string tmp;
    while (std::getline(inputFile, line)){
        tmp += line;
    }
    inputFile.close();
    return tmp;
}


/*
std::string name = "from";
    auto find_name = file.find(name);
    std::cout << "index " << find_name << std::endl;
    std::string sub = file.substr(find_name+8, file.find('\"'));
    std::cout << "index \" : " << file.find(find_name, '\"') << std::endl;
    std::cout << sub;*/