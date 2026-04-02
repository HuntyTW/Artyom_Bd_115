#include <gtest/gtest.h>
#include "doubly_linked_list.hpp"

#include <sstream>
#include <string>

// =============================================
// Тесты для DoublyLinkedList<T>
// =============================================

namespace {

class DoublyLinkedListTest : public ::testing::Test {
protected:
    biv::DoublyLinkedList<int> list;
};

} // anonymous namespace

// ====================== Основные операции ======================

TEST_F(DoublyLinkedListTest, DefaultConstructor_EmptyList) {
    EXPECT_EQ(list.get_size(), 0u);
    EXPECT_FALSE(list.has_item(42));
    EXPECT_FALSE(list.has_item(0));
}

TEST_F(DoublyLinkedListTest, PushBack_SingleElement) {
    list.push_back(10);
    EXPECT_EQ(list.get_size(), 1u);
    EXPECT_TRUE(list.has_item(10));
    EXPECT_FALSE(list.has_item(20));
}

TEST_F(DoublyLinkedListTest, PushBack_MultipleElements) {
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    EXPECT_EQ(list.get_size(), 3u);
    EXPECT_TRUE(list.has_item(1));
    EXPECT_TRUE(list.has_item(2));
    EXPECT_TRUE(list.has_item(3));
    EXPECT_FALSE(list.has_item(4));
}

TEST_F(DoublyLinkedListTest, HasItem_AfterPushAndRemove) {
    list.push_back(5);
    list.push_back(6);
    EXPECT_TRUE(list.has_item(5));
    EXPECT_TRUE(list.has_item(6));

    list.remove_first(5);
    EXPECT_FALSE(list.has_item(5));
    EXPECT_TRUE(list.has_item(6));
    EXPECT_EQ(list.get_size(), 1u);
}

// ====================== remove_first ======================

TEST_F(DoublyLinkedListTest, RemoveFirst_EmptyList) {
    EXPECT_FALSE(list.remove_first(999));
    EXPECT_EQ(list.get_size(), 0u);
}

TEST_F(DoublyLinkedListTest, RemoveFirst_NotFound) {
    list.push_back(10);
    list.push_back(20);
    EXPECT_FALSE(list.remove_first(999));
    EXPECT_EQ(list.get_size(), 2u);
    EXPECT_TRUE(list.has_item(10));
    EXPECT_TRUE(list.has_item(20));
}

TEST_F(DoublyLinkedListTest, RemoveFirst_FirstElement) {
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_TRUE(list.remove_first(1));
    EXPECT_EQ(list.get_size(), 2u);
    EXPECT_FALSE(list.has_item(1));
    EXPECT_TRUE(list.has_item(2));
    EXPECT_TRUE(list.has_item(3));
}

TEST_F(DoublyLinkedListTest, RemoveFirst_MiddleElement) {
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    EXPECT_TRUE(list.remove_first(20));
    EXPECT_EQ(list.get_size(), 2u);
    EXPECT_TRUE(list.has_item(10));
    EXPECT_FALSE(list.has_item(20));
    EXPECT_TRUE(list.has_item(30));
}

TEST_F(DoublyLinkedListTest, RemoveFirst_LastElement) {
    list.push_back(100);
    list.push_back(200);
    list.push_back(300);

    EXPECT_TRUE(list.remove_first(300));
    EXPECT_EQ(list.get_size(), 2u);
    EXPECT_TRUE(list.has_item(100));
    EXPECT_TRUE(list.has_item(200));
    EXPECT_FALSE(list.has_item(300));
}

TEST_F(DoublyLinkedListTest, RemoveFirst_MultipleSameValues_RemovesOnlyFirst) {
    list.push_back(7);
    list.push_back(7);
    list.push_back(7);
    list.push_back(8);

    EXPECT_TRUE(list.remove_first(7));
    EXPECT_EQ(list.get_size(), 3u);
    EXPECT_TRUE(list.has_item(7));        // осталось два 7
    EXPECT_TRUE(list.has_item(8));
}

// ====================== print() ======================

TEST_F(DoublyLinkedListTest, Print_EmptyList) {
    std::stringstream buffer;
    auto old_buf = std::cout.rdbuf(buffer.rdbuf());

    list.print();

    std::cout.rdbuf(old_buf);
    EXPECT_EQ(buffer.str(), "\n");
}

TEST_F(DoublyLinkedListTest, Print_SingleElement) {
    list.push_back(42);

    std::stringstream buffer;
    auto old_buf = std::cout.rdbuf(buffer.rdbuf());

    list.print();

    std::cout.rdbuf(old_buf);
    EXPECT_EQ(buffer.str(), "42\n");
}

TEST_F(DoublyLinkedListTest, Print_MultipleElements) {
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::stringstream buffer;
    auto old_buf = std::cout.rdbuf(buffer.rdbuf());

    list.print();

    std::cout.rdbuf(old_buf);
    EXPECT_EQ(buffer.str(), "1 2 3\n");
}

// ====================== Дополнительные сценарии ======================

TEST_F(DoublyLinkedListTest, RemoveAllElementsOneByOne) {
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    EXPECT_TRUE(list.remove_first(10));
    EXPECT_TRUE(list.remove_first(20));
    EXPECT_TRUE(list.remove_first(30));

    EXPECT_EQ(list.get_size(), 0u);
    EXPECT_FALSE(list.has_item(10));
    EXPECT_FALSE(list.has_item(20));
    EXPECT_FALSE(list.has_item(30));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "=== Запуск Google Tests для DoublyLinkedList ===\n\n";
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "\n=== Тесты завершены ===\n";
    return result;
}