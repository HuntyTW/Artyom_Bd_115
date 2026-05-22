#include "long_number.hpp"
#include <stdexcept>

using biv::LongNumber;
		
LongNumber::LongNumber() {
	length = 1;
	sign = 1;
	numbers = new int[1];
	numbers[0] = 0;
}

LongNumber::LongNumber(int length, int sign) {
	this->length = length > 0 ? length : 1;
	this->sign = sign;
	this->numbers = new int[this->length]();
}

LongNumber::LongNumber(const char* const str) {
	int start = 0;
	int s = 1;
	if (str[0] == '-') {
		s = -1;
		start = 1;
	} else if (str[0] == '+') {
		start = 1;
	}
	
	int len = 0;
	while (str[start + len] >= '0' && str[start + len] <= '9') {
		len++;
	}
	
	if (len == 0) {
		this->length = 1;
		this->sign = 1;
		this->numbers = new int[1]{0};
		return;
	}
	
	this->length = len;
	this->sign = s;
	this->numbers = new int[len];
	for (int i = 0; i < len; ++i) {
		this->numbers[i] = str[start + len - 1 - i] - '0';
	}
	trim();
}

LongNumber::LongNumber(const LongNumber& x) {
	length = x.length;
	sign = x.sign;
	numbers = new int[length];
	for (int i = 0; i < length; ++i) {
		numbers[i] = x.numbers[i];
	}
}

LongNumber::LongNumber(LongNumber&& x) {
	length = x.length;
	sign = x.sign;
	numbers = x.numbers;
	
	x.numbers = nullptr;
	x.length = 0;
	x.sign = 1;
}

LongNumber::~LongNumber() {
	delete[] numbers;
}

LongNumber& LongNumber::operator = (const char* const str) {
	delete[] numbers;
	int start = 0;
	int s = 1;
	if (str[0] == '-') {
		s = -1;
		start = 1;
	} else if (str[0] == '+') {
		start = 1;
	}
	int len = 0;
	while (str[start + len] >= '0' && str[start + len] <= '9') {
		len++;
	}
	if (len == 0) {
		this->length = 1;
		this->sign = 1;
		this->numbers = new int[1]{0};
		return *this;
	}
	this->length = len;
	this->sign = s;
	this->numbers = new int[len];
	for (int i = 0; i < len; ++i) {
		this->numbers[i] = str[start + len - 1 - i] - '0';
	}
	trim();
	return *this;
}

LongNumber& LongNumber::operator = (const LongNumber& x) {
	if (this != &x) {
		delete[] numbers;
		length = x.length;
		sign = x.sign;
		numbers = new int[length];
		for (int i = 0; i < length; ++i) {
			numbers[i] = x.numbers[i];
		}
	}
	return *this;
}

LongNumber& LongNumber::operator = (LongNumber&& x) {
	if (this != &x) {
		delete[] numbers;
		length = x.length;
		sign = x.sign;
		numbers = x.numbers;
		
		x.numbers = nullptr;
		x.length = 0;
		x.sign = 1;
	}
	return *this;
}

bool LongNumber::operator == (const LongNumber& x) const {
	if (sign != x.sign || length != x.length) return false;
	for (int i = 0; i < length; ++i) {
		if (numbers[i] != x.numbers[i]) return false;
	}
	return true;
}

bool LongNumber::operator != (const LongNumber& x) const {
	return !(*this == x);
}

bool LongNumber::operator < (const LongNumber& x) const {
	if (sign != x.sign) {
		return sign < x.sign;
	}
	if (sign == 1) {
		return compare_absolute(x) < 0;
	} else {
		return compare_absolute(x) > 0;
	}
}

bool LongNumber::operator > (const LongNumber& x) const {
	return x < *this;
}

LongNumber LongNumber::operator + (const LongNumber& x) const {
	if (sign == x.sign) {
		LongNumber res = add_absolute(x);
		res.sign = sign;
		return res;
	} else {
		int cmp = compare_absolute(x);
		if (cmp >= 0) {
			LongNumber res = sub_absolute(x);
			res.sign = sign;
			return res;
		} else {
			LongNumber res = x.sub_absolute(*this);
			res.sign = x.sign;
			return res;
		}
	}
}

LongNumber LongNumber::operator - (const LongNumber& x) const {
	if (sign != x.sign) {
		LongNumber res = add_absolute(x);
		res.sign = sign;
		return res;
	} else {
		int cmp = compare_absolute(x);
		if (cmp >= 0) {
			LongNumber res = sub_absolute(x);
			res.sign = sign;
			return res;
		} else {
			LongNumber res = x.sub_absolute(*this);
			res.sign = -sign;
			return res;
		}
	}
}

LongNumber LongNumber::operator * (const LongNumber& x) const {
	LongNumber res(length + x.length, sign * x.sign);
	for (int i = 0; i < length; ++i) {
		long long carry = 0;
		for (int j = 0; j < x.length || carry; ++j) {
			long long cur = res.numbers[i + j] + (long long)numbers[i] * (j < x.length ? x.numbers[j] : 0) + carry;
			res.numbers[i + j] = cur % 10;
			carry = cur / 10;
		}
	}
	res.trim();
	return res;
}

LongNumber LongNumber::operator / (const LongNumber& x) const {
	LongNumber q, r;
	divide(*this, x, q, r);
	return q;
}

LongNumber LongNumber::operator % (const LongNumber& x) const {
	LongNumber q, r;
	divide(*this, x, q, r);
	return r;
}

bool LongNumber::is_negative() const noexcept {
	return sign == -1;
}

// ----------------------------------------------------------
// PRIVATE
// ----------------------------------------------------------
int LongNumber::get_length(const char* const str) const noexcept {
	int start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
	int len = 0;
	while (str[start + len] >= '0' && str[start + len] <= '9') {
		len++;
	}
	return len;
}

void LongNumber::trim() {
	while (length > 1 && numbers[length - 1] == 0) {
		length--;
	}
	if (length == 1 && numbers[0] == 0) {
		sign = 1;
	}
}

int LongNumber::compare_absolute(const LongNumber& x) const {
	if (length != x.length) {
		return length < x.length ? -1 : 1;
	}
	for (int i = length - 1; i >= 0; --i) {
		if (numbers[i] != x.numbers[i]) {
			return numbers[i] < x.numbers[i] ? -1 : 1;
		}
	}
	return 0;
}

LongNumber LongNumber::add_absolute(const LongNumber& x) const {
	int max_len = (length > x.length ? length : x.length) + 1;
	LongNumber res(max_len, 1);
	int carry = 0;
	for (int i = 0; i < max_len - 1 || carry; ++i) {
		long long sum = carry;
		if (i < length) sum += numbers[i];
		if (i < x.length) sum += x.numbers[i];
		res.numbers[i] = sum % 10;
		carry = sum / 10;
	}
	res.trim();
	return res;
}

LongNumber LongNumber::sub_absolute(const LongNumber& x) const {
	LongNumber res(length, 1);
	int borrow = 0;
	for (int i = 0; i < length; ++i) {
		int diff = numbers[i] - borrow;
		if (i < x.length) diff -= x.numbers[i];
		if (diff < 0) {
			diff += 10;
			borrow = 1;
		} else {
			borrow = 0;
		}
		res.numbers[i] = diff;
	}
	res.trim();
	return res;
}

void LongNumber::divide(const LongNumber& dividend, const LongNumber& divisor, LongNumber& quotient, LongNumber& remainder) {
	if (divisor.length == 1 && divisor.numbers[0] == 0) {
		throw std::invalid_argument("Division by zero");
	}

	LongNumber abs_divisor = divisor;
	abs_divisor.sign = 1;
	
	LongNumber abs_quotient(dividend.length, 1);
	LongNumber abs_remainder; 

	for (int i = dividend.length - 1; i >= 0; --i) {
		if (!(abs_remainder.length == 1 && abs_remainder.numbers[0] == 0)) {
			int* new_nums = new int[abs_remainder.length + 1];
			new_nums[0] = dividend.numbers[i];
			for (int j = 0; j < abs_remainder.length; ++j) {
				new_nums[j + 1] = abs_remainder.numbers[j];
			}
			delete[] abs_remainder.numbers;
			abs_remainder.numbers = new_nums;
			abs_remainder.length++;
		} else {
			abs_remainder.numbers[0] = dividend.numbers[i];
		}
		abs_remainder.trim();
		
		int d = 0;
		while (abs_remainder.compare_absolute(abs_divisor) >= 0) {
			abs_remainder = abs_remainder.sub_absolute(abs_divisor);
			d++;
		}
		abs_quotient.numbers[i] = d;
	}
	abs_quotient.trim();

	bool is_rem_zero = (abs_remainder.length == 1 && abs_remainder.numbers[0] == 0);

	if (dividend.sign == 1) {
		quotient = abs_quotient;
		quotient.sign = divisor.sign;
		remainder = abs_remainder;
	} else {
		if (is_rem_zero) {
			quotient = abs_quotient;
			quotient.sign = -divisor.sign;
			remainder = abs_remainder;
		} else {
			remainder = abs_divisor.sub_absolute(abs_remainder);
			LongNumber one("1");
			quotient = abs_quotient.add_absolute(one);
			if (divisor.sign == 1) {
				quotient.sign = -1;
			} else {
				quotient.sign = 1;
			}
		}
	}
	quotient.trim();
	remainder.trim();
}

// ----------------------------------------------------------
// FRIENDLY
// ----------------------------------------------------------
namespace biv {
	std::ostream& operator << (std::ostream &os, const LongNumber& x) {
		if (x.sign == -1 && !(x.length == 1 && x.numbers[0] == 0)) {
			os << '-';
		}
		for (int i = x.length - 1; i >= 0; --i) {
			os << x.numbers[i];
		}
		return os;
	}
}