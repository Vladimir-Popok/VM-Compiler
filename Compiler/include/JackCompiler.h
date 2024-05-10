#include "CompilationEngine.h"
#include <filesystem>

namespace fs = std::filesystem;

std::string relative_path(const std::string& fullPath);

int compile(int argc, char** argv);
