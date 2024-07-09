#pragma once

// Standard Library
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstring>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include<array>
#include<numeric>
#include<iterator>
#include<initializer_list>
#include<fstream>
#include<chrono>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<atomic>
#include<filesystem>
#include <tuple>
#include <variant>
#include <optional>
#include<map>
#include<string_view>

// Meta-programming
#include <type_traits>


#ifdef FD_PLATFORM_WINDOWS
  #include <Windows.h>
#endif


//Third Party
#include<glad/glad.h>
#include <GLFW/glfw3.h>

//glm
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // For glm::ortho




