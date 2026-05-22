#pragma once

#include <iostream>

namespace biv {
	class LongNumber {
		private:
			int* numbers;
			int length;
			int sign; // 1 для положительных и нуля, -1 для отрицательных
		
		public:
			LongNumber();
			LongNumber(const char* const str);
			LongNumber(const LongNumber& x);
			LongNumber(LongNumber&& x);
			
			~LongNumber();
			
			LongNumber& operator = (const char* const str);
			LongNumber& operator = (const LongNumber& x);
			LongNumber& operator = (LongNumber&& x);
			
			bool operator == (const LongNumber& x) const;
			bool operator != (const LongNumber& x) const;
			bool operator > (const LongNumber& x) const;
			bool operator < (const LongNumber& x) const;
			
			LongNumber operator + (const LongNumber& x) const;
			LongNumber operator - (const LongNumber& x) const;
			LongNumber operator * (const LongNumber& x) const;
			LongNumber operator / (const LongNumber& x) const;
			LongNumber operator % (const LongNumber& x) const;
			
			bool is_negative() const noexcept;
			
			friend std::ostream& operator << (std::ostream &os, const LongNumber& x);
			
		private:
			// Приватный конструктор, который присутствовал в скелете cpp
			LongNumber(int length, int sign);
			int get_length(const char* const str) const noexcept;
			
			// Вспомогательные методы для реализации арифметики
			void trim(); 
			int compare_absolute(const LongNumber& x) const;
			LongNumber add_absolute(const LongNumber& x) const;
			LongNumber sub_absolute(const LongNumber& x) const;
			static void divide(const LongNumber& dividend, const LongNumber& divisor, LongNumber& quotient, LongNumber& remainder);
	};
}