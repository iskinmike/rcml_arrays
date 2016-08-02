#include <cstdlib>
#include <exception>

#include <map>
#include <vector>
#include <string>

#include <cstdarg>

#include "module.h"
#include "function_module.h"

#include "array_function_module.h"

#define IID "RCT.Array_functions_module_v107"
const unsigned int COUNT_ARRAY_FUNCTIONS = 4;

#define ADD_ARRAY_FUNCTION1(FUNCTION_NAME)                                \
  array_functions[function_id] = new FunctionData;                        \
  array_functions[function_id]->command_index = function_id + 1;          \
  array_functions[function_id]->count_params = 1;                         \
  array_functions[function_id]->params = new FunctionData::ParamTypes[1]; \
  array_functions[function_id]->params[0] = FunctionData::STRING;         \
  array_functions[function_id]->name = FUNCTION_NAME;                     \
  function_id++;

#define ADD_ARRAY_FUNCTION2(FUNCTION_NAME)                                \
  array_functions[function_id] = new FunctionData;                        \
  array_functions[function_id]->command_index = function_id + 1;          \
  array_functions[function_id]->count_params = 2;                         \
  array_functions[function_id]->params = new FunctionData::ParamTypes[2]; \
  array_functions[function_id]->params[0] = FunctionData::STRING;         \
  array_functions[function_id]->params[1] = FunctionData::FLOAT;          \
  array_functions[function_id]->name = FUNCTION_NAME;                     \
  function_id++;

#define ADD_ARRAY_FUNCTION3(FUNCTION_NAME)                                \
  array_functions[function_id] = new FunctionData;                        \
  array_functions[function_id]->command_index = function_id + 1;          \
  array_functions[function_id]->count_params = 3;                         \
  array_functions[function_id]->params = new FunctionData::ParamTypes[3]; \
  array_functions[function_id]->params[0] = FunctionData::STRING;         \
  array_functions[function_id]->params[1] = FunctionData::FLOAT;          \
  array_functions[function_id]->params[2] = FunctionData::FLOAT;          \
  array_functions[function_id]->name = FUNCTION_NAME;                     \
  function_id++;

#define DEFINE_ALL_FUNCTIONS    \
  ADD_ARRAY_FUNCTION2("create") \
  ADD_ARRAY_FUNCTION1("delete") \
  ADD_ARRAY_FUNCTION3("set")    \
  ADD_ARRAY_FUNCTION2("get")


ArrayFunctionModule::ArrayFunctionModule() {
  mi = new ModuleInfo;
  mi->uid = IID;
  mi->mode = ModuleInfo::Modes::PROD;
  mi->version = BUILD_NUMBER;
  mi->digest = NULL; 

  array_functions = new FunctionData *[COUNT_ARRAY_FUNCTIONS];
  system_value function_id = 0;
  DEFINE_ALL_FUNCTIONS
};

const struct ModuleInfo &ArrayFunctionModule::getModuleInfo() { return *mi; }

FunctionData **ArrayFunctionModule::getFunctions(unsigned int *count_functions) {
  *count_functions = COUNT_ARRAY_FUNCTIONS;
  return array_functions;
};

void *ArrayFunctionModule::writePC(unsigned int *buffer_length) {
  *buffer_length = 0;
  return NULL;
}

int ArrayFunctionModule::init() { return 0; }
int ArrayFunctionModule::readPC(int pc_index, void *buffer, unsigned int buffer_length) { return 0; }
int ArrayFunctionModule::startProgram(int run_index, int pc_index) { return 0; }

void ArrayMap::create(std::string name, int size){
  if (!arrays.count(name)) {
    std::vector<float> empty_vector(size, 0);
    arrays.emplace(name, empty_vector);
  } else {
    throw std::exception();
  }
}; 
void ArrayMap::remove(std::string name){
  if (arrays.count(name)) {
    arrays.erase(name);
  } else {
    throw std::exception();
  }
}; 
void ArrayMap::set(std::string name, size_t position, float value){
  if (arrays.count(name)) {
    if ( (arrays[name].size() - 1) < position) {
      arrays[name][position] = value;
    } else {
      throw std::exception();
    }
  } else {
    throw std::exception();
  }
};
float ArrayMap::get(std::string name, size_t position){
  if (arrays.count(name)) {
    if ( (arrays[name].size() - 1) < position) {
      return arrays[name][position];
    } else {
      throw std::exception();
    }
  } else {
    throw std::exception();
  }
};



FunctionResult *ArrayFunctionModule::executeFunction(int run_index, system_value function_index,
                                                    void **args) { 
  if ((function_index < 1) || (function_index > ((int)COUNT_ARRAY_FUNCTIONS))) {
    return NULL;
  }

  try {
    variable_value rez = 0;
    switch (function_index) {
      case 1: {
        const char* name = (const char *)args[0];
        variable_value *size = (variable_value *)args[1];

        module_array.create(name, *size);
        break;
      }
      case 2: {
        const char* name = (const char *)args[0];
        module_array.remove(name);
        break;
      }
      case 3: {
        const char* name = (const char *)args[0];
        variable_value *position = (variable_value *)args[1];
        variable_value *value = (variable_value *)args[2];

        module_array.set(name, *position, *value);
        break;
      }
      case 4: {
        const char* name = (const char *)args[0];
        variable_value *position = (variable_value *)args[1];

        rez = module_array.get(name, *position);
        break;
      }
    }
    return new FunctionResult(FunctionResult::Types::VALUE, rez);
  } catch (...) {
    return new FunctionResult(FunctionResult::Types::EXCEPTION);
  }
};

int ArrayFunctionModule::endProgram(int run_index) { return 0; }

void ArrayFunctionModule::destroy() {
  delete mi;
  for (unsigned int j = 0; j < COUNT_ARRAY_FUNCTIONS; ++j) {
    if (array_functions[j]->count_params) {
      delete[] array_functions[j]->params;
    }
    delete array_functions[j];
  }
  delete[] array_functions;
  delete this;
};

PREFIX_FUNC_DLL unsigned short getFunctionModuleApiVersion() {
  return MODULE_API_VERSION;
};

PREFIX_FUNC_DLL FunctionModule *getFunctionModuleObject() {
  return new ArrayFunctionModule();
};
