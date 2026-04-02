#include <gtest/gtest.h>
#include "vector.hpp"

#include <sstream>
#include <iostream>
#include <string>

// Вспомогательная функция для перехвата вывода print()
template<typename T>
std::string capture_print(const biv::Vector<T>& vec) {
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
    vec.print();
    std::cout.rdbuf(old_cout);
    return oss.str();
}

namespace {

// ====================== Vector<int> ======================

TEST(VectorTest, DefaultConstructor) {
    biv::Vector<int> v;
    EXPECT_EQ(v.get_size(), 0);
    EXPECT_EQ(capture_print(v), "\n");  // пустой вектор печатает только перевод строки
}

TEST(VectorTest, PushBack) {
    biv::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v.get_size(), 3);
    EXPECT_TRUE(v.has_item(10));
    EXPECT_TRUE(v.has_item(20));
    EXPECT_TRUE(v.has_item(30));
    EXPECT_FALSE(v.has_item(999));

    EXPECT_EQ(capture_print(v), "10 20 30\n");
}

TEST(VectorTest, PushBackGrowth) {
    biv::Vector<int> v;
    for (int i = 1; i <= 20; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(v.get_size(), 20);
    std::string expected;
    for (int i = 1; i <= 20; ++i) {
        if (i > 1) expected += " ";
        expected += std::to_string(i);
    }
    expected += "\n";
    EXPECT_EQ(capture_print(v), expected);
}

TEST(VectorTest, HasItem) {
    biv::Vector<int> v;
    EXPECT_FALSE(v.has_item(42));

    v.push_back(42);
    v.push_back(100);
    EXPECT_TRUE(v.has_item(42));
    EXPECT_TRUE(v.has_item(100));
    EXPECT_FALSE(v.has_item(0));
}

TEST(VectorTest, InsertValidPositions) {
    biv::Vector<int> v;
    v.push_back(10);
    v.push_back(30);

    // вставка в середину
    EXPECT_TRUE(v.insert(1, 20));
    EXPECT_EQ(v.get_size(), 3);
    EXPECT_EQ(capture_print(v), "10 20 30\n");

    // вставка в начало
    EXPECT_TRUE(v.insert(0, 5));
    EXPECT_EQ(v.get_size(), 4);
    EXPECT_EQ(capture_print(v), "5 10 20 30\n");

    // вставка в конец (position == size)
    EXPECT_TRUE(v.insert(4, 40));
    EXPECT_EQ(v.get_size(), 5);
    EXPECT_EQ(capture_print(v), "5 10 20 30 40\n");
}

TEST(VectorTest, InsertInvalidPosition) {
    biv::Vector<int> v;
    v.push_back(10);

    EXPECT_FALSE(v.insert(2, 999));   // позиция > size
    EXPECT_EQ(v.get_size(), 1);       // размер не изменился
    EXPECT_EQ(capture_print(v), "10\n");

    EXPECT_FALSE(v.insert(100, 999));
    EXPECT_EQ(v.get_size(), 1);
}

TEST(VectorTest, RemoveFirst) {
    biv::Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(10);
    v.push_back(30);

    EXPECT_TRUE(v.remove_first(10));          // удаляем первое вхождение
    EXPECT_EQ(v.get_size(), 3);
    EXPECT_EQ(capture_print(v), "20 10 30\n");

    EXPECT_TRUE(v.remove_first(10));          // удаляем оставшееся
    EXPECT_EQ(v.get_size(), 2);
    EXPECT_EQ(capture_print(v), "20 30\n");

    EXPECT_FALSE(v.remove_first(999));        // несуществующий элемент
    EXPECT_EQ(v.get_size(), 2);               // размер не изменился
}

TEST(VectorTest, RemoveFromEmpty) {
    biv::Vector<int> v;
    EXPECT_FALSE(v.remove_first(5));
    EXPECT_EQ(v.get_size(), 0);
    EXPECT_EQ(capture_print(v), "\n");
}

TEST(VectorTest, RemoveAllElements) {
    biv::Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    EXPECT_TRUE(v.remove_first(1));
    EXPECT_TRUE(v.remove_first(2));
    EXPECT_TRUE(v.remove_first(3));

    EXPECT_EQ(v.get_size(), 0);
    EXPECT_EQ(capture_print(v), "\n");
}

// ====================== Vector<std::string> (проверка шаблона) ======================

TEST(VectorTest, StringVector) {
    biv::Vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    v.push_back("!");

    EXPECT_EQ(v.get_size(), 3);
    EXPECT_TRUE(v.has_item("world"));
    EXPECT_FALSE(v.has_item("foo"));

    EXPECT_EQ(capture_print(v), "hello world !\n");

    EXPECT_TRUE(v.insert(1, "beautiful"));
    EXPECT_EQ(capture_print(v), "hello beautiful world !\n");

    EXPECT_TRUE(v.remove_first("world"));
    EXPECT_EQ(capture_print(v), "hello beautiful !\n");
}

} 

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}