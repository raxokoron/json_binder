#pragma once

#ifndef ASSERT
#define ASSERT(eval) { if(!(eval)) { std::cout << "ASSERT: " << (#eval) << std::endl; std::terminate(); } }
#endif