#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
using namespace std;


TEST(test_empty_false) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    ASSERT_FALSE(bst.empty());
}

TEST(test_empty_tree) {
    BinarySearchTree<int> bst;
    ASSERT_TRUE(bst.empty());
    ASSERT_TRUE(bst.size() == 0);
    ASSERT_EQUAL(bst.height(), 0);
}

TEST(test_single_element) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    ASSERT_FALSE(bst.empty());
    ASSERT_TRUE(bst.size() == 1);
    ASSERT_EQUAL(bst.height(), 1);
}

TEST(test_size) {
    BinarySearchTree<int> bst;
    bst.insert(1);
    bst.insert(3);
    bst.insert(5);
    ASSERT_EQUAL(bst.size(), 3);

    //multiple rows
    BinarySearchTree<int> bst2;
    bst2.insert(5);
    bst2.insert(2);
    bst2.insert(9);
    bst2.insert(10);
    bst2.insert(7);
    bst2.insert(4);
    bst2.insert(1);
    ASSERT_EQUAL(bst2.size(), 7);

}

TEST(test_height_one_row) {
    BinarySearchTree<int> bst;
    bst.insert(1);
    bst.insert(3);
    bst.insert(5);
    ASSERT_EQUAL(bst.height(), 3);

}

TEST(test_height_left_and_right) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst.insert(4);
    ASSERT_EQUAL(bst.height(), 3);
}

TEST(test_height_loop) {
    BinarySearchTree<int> bst;
    for(int i =0; i<10; ++i) {
        bst.insert(i);
    }
    ASSERT_EQUAL(bst.height(), 10);
}

TEST(test_height_branches) {
    BinarySearchTree<int> bst;
    bst.insert(4);
    bst.insert(2);
    bst.insert(6);
    ASSERT_TRUE(bst.height() == 2);
    bst.insert(1);
    bst.insert(3);
    bst.insert(5);
    bst.insert(7);
    ASSERT_EQUAL(bst.height(), 3);
}


TEST(test_copy_ctor_empty) {
    BinarySearchTree<int> bst;
    BinarySearchTree<int> bst_copy = BinarySearchTree(bst);
    ASSERT_TRUE(bst_copy.empty());
}

TEST(test_copy_ctor) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    BinarySearchTree<int> bst_copy = BinarySearchTree(bst);
    ASSERT_EQUAL(bst_copy.find(1), bst_copy.begin());

    for (auto iter = bst.begin(); iter != bst.end(); ++iter) {
        for (auto iter2 = bst_copy.begin(); iter2 != bst_copy.end(); ++iter2) {
             ASSERT_NOT_EQUAL(&(*(iter)), &(*(iter2)));
             ASSERT_NOT_EQUAL(iter, iter2);
        }
    }
    
    //compare addresses 
    ASSERT_NOT_EQUAL(&(*bst_copy.find(1)), &(*bst.find(1)));
    ASSERT_NOT_EQUAL(&(*bst_copy.find(5)), &(*bst.find(5)));
    ASSERT_NOT_EQUAL(&(*bst_copy.find(3)), &(*bst.find(3)));

    //compare addresses using ++
    ASSERT_NOT_EQUAL(&(*bst_copy.begin()), &(*bst.begin()));
    ASSERT_NOT_EQUAL(&(*(++bst_copy.begin())), &(*(++bst.begin())));
}

TEST(test_assign_operator_self_empty) {
    BinarySearchTree<int> bst;
    bst = bst;
    ASSERT_TRUE(bst.empty());
}

TEST(test_assign_operator_self) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst = bst;
    ASSERT_EQUAL(*bst.begin(), 1);
    ASSERT_NOT_EQUAL(*bst.begin(), 7);
}

TEST(find_elt) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst.insert(4);
    ASSERT_EQUAL(bst.find(1), bst.begin());
    ASSERT_NOT_EQUAL(bst.find(5), bst.begin());
}

TEST(test_insert_one_row) {
    BinarySearchTree<int> bst;
    bst.insert(1);
    bst.insert(3);
    bst.insert(5);
    ASSERT_EQUAL(bst.size(), 3);
    ASSERT_TRUE(bst.check_sorting_invariant());
}

TEST(test_insert_left_and_right) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst.insert(4);
    bst.insert(2);
    ASSERT_EQUAL(bst.size(), 5);
    ASSERT_TRUE(bst.check_sorting_invariant());
}

TEST(test_sorting_invariant) {
    
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst.insert(4);
    bst.insert(2);
    *bst.begin() = 9;
    ASSERT_FALSE(bst.check_sorting_invariant());

    //empty tree
    BinarySearchTree<int> bst2;
    ASSERT_TRUE(bst2.check_sorting_invariant());

    //from piazza
    BinarySearchTree<int> bst3;
    bst3.insert(1);
    bst3.insert(0);
    // change first datum to 2, resulting in the first broken tree above
    *bst3.begin() = 2;
    ASSERT_FALSE(bst3.check_sorting_invariant());  

    //checking subtree 
    BinarySearchTree<int> bst4;
    bst4.insert(4);
    bst4.insert(5);
    bst4.insert(1);
    bst4.insert(2);
    bst4.insert(3);
    *(++++bst4.begin()) = 7;
    ASSERT_FALSE(bst4.check_sorting_invariant());
}

TEST(test_min_elt) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst.insert(4);
    bst.insert(2);
    ASSERT_EQUAL(*bst.min_element(), 1);
}

TEST(test_min_elt_negative) {
    BinarySearchTree<int> bst;
    bst.insert(-2);
    bst.insert(-5);
    bst.insert(1);
    bst.insert(-4);
    bst.insert(-9);
    ASSERT_EQUAL(*bst.min_element(), -9);
}

TEST(test_max_elt) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    bst.insert(4);
    bst.insert(2);
    ASSERT_EQUAL(*bst.max_element(), 5);
}

TEST(test_max_elt_negative) {
    BinarySearchTree<int> bst;
    bst.insert(-2);
    bst.insert(-5);
    bst.insert(-1);
    bst.insert(-4);
    bst.insert(-9);
    ASSERT_EQUAL(*bst.max_element(), -1);
}

TEST(test_min_greater_than_impl) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    ASSERT_TRUE(*bst.min_greater_than(4) == 5);
}

TEST(test_min_greater_than_impl_same_elt) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    ASSERT_TRUE(*bst.min_greater_than(3) == 5);
}

TEST(test_min_greater_than_impl_multiple) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    ASSERT_TRUE(*bst.min_greater_than(2) == 3);
}

TEST(test_min_greater_than_impl_multiple_and_same) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    ASSERT_TRUE(*bst.min_greater_than(1) == 3);
}

TEST(test_min_greater_than_impl_none) {
    BinarySearchTree<int> bst;
    bst.insert(3);
    bst.insert(5);
    bst.insert(1);
    ASSERT_FALSE(bst.min_greater_than(6) == bst.begin());
    ASSERT_TRUE(bst.min_greater_than(6) == bst.end());
}

TEST(test_min_greater_than_empty) {
    BinarySearchTree<int> bst;
    BinarySearchTree<int>::Iterator pointer = bst.min_greater_than(1);
    ASSERT_EQUAL(pointer, bst.end());
}

TEST(test_inorder) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  bst.insert(5);
  bst.insert(1);
  ostringstream oss_inorder;
  bst.traverse_inorder(oss_inorder);
  ASSERT_TRUE(oss_inorder.str() == "1 3 5 ");
}

TEST(test_inorder_single_element) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  ostringstream oss_inorder;
  bst.traverse_inorder(oss_inorder);
  ASSERT_TRUE(oss_inorder.str() == "3 ");
}

TEST(test_inorder_empty) {
  BinarySearchTree<int> bst;
  ostringstream oss_preorder;
  bst.traverse_inorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "");
}

TEST(test_preorder) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  bst.insert(5);
  bst.insert(1);
  ostringstream oss_preorder;
  bst.traverse_preorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "3 1 5 ");
}
 
TEST(test_preorder_single_element) {
  BinarySearchTree<int> bst;
  bst.insert(3);
  ostringstream oss_preorder;
  bst.traverse_preorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "3 ");
}

TEST(test_preorder_empty) {
  BinarySearchTree<int> bst;
  ostringstream oss_preorder;
  bst.traverse_preorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "");
}


TEST_MAIN()
