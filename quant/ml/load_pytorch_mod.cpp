#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>
#include <memory>
/* This file serves as a demo for loading and accessing Pytroch model in C++ */

/* 
  Directions:
    1. Install Cmake3 and LibTorch.
      https://pytorch.org/cppdocs/installing.html
      sudo yum -y install cmake3
    2. Install GCC 5 from source
      https://gist.github.com/tyleransom/2c96f53a828831567218eeb7edc2b1e7
    3. Change gcc env path to GCC 5.
     https://cmake.org/pipermail/cmake/2006-October/011638.html
     4. To build cmake --build . --config Release
*/

const std::string path_to_model = "/home/vagrant/Desktop/strategy_studio/localdev/RCM/StrategyStudio/examples/strategies/DummyMLStrategy/saved_dummy_model.pt";
int main(int argc, const char* argv[]) {

  torch::jit::script::Module module;
  try {
    // Deserialize the ScriptModule from a file using torch::jit::load().
    module = torch::jit::load(path_to_model);
  }
  catch (const c10::Error& e) {
    std::cerr << "error loading the model\n";
    return -1;
  }

  std::vector<torch::jit::IValue> inputs;
  inputs.push_back(torch::ones({1, 3, 224, 224}));

  // Execute the model and turn its output into a tensor.
  at::Tensor output = module.forward(inputs).toTensor();;

  std::cout << "Decision (0 for sell, 1 for buy):" <<output.index({0}) << ", with trade size: " << output.index({1}) <<"."<< std::endl;

  output = module.forward(inputs).toTensor();;
  std::cout << "Decision (0 for sell, 1 for buy):" <<output.index({0}) << ", with trade size: " << output.index({1}) <<"."<< std::endl;

  std::cout << "Done.\n";
}