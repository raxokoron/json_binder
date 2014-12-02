#pragma once

#ifndef ASSERT
#include <iostream>
#define ASSERT(eval) { if(!(eval)) { std::cout << "ASSERT: " << (#eval) << std::endl; std::terminate(); } }
#endif