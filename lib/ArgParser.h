#pragma once


#include <iostream>

#include <string>

#include <vector>
namespace ArgumentParser {

class ArgString {

 public:
  char short_name;
  std::string name;
  std::string info;
  std::string* ref_info = nullptr;
  bool is_multi;
  bool is_pos;
  bool is_info = false;
  std::vector<ArgString> values;
  ArgString& Default(std::string def_val);
  ArgString& StoreValue(std::string& line);
  ArgString& MultiValue(int i);
  ArgString(){};
  ArgString(std::string basic_string_1):name(basic_string_1){};
  ArgString(char ch, std::string name_):name(name_), short_name(ch){};
};

class ArgInt {

 public:
  char short_name;
  std::string name;
  std::string desc;
  int info = 30;
  bool is_multi = false;
  std::vector<int>* vals = nullptr;
  int min_args_count;
  bool min_args = false;
  bool is_pos = false;
  ArgInt& Positional();
  ArgInt& StoreValues(std::vector<int>& values);
  ArgInt& MultiValue(int min_args_count);
  ArgInt& MultiValue();
  ArgInt& StoreValue(std::string line);
  ArgInt& Default(std::string name_);
  ArgInt(){};
  ArgInt(std::string name_):name(name_){};
  ArgInt(char ch, std::string name_):name(name_), short_name(ch){};
  ArgInt(std::string name_, std::string info_):desc(info_), name(name_){};
};

class ArgFlag {

 public:
  char short_name;
  std::string name;
  std::string info;
  bool fl = false;
  bool* pfl = &fl;
  ArgFlag& Default(bool val);
  ArgFlag& StoreValue(bool& val);
  ArgFlag(std::string name_) : name(name_) {};
  ArgFlag(char ch, std::string name_) : name(name_), short_name(ch) {};
  ArgFlag(char ch, std::string name_, std::string info_):info(info_), name(name_), short_name(ch){};
  ArgFlag& MultiValue();
  ArgFlag& MultiValue(int min_args_count);
  ArgFlag& StoreValues(std::vector<int> values);
  ArgFlag(){};
};

class ArgHelp {
 public:
  char short_name;
  std::string name;
  std::string info;
  ArgHelp(){};
  ArgHelp(char ch, std::string name_, std::string info_): info(info_), short_name(ch), name(name_){};
};

class ArgParser {

  // Your Implementation here!
  std::string& name;
  std::vector<ArgString> string_data;
  std::vector<ArgInt> int_data;
  std::vector<ArgFlag> flag_data;
  ArgHelp help_info;
  bool is_help;
 public:
  bool Parse(std::vector<std::string> args);
  bool Parse(int argc, char** argv);
  ArgParser(std::string name_) : name(name_) {};

  ArgString& AddStringArgument(std::string name_);
  ArgString& AddStringArgument(char short_name_, std::string name_);
  ArgString& AddStringArgument(char ch, std::string name_, std::string info);
  std::string GetStringValue(std::string name_);

  ArgInt& AddIntArgument(char ch, std::string);
  ArgInt& AddIntArgument(std::string);
  ArgInt& AddIntArgument(std::string name_, std::string info_);
  int GetIntValue(std::string name);
  int GetIntValue(std::string name, int i);

  ArgFlag& AddFlag(char ch, std::string);
  ArgFlag& AddFlag(char ch, std::string, std::string info);
  ArgFlag& AddFlag(std::string name_, std::string info);
  bool GetFlag(std::string name_);
  ArgHelp& AddHelp(char ch, std::string name_, std::string info_);
  std::string HelpDescription();

  bool Help();
};

} // namespace ArgumentParser