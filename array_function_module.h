
/*
* File: array_function_module.h
* Author: m79lol, iskinmike
*
*/

#ifndef ARRAY_FUNCTION_MODULE_H
#define ARRAY_FUNCTION_MODULE_H

class ArrayMap
{
  std::map<std::string, std::vector<float>> arrays;
public:
  void create(std::string name, int size); 
  void delete(std::string name); 
  void set(std::string name, int position, float value);
  float get(std::string name, int position);
  ArrayMap(){};
  ~ArrayMap(){};
};

class ArrayFunctionModule : public FunctionModule {
  FunctionData **math_functions;
  ArrayMap module_array;

#if MODULE_API_VERSION > 000
  ModuleInfo *mi;
#endif

 public:
  ArrayFunctionModule();

  // init
  const struct ModuleInfo &getModuleInfo();
  void prepare(colorPrintfModule_t *colorPrintf_p,
               colorPrintfModuleVA_t *colorPrintfVA_p){};

  // compiler only
  FunctionData **getFunctions(unsigned int *count_functions);
  void *writePC(unsigned int *buffer_length);

  // intepreter - devices
  int init();
  void final() {};
  
  // intepreter - program & lib
  int readPC(int pc_index, void *buffer, unsigned int buffer_length);
  // intepreter - program
  int startProgram(int run_index, int pc_index);
  FunctionResult *executeFunction(int run_index, system_value function_index, void **args);
  int endProgram(int run_index);

  // destructor
  void destroy();
  ~ArrayFunctionModule(){};
};



#endif /* ARRAY_FUNCTION_MODULE_H  */
