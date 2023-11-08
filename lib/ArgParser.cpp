#include "ArgParser.h"
using namespace ArgumentParser;

bool ArgParser::Parse(std::vector<std::string> args) {
  for (int i = 0; i < args.size(); i++) {
    if (args[i] != "app") {
      if (args[i] == "--help") {
        is_help = true;
        return true;
      } else if (args[i].find("=") != args[i].npos) {
        std::string val = args[i].substr(args[i].find("=") + 1, args[i].size() - args[i].find("=") + 1);

        if (val[0] <= '9' && val[0] >= '0' || val[0] == '-') {
            std::string name_ = args[i].substr(2, args[i].find("=") - 2);
            for (int j = 0; j < int_data.size(); j++) {
              if (name_ == int_data[j].name) {
                if (!int_data[j].is_multi) {
                  int_data[j].info = std::stoi(val);
                } else {
                  int_data[j].vals->push_back(std::stoi(val));
                }
              }
            }
        } else {
          if (args[i].substr(0, 2) == "--") {
            std::string name = args[i].substr(2, args[i].find("=") - 2);
            std::string val = args[i].substr(args[i].find("=") + 1, args[i].size() - args[i].find("=") + 1);
            for (int j = 0; j < string_data.size(); j++) {
              if (name == string_data[j].name) {
                string_data[j].info = val;
                if (string_data[j].ref_info)
                  *string_data[j].ref_info = val;
                else
                  string_data[j].ref_info = &val;
                string_data[j].is_info = true;
              }
            }
          } else if (args[i].substr(0, 2) != "--") {
            char ch = args[i][1];
            std::string val = args[i].substr(args[i].find("=") + 1, args[i].size() - args[i].find("=") + 1);
            for (int j = 0; j < string_data.size(); j++) {
              if (ch == string_data[j].short_name) {
                string_data[j].info = val;
                if (string_data[j].ref_info)
                  *string_data[j].ref_info = val;
                else
                  string_data[j].ref_info = &val;
                string_data[j].is_info = true;
              }
            }
          }
        }
      } else if (args[i].size() == 1 && args[i][0] >= '0' && args[i][0] <= '9'){
        for (int j = 0; j < int_data.size(); j++){
          if (int_data[j].is_pos){
            int_data[j].vals->push_back(args[i][0] - '0');
          }
        }
      } else {
        if (args[i].substr(0, 2) == "--"){
          std::string name_ = args[i].substr(2, args[i].size()-2);
          for (int j = 0; j < flag_data.size(); j++){
            if (flag_data[j].name == name_){
              flag_data[j].fl = true;

            }
          }
        } else {
          for (int j = 1; j < args[i].size(); j++){
            for (int k = 0; k < flag_data.size(); k++){
              if (flag_data[k].short_name == args[i][j]){
                flag_data[k].fl = true;
                if (!flag_data[k].pfl)
                  flag_data[k].pfl = new bool(true);
                else
                  *flag_data[k].pfl = true;
              }

            }
          }
        }
      }
    }
  }

  for (int i = 0; i < string_data.size(); i++) {
    if (!string_data[i].is_info)
      return false;
  }
  for (int i = 0; i < int_data.size(); i++) {
    if (int_data[i].is_multi && int_data[i].min_args)
      if (int_data[i].vals->size() < int_data[i].min_args_count)
        return false;
  }


  return true;
}
bool ArgParser::Parse(int argc, char** argv) {
  std::vector<std::string> params;
  for (int i = 0; i < argc; i++){
    params.push_back(std::string(argv[i]));
  }
  return Parse(params);
}
ArgString& ArgString::Default(std::string def_val) {
  this->info = def_val;
  is_info = true;
  return *this;
}
ArgString& ArgString::StoreValue(std::string& line) {
  ref_info = &line;
  return *this;
}
ArgString& ArgString::MultiValue(int i) {
  return *this;
}

ArgInt& ArgInt::Default(std::string name_) {
  return *this;
}
ArgInt& ArgInt::StoreValue(std::string line) {
  return *this;
}

ArgInt& ArgInt::MultiValue() {
  is_multi = true;
  return *this;
}
ArgInt& ArgInt::MultiValue(int min_args_count_) {
  min_args_count = min_args_count_;
  is_multi = true;
  min_args = true;
  return *this;
}
ArgInt& ArgInt::StoreValues(std::vector<int>& values) {
  vals = &values;
  return *this;
}
ArgInt& ArgInt::Positional() {
  is_pos = true;
  return *this;
}
ArgFlag& ArgFlag::Default(bool val) {
  *pfl = val;
  fl = val;
  return *this;
}
ArgFlag& ArgFlag::StoreValue(bool& val) {
  pfl = &val;
  return *this;
}
ArgFlag& ArgFlag::MultiValue() {
  return *this;
}
ArgFlag& ArgFlag::MultiValue(int min_args_count) {
  return *this;
}
ArgFlag& ArgFlag::StoreValues(std::vector<int> values) {
  return *this;
}

ArgString& ArgParser::AddStringArgument(std::string name_) {
  ArgString a = ArgString(name_);

  string_data.push_back(a);
  return string_data.back();
}
ArgString& ArgParser::AddStringArgument(char short_name_, std::string name_) {
  ArgString a = ArgString(short_name_, name_);
  string_data.push_back(a);
  return string_data.back();
}
ArgString& ArgParser::AddStringArgument(char ch, std::string name_, std::string info) {
  ArgString a = ArgString();
  return a;
}
std::string ArgParser::GetStringValue(std::string name_) {
  std::string ans;
  for (int i = 0; i < string_data.size(); i++) {
    if (name_ == string_data[i].name)
      ans = string_data[i].info;
  }
  return ans;
}
ArgInt& ArgParser::AddIntArgument(char ch, std::string name_) {
  ArgInt a = ArgInt(ch, name_);
  int_data.push_back(a);
  return int_data.back();
}
ArgInt& ArgParser::AddIntArgument(std::string name_) {
  ArgInt a = ArgInt(name_);
  int_data.push_back(a);
  return int_data.back();
}
ArgInt& ArgParser::AddIntArgument(std::string name_, std::string info_) {
  ArgInt a = ArgInt(name_, info_);
  int_data.push_back(a);
  return int_data.back();
}
int ArgParser::GetIntValue(std::string name) {
  int ans;
  for (int i = 0; i < int_data.size(); i++){
    if (int_data[i].name == name)
      ans = int_data[i].info;
  }
  return ans;
}
int ArgParser::GetIntValue(std::string name, int i) {
  for (int j = 0; j < int_data.size(); j++){
    if (name == int_data[j].name){
      return  int_data[j].vals->at(i);
    }
  }
  return -1;
}
ArgFlag& ArgParser::AddFlag(char ch, std::string name_) {
  ArgFlag a = ArgFlag(ch, name_);
  flag_data.push_back(a);
  return flag_data.back();
}
ArgFlag& ArgParser::AddFlag(char ch, std::string name_, std::string info_) {
  ArgFlag a = ArgFlag(ch, name_, info_);
  flag_data.push_back(a);
  return flag_data.back();
}
ArgFlag& ArgParser::AddFlag(std::string name_, std::string info) {
  ArgFlag a = ArgFlag();
  return a;
}
bool ArgParser::GetFlag(std::string name_) {
  for (int i = 0; i < flag_data.size(); i++){
    if (flag_data[i].name == name_){
      return flag_data[i].fl;
    }
  }
}
ArgHelp& ArgParser::AddHelp(char ch, std::string name_, std::string info_) {
  ArgHelp a = ArgHelp(ch, name_, info_);
  help_info = a;
  return help_info;
}
std::string ArgParser::HelpDescription() {
  std::string a = "no help";
  return a;
}
bool ArgParser::Help() {
  return is_help;
}


