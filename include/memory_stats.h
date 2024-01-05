/*
 * =====================================================================================
 *
 *       Filename:  memory_stats.h
 *
 *    Description: A basic memory usage class 
 *
 *        Version:  1.0
 *        Created:  05/01/2024 19:40:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Thabo Njabulo Twala (), thabonjabulotwala@gmail.com
 *   Organization:  Unemployed Co.
 *
 * =====================================================================================
 */
#ifndef MEMORY_USAGE_H
#define MEMORY_USAGE_H
#include <iostream>
long long used = 0;
long long freed = 0;
void* operator new(std::size_t size){
		used += size;
		return malloc(size);
}
void operator delete(void* mem, std::size_t size){
	freed += size;
}
long long memory_used(){
	return used - freed;
}
#endif
